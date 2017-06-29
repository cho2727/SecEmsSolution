// SecManagerService.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "SecServer.h"
#include "SecWinService.h"
#include "Options.h"

#ifdef _DEBUG
#pragma comment(lib, "CubeLogd.lib")
#pragma comment(lib, "CubeThreadd.lib")
//#pragma comment(lib, "SecClientSocketd.lib")
//#pragma comment(lib, "SecServerSocketd.lib")
#pragma comment(lib, "SecBaseConfigd.lib")
#pragma comment(lib, "CubeBaseStringd.lib")
#pragma comment(lib, "CubeServiced.lib")
#pragma comment(lib, "SecProcessd.lib")
#else
#pragma comment(lib, "CubeLog.lib")
#pragma comment(lib, "CubeThread.lib")
//#pragma comment(lib, "SecClientSocket.lib")
//#pragma comment(lib, "SecServerSocket.lib")
#pragma comment(lib, "SecBaseConfig.lib")
#pragma comment(lib, "CubeBaseString.lib")
#pragma comment(lib, "CubeService.lib")
#pragma comment(lib, "SecProcess.lib")
#endif


LONG CALLBACK UnhandledExceptionHandler(EXCEPTION_POINTERS *lpExceptionInfo)  
{  
	DWORD dwProcID=0, dwThreadID=0;

	dwProcID   = GetCurrentProcessId();
	dwThreadID = GetCurrentThreadId(); 
	if(lpExceptionInfo && lpExceptionInfo->ExceptionRecord)
	{
		WLOG("*******************************************************************\n" );
		WLOG("[EXECPTION] PID=%d, TID=%d, ExceptCode    = 0x%08x ]>>\n", dwProcID, dwThreadID, lpExceptionInfo->ExceptionRecord->ExceptionCode    );
		WLOG("[EXECPTION] PID=%d, TID=%d, ExceptAddress = 0x%08x ]>>\n", dwProcID, dwThreadID, lpExceptionInfo->ExceptionRecord->ExceptionAddress );		
		WLOG("*******************************************************************\n" );
	}
	return EXCEPTION_EXECUTE_HANDLER;
} 

static BOOL WINAPI console_ctrl_handler(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
	case CTRL_C_EVENT: // Ctrl+C
		SECSERVER->SetEvent(); // 메인 쓰레드에게 프로그램 정상종료하게끔 함.
		break;
	case CTRL_BREAK_EVENT: // Ctrl+Break
		SECSERVER->SetEvent(); // 메인 쓰레드에게 프로그램 정상종료하게끔 함.
		break;
	case CTRL_CLOSE_EVENT: // Closing the console window
		break;
	case CTRL_LOGOFF_EVENT: // User logs off. Passed only to services!
		break;
	case CTRL_SHUTDOWN_EVENT: // System is shutting down. Passed only to services!
		break;
	}

	// Return TRUE if handled this message, further handler functions won't be called.
	// Return FALSE to pass this message to further handlers until default handler calls ExitProcess().
	return TRUE;
}


int main(int argc, char* argv[])
{
	int iRet = 0;

	SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
	SetUnhandledExceptionFilter(UnhandledExceptionHandler); // 예외발생 이벤트 핸들러 등록

	iRet = Options::GetInstance()->run(argc, argv);
	if(iRet != 0)
	{
		SecWinService service( SERVICE_NAME, SERVICE_SHOW_NAME );

		BEGIN_SERVICE_MAP
			SERVICE_MAP_ENTRY( SecWinService, SecMainService )
		END_SERVICE_MAP

		SECSERVER->ServiceDestroy();
	}
	WLOG("%s Program Stop\n", OWNPROCNAME);

	return iRet;
}
