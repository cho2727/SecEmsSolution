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
	WSAEVENT			sockEvent;			//< SOCKET EVENT(이벤트 객체)
	WSANETWORKEVENTS	events;				//< 소켓의 네트워크 이벤트 정보를 저장하기 우한 구조체
	char				strAddr[128];
	UINT				nPort;

	try
	{
		if(server_port_ == 0 || serverAccept_ == NULL)
			return -1;

		memset(strAddr, 0x00, 128);

		listen_socket_.Create();
		// 소켓 레벨 (SOL_SOCKET:소켓 레벨에서의 처리, IPPROTO_TCP:프로토콜 레벨에서의 처리)
		listen_socket_.SetSockOpt(SO_REUSEADDR, (char*)&nOptValue, sizeof(nOptValue), SOL_SOCKET); //< 소켓 옵션 지정
		sockEvent = WSACreateEvent(); //< 이벤트 객체를 만듭니다.
		if(sockEvent != WSA_INVALID_EVENT)
		{//정상
			nRet = WSAEventSelect(SOCKET(listen_socket_), sockEvent, FD_ACCEPT); //< 이벤트 객체를 통해 네트워크 이벤트를 감지합니다.
			if(nRet != SOCKET_ERROR)
			{
				listen_socket_.Bind(server_port_); // 임시로 포트 지정(추후 인자값으로 받아야함)
				listen_socket_.Listen();
				listen_socket_.GetSockName(strAddr, nPort);

				WLOG("server socket listen(%s:%d) suc\n", strAddr, nPort);
			}
		}

		while(true)
		{
			dwRet = ::WaitForSingleObject( shutdown_event_.GetEvent(), m_dwThreadWait );
			if( dwRet  == WAIT_OBJECT_0 ) //이벤트 발생시
			{
				WLOG("server listen socket thread exit(event)\n");
				break;
			}

			//< 리슨 소켓 검사.
			dwRet = WSAWaitForMultipleEvents(1, &sockEvent, FALSE, 100, FALSE); //< 이벤트 객체의 신호 감시
			if(dwRet == WSA_WAIT_TIMEOUT)
				continue;

			nRet = WSAEnumNetworkEvents(SOCKET(listen_socket_), sockEvent, &events); //< 구체적인 네트워크 이벤트를 알아냅니다.
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
		// SECSERVER->SetEvent(); // 메인 쓰레드에게 프로그램 정상종료하게끔 함.
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
		listen_socket_.Accept(clientSock, clientAddr); //< 실패시 예외 발생
		WLOG("socket notifyaccect succ (cli_sock:%d)\n", clientSock);
		// ACCEPT 성공

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