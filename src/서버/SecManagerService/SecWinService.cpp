#include "StdAfx.h"
#include "SecWinService.h"
#include "SecServer.h"

IMPLEMENT_SERVICE( SecWinService, SecMainService )

	SecWinService::SecWinService( LPTSTR szName, LPTSTR szDisplay, DWORD dwType )
	: CubeNTService( szName, szDisplay, dwType )
{
	m_pThis		= this;
	m_bStop		= FALSE;
}

SecWinService::~SecWinService(void)
{
}

void SecWinService::Run()
{	
	SetStatus(SERVICE_RUNNING, 0, 0, SERVICE_ACCEPT_STOP );
	SECSERVER->ServiceRun();
	SetStatus(SERVICE_STOP_PENDING, 2, 1000);
}

void SecWinService::OnStop()
{	
	//SetStatus( SERVICE_STOP_PENDING, 1, 1000);	// 안해도 되는것으로 추정
	SECSERVER->SetEvent(); // 메인 쓰레드에게 프로그램 정상종료하게끔 함.
	WLOG("[SVC] DSC Service Program Stop\n");
	InterlockedExchange((LPLONG)&m_bStop, (LONG)TRUE);
}

bool SecWinService::PreInit()
{
	bool bRet = CubeNTService::PreInit();
	return bRet;
}

bool SecWinService::Init()	// 서비스 초기화
{
	bool	bRet = true;
	int nArgc = 0;

	bRet = CubeNTService::Init();
	if ( bRet == false )
	{
		WLOG("[SVR] DSCS Cube NT Init Fail(ret:%d)\n", bRet);
		return bRet;
	}

	int iRet = SECSERVER->ServiceInit(0, "");
	if(iRet != SEC_OK)
	{
		bRet = false;
		WLOG("[SVR] DSCS Program Init Fail(ret:%d)\n", iRet);
		Sleep(30);
	}

	return bRet;
}

void SecWinService::Release()
{		
	return CubeNTService::Release();
}
