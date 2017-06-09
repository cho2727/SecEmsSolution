#include <winsock2.h>
#include "sec/SecServerSock.h"
//#include "Sec/SecClientSock.h"
#include "log/CubeLog.h"


#ifdef _DEBUG
#pragma comment(lib, "CubeLogd.lib")
#pragma comment(lib, "CubeThreadd.lib")
#pragma comment(lib, "CubeSocketd.lib")
#pragma comment(lib, "ws2_32.lib")
#else
#pragma comment(lib, "CubeLog.lib")
#pragma comment(lib, "CubeThread.lib")
#pragma comment(lib, "CubeSocket.lib")
#pragma comment(lib, "ws2_32.lib")
#endif


#define m_dwThreadWait			5
SecServerSock::SecServerSock(void)
	: server_port_(0), serverAccept_(NULL)
{
}

SecServerSock::SecServerSock(int nPort, ServerAccept pFunc)
	:server_port_(nPort), serverAccept_(pFunc)
{

}

SecServerSock::~SecServerSock(void)
{
}

int		SecServerSock::Init(int nPort, ServerAccept pFunc)
{
	server_port_ = nPort;
	serverAccept_ = pFunc;
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
		if(server_port_ == 0 || serverAccept_ == NULL)
			return -1;

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
				listen_socket_.Bind(server_port_); // �ӽ÷� ��Ʈ ����(���� ���ڰ����� �޾ƾ���)
				listen_socket_.Listen();
				listen_socket_.GetSockName(strAddr, nPort);

				WLOG("server socket listen(%s:%d) suc\n", strAddr, nPort);
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
		// SECSERVER->SetEvent(); // ���� �����忡�� ���α׷� ���������ϰԲ� ��.
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

		if(serverAccept_)
		{
			serverAccept_(clientSock);
		}
	}
	catch(CWSocketException& ex)
	{
		WLOG("socket notifyaccept fail (code:%ld)\n", ex.GetErrorCode());
	}

	return true;
}