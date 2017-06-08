#include "StdAfx.h"
#include "SecServerSock.h"
//#include "wemsservice.h"
//#include "wemsclientsock.h"
#include "SecServer.h"
#include "Sec/SecClientSock.h"
#include "SecDataProc.h"


int __stdcall PacketDataProc(wemsGPN_st* stSrcProc, wemsGPN_st* stDestProc, wemsDataPacket_st* pRcvData)
{
	SecDataProc::GetInstance()->PacketDataProc(stSrcProc, stDestProc, pRcvData);
	return 0;
}


#define m_dwThreadWait			5
SecServerSock::SecServerSock(void)
{
}


SecServerSock::~SecServerSock(void)
{
}



int		SecServerSock::Init()
{
	// ���� �ʱ�ȭ ������ ����
	return 0;
}

void SecServerSock::Activate()
{
	shutdown_event_.Reset();
	
	Start();
}

void SecServerSock::Abort()
{
	shutdown_event_.Set();
	Wait();
}

int SecServerSock::Run()
{
	int				nRet = 0;
	DWORD			dwRet = 0x00;
	int				nCount = 0;
	int					nOptValue      = 0; //< SOCKET OPT
	WSAEVENT			sockEvent;			//< SOCKET EVENT(�̺�Ʈ ��ü)
	WSANETWORKEVENTS	events;				//< ������ ��Ʈ��ũ �̺�Ʈ ������ �����ϱ� ���� ����ü
	char				strAddr[128];
	UINT				nPort;

	try
	{
		nPort = 39701;
		memset(strAddr, 0x00, 128);

		listen_socket_.Create();
		// ���� ���� (SOL_SOCKET:���� ���������� ó��, IPPROTO_TCP:�������� ���������� ó��)
		listen_socket_.SetSockOpt(SO_REUSEADDR, (char*)&nOptValue, sizeof(nOptValue), SOL_SOCKET); //< ���� �ɼ� ����
		sockEvent = WSACreateEvent(); //< �̺�Ʈ ��ü�� ����ϴ�.
		if(sockEvent != WSA_INVALID_EVENT)
		{//����
			nRet = WSAEventSelect(SOCKET(listen_socket_), sockEvent, FD_ACCEPT); //< �̺�Ʈ ��ü�� ���� ��Ʈ��ũ �̺�Ʈ�� �����մϴ�.
			if(nRet != SOCKET_ERROR)
			{
// 				wemsNodeName_st	OwnNodeName = WS_SERVICE->GetOwnProc()->stNodeName;
// 				wemsNodeInfo_st* OwnNodeInfo = (wemsNodeInfo_st*)WEMS_SHM->shm_GetPhysicalNodeInfo(OwnNodeName.eNodeType, OwnNodeName.eNodeRank, OwnNodeName.eNodeGroup);
// 				
// 				if(OwnNodeInfo == NULL)
// 					return -1;

				listen_socket_.Bind(39701); // �ӽ÷� ��Ʈ ����(���� ���ڰ����� �޾ƾ���)
				listen_socket_.Listen();
				listen_socket_.GetSockName(strAddr, nPort);

				WLOG("server socket listen(%s:%d) suc\n", strAddr, nPort);
				// WEMS_SHM->shm_SetProcConn(WS_SERVICE->GetOwnProc()->stProcName.iCopy, WS_SERVICE->GetOwnProc()->stProcName.szProcName, TRUE);
			}
		}

		while(true)
		{
			dwRet = ::WaitForSingleObject( shutdown_event_.GetEvent(), m_dwThreadWait );
			if( dwRet  == WAIT_OBJECT_0 ) //�̺�Ʈ �߻���
			{
				WLOG("server listen socket thread exit(event)\n");
				break;
			}

			//< ���� ���� �˻�.
			dwRet = WSAWaitForMultipleEvents(1, &sockEvent, FALSE, 100, FALSE); //< �̺�Ʈ ��ü�� ��ȣ ����
			if(dwRet == WSA_WAIT_TIMEOUT)
				continue;

			nRet = WSAEnumNetworkEvents(SOCKET(listen_socket_), sockEvent, &events); //< ��ü���� ��Ʈ��ũ �̺�Ʈ�� �˾Ƴ��ϴ�.
			if(nRet == SOCKET_ERROR)
			{
				WLOG("listen socket evt chk err (code:%ld)\n", WSAGetLastError());
				break;
			}

			if(events.lNetworkEvents & FD_ACCEPT) //< ��Ʈ��ũ �̺�Ʈ ������ FD_ACCEPT������ ����
			{
				if(events.iErrorCode[FD_ACCEPT_BIT] == 0) //< ��Ʈ��ũ �̺�Ʈ�� �����ڵ尡 0������ ����
				{
					//< ACCEPT....
					WLOG("listen socket accept check succ\n");
					NotifyAccept();
				}else
				{
					WLOG("listen socket accept chk err (code:%ld)\n", WSAGetLastError());
					break;
				}
			}
		}
	}
	catch(CWSocketException& ex)
	{
		SECSERVER->SetEvent(); // ���� �����忡�� ���α׷� ���������ϰԲ� ��.
		WLOG("server listen socket thread exit(exception) ex:%d\n", ex.GetErrorCode());
	}
	
	//WEMS_SHM->shm_SetProcConn(WS_SERVICE->GetOwnProc()->stProcName.iCopy, WS_SERVICE->GetOwnProc()->stProcName.szProcName, FALSE);
	WLOG("server listen socket thread exit\n");
	return 0;
}



bool SecServerSock::NotifyAccept(void)
{
	char*			strIPAddr;
	sockaddr_in		clientAddr; //< Ŭ���̾�Ʈ Addr
	SOCKET			clientSock; //< Ŭ���̾�Ʈ ����
	int				nErr = 0;

	try
	{
		listen_socket_.Accept(clientSock, clientAddr); //< ���н� ���� �߻�
		WLOG("socket notifyaccect succ (cli_sock:%d)\n", clientSock);
		// ACCEPT ����

 		wemsGPN_st* ownProc = SECSERVER->GetOwnProc();
		SecClientSock* client = new SecClientSock(clientSock, *(ushort*)&ownProc->stNodeName, ownProc->stProcName.iCopy, ownProc->stProcName.szProcName);
		client->ClientInit(PacketDataProc);
// 		wemsclientsock* client = new wemsclientsock(clientSock, *(ushort*)&OwnProc->stNodeName, OwnProc->stProcName.iCopy, OwnProc->stProcName.szProcName); // �������� �����Ǵ� ....

		client->Activate();

		// Ŭ���̾�Ʈ ���� �߰�
		SECSERVER->AddClient(client);
	}
	catch(CWSocketException& ex)
	{
		WLOG("socket notifyaccept fail (code:%ld)\n", ex.GetErrorCode());
	}

	return true;
}