#include "StdAfx.h"
#include "SecServer.h"
#include "SecDataProc.h"
#include "sec/SecMessageCode.h"
#include "sec/SecMultiSock.h"
#include "process/SecProcManager.h"


int __stdcall PacketDataProc(wemsGPN_st* stSrcProc, wemsGPN_st* stDestProc, wemsDataPacket_st* pRcvData)
{
	SecDataProc::GetInstance()->PacketDataProc(stSrcProc, stDestProc, pRcvData);
	return 0;
}


SecServer::SecServer(void)
{
	service_handle_ = ::CreateEvent( 0, TRUE, FALSE, 0 );
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

	iRet = SecConfig::GetInstance()->ConfigLoad(szConfigFile);
	if(iRet != SEC_OK)
	{
		WLOG("Config Load Fail(ret:%d)\n", iRet);
		return iRet;
	}

	iRet = SecConfig::GetInstance()->ManagerProcLoad();
	if(iRet != SEC_OK)
	{
		WLOG("Program Load Fail(ret:%d)\n", iRet);
		return iRet;
	}


	
	return iRet;
}

int			SecServer::ServiceRun()
{
	int iRet = 0;
	DWORD dwWait;
	int		iData = 1;

	SecProcMana::SecProcManager::GetInstance()->ProcStart();
	while( (dwWait=::WaitForSingleObject(service_handle_, /*INFINITE*/MainThreadWait)) != WAIT_OBJECT_0 ) 
	{
		if(dwWait == WAIT_TIMEOUT)
		{
			// 실제 구현부

			SecProcMana::SecProcManager::GetInstance()->ProcCheck();
		}
	}

	SecProcMana::SecProcManager::GetInstance()->ProcStop();
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
