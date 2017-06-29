#include "StdAfx.h"
#include "SecServer.h"
#include "sec/SecMessageCode.h"
#include "sec/SecMultiSock.h"
#include "process/SecProcManager.h"

SecServer::SecServer(void)
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

	// 초기 서버 기본 프로세스 등록1
 	SecProcMana::SecProcManager::GetInstance()->AddProgram(SEC_REAL_PROC, 0, true, "");
	SecProcMana::SecProcManager::GetInstance()->AddProgram(SEC_CON_PROC, 0, true, "");
 	SecProcMana::SecProcManager::GetInstance()->AddProgram(SEC_MSG_PROC, 0, true, "");
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
			SecProcMana::SecProcManager::GetInstance()->ProcCheck();
			WLOG("%s Program Run...\n", this->ownProc_.stProcName.szProcName);
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
