// CubeLogImpl.cpp: implementation of the CubeLogImpl class.
//
//////////////////////////////////////////////////////////////////////

#include <Windows.h>
#include "CubeLogImpl.h"
#if 0		// get_process_name을 사용안할려구(나중에 할지 모름(2015-12-04)
#include <psapi.h>
#pragma comment(lib,"psapi.lib")
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CubeLogImpl::CubeLogImpl()
	:process_name_("")
{
}


CubeLogImpl::~CubeLogImpl()
{

}

#if 0 // get_process_name을 사용안할려구(나중에 할지 모름(2015-12-04)
void		
	CubeLogImpl::get_process_name ( unsigned long pID , std::string&	process_name )
{

	if ( process_name_ != "" )
	{
		process_name.assign( process_name_ ) ;
		return;
	}

	char szProcessName[MAX_PATH] = "Unknown";//TEXT("Unknown");
	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, pID );
	if ( hProcess )
	{

		HMODULE hMod ;
		unsigned long cbNeeded;
		if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
		{
			GetModuleBaseNameA( hProcess , hMod , szProcessName , sizeof(szProcessName) );
		}
	}
	CloseHandle( hProcess );
	process_name.assign( szProcessName );
	process_name_.assign( szProcessName );
}
#endif