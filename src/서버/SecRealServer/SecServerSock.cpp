#include "StdAfx.h"
#include "SecServerSock.h"
//#include "wemsservice.h"
//#include "wemsclientsock.h"
#include "SecServer.h"
#include "Sec/SecClientSock.h"

#define m_dwThreadWait			5
SecServerSock::SecServerSock(void)
{
}


SecServerSock::~SecServerSock(void)
{
}



int		SecServerSock::Init()
{
	// 소켓 초기화 과정을 수행
	return 0;
}

void SecServerSock::Activate()
{
	m_shutdownEvent.Reset();
	
	Start();
}

void SecServerSock::Abort()
{
	m_shutdownEvent.Set();
	Wait();
}

int SecServerSock::Run()
{
	int				nRet = 0;
	DWORD			dwRet = 0x00;
	int				nCount = 0;
	int					nOptValue      = 0; //< SOCKET OPT
	WSAEVENT			sockEvent;			//< SOCKET EVENT(이벤트 객체)
	WSANETWORKEVENTS	events;				//< 소켓의 네트워크 이벤트 정보를 저장하기 우한 구조체
	char				strAddr[128];
	UINT				nPort;

	try
	{
		memset(strAddr, 0x00, 128);

		m_listenerSocket.Create();
		// 소켓 레벨 (SOL_SOCKET:소켓 레벨에서의 처리, IPPROTO_TCP:프로토콜 레벨에서의 처리)
		m_listenerSocket.SetSockOpt(SO_REUSEADDR, (char*)&nOptValue, sizeof(nOptValue), SOL_SOCKET); //< 소켓 옵션 지정
		sockEvent = WSACreateEvent(); //< 이벤트 객체를 만듭니다.
		if(sockEvent != WSA_INVALID_EVENT)
		{//정상
			nRet = WSAEventSelect(SOCKET(m_listenerSocket), sockEvent, FD_ACCEPT); //< 이벤트 객체를 통해 네트워크 이벤트를 감지합니다.
			if(nRet != SOCKET_ERROR)
			{
// 				wemsNodeName_st	OwnNodeName = WS_SERVICE->GetOwnProc()->stNodeName;
// 				wemsNodeInfo_st* OwnNodeInfo = (wemsNodeInfo_st*)WEMS_SHM->shm_GetPhysicalNodeInfo(OwnNodeName.eNodeType, OwnNodeName.eNodeRank, OwnNodeName.eNodeGroup);
// 				
// 				if(OwnNodeInfo == NULL)
// 					return -1;

				m_listenerSocket.Bind(39701); // 임시로 포트 지정(추후 인자값으로 받아야함)
				m_listenerSocket.Listen();
				m_listenerSocket.GetSockName(strAddr, nPort);

				WLOG("server socket listen(%s:%d) suc\n", strAddr, nPort);
				// WEMS_SHM->shm_SetProcConn(WS_SERVICE->GetOwnProc()->stProcName.iCopy, WS_SERVICE->GetOwnProc()->stProcName.szProcName, TRUE);
			}
		}

		while(true)
		{
			dwRet = ::WaitForSingleObject( m_shutdownEvent.GetEvent(), m_dwThreadWait );
			if( dwRet  == WAIT_OBJECT_0 ) //이벤트 발생시
			{
				WLOG("server listen socket thread exit(event)\n");
				break;
			}

			//< 리슨 소켓 검사.
			dwRet = WSAWaitForMultipleEvents(1, &sockEvent, FALSE, 100, FALSE); //< 이벤트 객체의 신호 감시
			if(dwRet == WSA_WAIT_TIMEOUT)
				continue;

			nRet = WSAEnumNetworkEvents(SOCKET(m_listenerSocket), sockEvent, &events); //< 구체적인 네트워크 이벤트를 알아냅니다.
			if(nRet == SOCKET_ERROR)
			{
				WLOG("listen socket evt chk err (code:%ld)\n", WSAGetLastError());
				break;
			}

			if(events.lNetworkEvents & FD_ACCEPT) //< 네트워크 이벤트 정보가 FD_ACCEPT인지를 조사
			{
				if(events.iErrorCode[FD_ACCEPT_BIT] == 0) //< 네트워크 이벤트의 에러코드가 0인지를 조사
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
		SECSERVER->SetEvent(); // 메인 쓰레드에게 프로그램 정상종료하게끔 함.
		WLOG("server listen socket thread exit(exception) ex:%d\n", ex.GetErrorCode());
	}
	
	//WEMS_SHM->shm_SetProcConn(WS_SERVICE->GetOwnProc()->stProcName.iCopy, WS_SERVICE->GetOwnProc()->stProcName.szProcName, FALSE);
	WLOG("server listen socket thread exit\n");
	return 0;
}



bool SecServerSock::NotifyAccept(void)
{
	char*			strIPAddr;
	sockaddr_in		clientAddr; //< 클라이언트 Addr
	SOCKET			clientSock; //< 클라이언트 소켓
	int				nErr = 0;

	try
	{
		m_listenerSocket.Accept(clientSock, clientAddr); //< 실패시 예외 발생
		WLOG("socket notifyaccect succ (cli_sock:%d)\n", clientSock);
		// ACCEPT 성공

 		wemsGPN_st* ownProc = SECSERVER->GetOwnProc();
		SecClientSock* client = new SecClientSock(clientSock, *(ushort*)&ownProc->stNodeName, ownProc->stProcName.iCopy, ownProc->stProcName.szProcName);

// 		wemsclientsock* client = new wemsclientsock(clientSock, *(ushort*)&OwnProc->stNodeName, OwnProc->stProcName.iCopy, OwnProc->stProcName.szProcName); // 서버측에 생성되는 ....

//		client->Activate();

		// 클라이언트 감시 추가
		//WS_SERVICE->AddClient(client);
	}
	catch(CWSocketException& ex)
	{
		WLOG("socket notifyaccept fail (code:%ld)\n", ex.GetErrorCode());
	}

	return true;
}