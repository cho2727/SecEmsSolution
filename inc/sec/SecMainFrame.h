#ifndef _SEC_MAINFRAME_H_
#define _SEC_MAINFRAME_H_

#define SEC_MAIN_DEFAULT_FUNC()	\
	LONG CALLBACK UnhandledExceptionHandler(EXCEPTION_POINTERS *lpExceptionInfo)  \
	{	\
		DWORD dwProcID=0, dwThreadID=0;	\
		dwProcID   = GetCurrentProcessId();	\
		dwThreadID = GetCurrentThreadId();	\
		if(lpExceptionInfo && lpExceptionInfo->ExceptionRecord)	\
		{	\
			WLOG("*******************************************************************\n" );	\
			WLOG("[EXECPTION] PID=%d, TID=%d, ExceptCode    = 0x%08x ]>>\n", dwProcID, dwThreadID, lpExceptionInfo->ExceptionRecord->ExceptionCode    );	\
			WLOG("[EXECPTION] PID=%d, TID=%d, ExceptAddress = 0x%08x ]>>\n", dwProcID, dwThreadID, lpExceptionInfo->ExceptionRecord->ExceptionAddress );	\
			WLOG("*******************************************************************\n" );	\
		}	\
		return EXCEPTION_EXECUTE_HANDLER;	\
	}	\
	\
	static BOOL WINAPI console_ctrl_handler(DWORD dwCtrlType)	\
	{	\
		switch (dwCtrlType)	\
		{	\
		case CTRL_C_EVENT: \
			SECSERVER->SetEvent();	\
			break;	\
		case CTRL_BREAK_EVENT:	\
			SECSERVER->SetEvent();	\
			break;	\
		case CTRL_CLOSE_EVENT:	\
			break;	\
		case CTRL_LOGOFF_EVENT:	\
			break;	\
		case CTRL_SHUTDOWN_EVENT:	\
			break;	\
		}	\
		return TRUE;	\
	}	\



#define		SEC_MAIN_DEFAULT_CALL()	\
	SetConsoleCtrlHandler(console_ctrl_handler, TRUE);	\
	SetUnhandledExceptionFilter(UnhandledExceptionHandler);


#endif