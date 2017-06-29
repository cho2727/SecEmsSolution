// SecScheduler.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "SecServer.h"
#include "sec/SecMainFrame.h"


#ifdef _DEBUG
#pragma comment(lib, "CubeLogd.lib")
#pragma comment(lib, "CubeThreadd.lib")
#pragma comment(lib, "SecClientSocketd.lib")
#pragma comment(lib, "SecBaseConfigd.lib")
#else
#pragma comment(lib, "CubeLog.lib")
#pragma comment(lib, "CubeThread.lib")
#pragma comment(lib, "SecClientSocket.lib")
#pragma comment(lib, "SecBaseConfig.lib")
#endif


SEC_MAIN_DEFAULT_FUNC()

	int main(int argc, char* argv[])
{
	SEC_MAIN_DEFAULT_CALL()

		if(argc < 3)
		{
			WLOG("[main] proc not start(param cnt:%d, ex:proc.exe 0 config.xml)\n", argc);
			return -1;
		}	

		WLOG("Command : %s %s %s\n", argv[0], argv[1], argv[2]);
		if(SECSERVER->ServiceInit(atoi(argv[1]), argv[2]) == SEC_OK)
			SECSERVER->ServiceRun();

		SECSERVER->ServiceDestroy();
		WLOG("%s Program Stop\n", OWNPROCNAME);
		return 0;
}
