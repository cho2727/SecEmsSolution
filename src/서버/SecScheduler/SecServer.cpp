#include "StdAfx.h"
#include "SecServer.h"
#include "SecDataProc.h"
#include "sec/SecMessageCode.h"
#include "sec/SecMultiSock.h"


int __stdcall PacketDataProc(wemsGPN_st* stSrcProc, wemsGPN_st* stDestProc, wemsDataPacket_st* pRcvData)
{
	SecDataProc::GetInstance()->PacketDataProc(stSrcProc, stDestProc, pRcvData);
	return 0;
}


SecServer::SecServer(void)
	: multi_sock_( )
{
	service_handle_  = ::CreateEvent( 0, TRUE, FALSE, 0 );
	memset(&ownProc_, 0x00, sizeof(wemsGPN_st));
}


SecServer::~SecServer(void)
{
}

SecServer*	SecServer::GetInstance( void )  
{
	static SecServer	_Instance;
	return &_Instance;
}


int			SecServer::ServiceInit(int nCopyNo, char* szConfigFile)
{
	int iRet = 0;
	WLOG("%s Program Start\n", OWNPROCNAME);
	SetOwnProc(0, nCopyNo, OWNPROCNAME);

	SecConfig::GetInstance()->ConfigLoad(szConfigFile);
	
	multi_sock_.Init(*(ushort*)&this->ownProc_.stNodeName
		, this->ownProc_.stProcName.iCopy
		, this->ownProc_.stProcName.szProcName
		, SecConfig::GetInstance()->ServerAddr()
		, SecConfig::GetInstance()->RealPort()
		, SecConfig::GetInstance()->ControlPort()
		, SecConfig::GetInstance()->MessagePort()
		, PacketDataProc);
	return iRet;
}

int			SecServer::ServiceRun()
{
	int iRet = 0;
	DWORD dwWait;
	int		iData = 1;
	while( (dwWait=::WaitForSingleObject(service_handle_, /*INFINITE*/MainThreadWait)) != WAIT_OBJECT_0 ) 
	{
		if(dwWait == WAIT_TIMEOUT)
		{
			// 실제 구현부
			multi_sock_.SockConnectCheck();		// 주기적으로 채크를 수행
			multi_sock_.SendRealData(*(ushort*)&this->ownProc_.stNodeName, this->ownProc_.stProcName.iCopy, this->ownProc_.stProcName.szProcName, FC_PROC_TEST_REQS, 1, (char*)&(iData), sizeof(int));
			multi_sock_.SendControlData(*(ushort*)&this->ownProc_.stNodeName, this->ownProc_.stProcName.iCopy, this->ownProc_.stProcName.szProcName, FC_PROC_TEST_REQS, 1, (char*)&(iData), sizeof(int));
			multi_sock_.SendMessageData(*(ushort*)&this->ownProc_.stNodeName, this->ownProc_.stProcName.iCopy, this->ownProc_.stProcName.szProcName, FC_PROC_TEST_REQS, 1, (char*)&(iData), sizeof(int));
			if(iData++ > 100000)
			{
				iData = 1;
			}
		}
	}


	multi_sock_.Uninit();
	return iRet;
}
void		SecServer::ServiceDestroy()
{
	::CloseHandle(service_handle_);
}

void		SecServer::SetEvent()
{
	::SetEvent(service_handle_);
}


void SecServer::SetOwnProc(ushort usNodeCode, ushort usCopyNo, char* szProcName)
{
	this->ownProc_.stNodeName = *(wemsNodeName_st*)&usNodeCode;
	this->ownProc_.stProcName.iCopy = usCopyNo;
	strcpy_s(this->ownProc_.stProcName.szProcName, szProcName);
}
