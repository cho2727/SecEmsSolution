#include "StdAfx.h"
#include "service/CubeNTServiceManager.h"
#include <vector>
#include <string>
#include <sstream>
using namespace std;

int GetNumberOfArgs( const wstring str, wchar_t delimeter)
{
	vector<wstring> _v;

	wstringstream ss(str);
	wstring tok;

	while( getline(ss, tok, delimeter) )
	{
		if ( tok.size() > 0 )
			_v.push_back( tok );
	}

	return _v.size();
}

vector<wstring> SplitArgs( const wstring str, wchar_t delimeter)
{
	vector<wstring> _v;

	wstringstream ss(str);
	wstring tok;

	while( getline(ss, tok, delimeter) )
	{
		if ( tok.size() > 0 )
			_v.push_back( tok );
	}

	return _v;
}

CubeNTServiceManager::CubeNTServiceManager( LPTSTR szServiceName, LPTSTR szDisplay )
{
	_tcscpy( m_szServiceName, szServiceName );
	_tcscpy( m_szDisplayName, szDisplay );
}

CubeNTServiceManager::~CubeNTServiceManager(void)
{
}

BOOL CubeNTServiceManager::Install(DWORD dwType, DWORD dwStart,
	LPCTSTR lpDepends, LPCTSTR lpName,
	LPCTSTR lpPassword )
{
	BOOL		bRet = TRUE;
	SC_HANDLE hSCM = NULL;
	SC_HANDLE hService = NULL;

	if( IsInstalled() == TRUE )
	{
		bRet = FALSE;
		goto cleanup;
	}

	hSCM = OpenSCManager(	NULL,
		NULL,
		SC_MANAGER_CREATE_SERVICE );
	if ( !hSCM )
	{
		//ErrorPrinter( _TEXT("OpenSCManager") );
		bRet = FALSE;

		goto cleanup;
	}

	// 서비스 프로그램 경로 취득
	TCHAR szFilePath[_MAX_PATH];
	::GetModuleFileName(NULL, szFilePath, _MAX_PATH);

	hService = CreateService( 
		hSCM,
		m_szServiceName,
		m_szDisplayName,
		SERVICE_ALL_ACCESS ,
		dwType,
		dwStart,
		SERVICE_ERROR_NORMAL,
		szFilePath,
		NULL,
		NULL,
		lpDepends,
		lpName,
		lpPassword );

	if ( !hService ) 
	{
		//ErrorPrinter( _TEXT("CreateService") );
		bRet = FALSE;
		goto cleanup;
	}
	else
	{
		_tprintf(_TEXT("%s Install\n"), m_szServiceName );
	}

cleanup:
	if ( hService ) 
		CloseServiceHandle( hService );
	if ( hSCM ) 
		CloseServiceHandle( hSCM );

	return bRet;
}

void CubeNTServiceManager::Remove( BOOL bForce )
{
	SC_HANDLE hSCM = NULL;
	SC_HANDLE hService = NULL;
	BOOL bSuccess = FALSE;

	hSCM = ::OpenSCManager(	NULL,
		NULL,
		SC_MANAGER_CONNECT );
	if ( !hSCM )
	{
		//ErrorPrinter( _TEXT("OpenSCManager") );
		return;
	}

	hService = ::OpenService(	hSCM,
		m_szServiceName,
		SERVICE_ALL_ACCESS );						
	if ( !hService ) 
	{
		//ErrorPrinter( _TEXT("OpenService") );
		goto cleanup;
	}

	//Force the service to stop 
	if( bForce == TRUE ) 
	{
		SERVICE_STATUS status;
		::ControlService(hService, SERVICE_CONTROL_STOP, &status);
		_tprintf(_TEXT("%s stopped\n"), m_szServiceName);
	}

	bSuccess = ::DeleteService( hService );

	if (bSuccess)
		_tprintf(_TEXT("%s Remove\n"), m_szServiceName); 
	else
	{
		//ErrorPrinter( _TEXT("DeleteService") );
	}

cleanup:
	if ( hService ) 
		CloseServiceHandle( hService );
	if ( hSCM ) 
		CloseServiceHandle( hSCM );

	return;
}

BOOL CubeNTServiceManager::IsInstalled( )
{
	BOOL bRet = FALSE;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);

	if ( hSCM != NULL )
	{
		SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bRet = TRUE;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}

	return bRet;
}

BOOL CubeNTServiceManager::Start(LPCTSTR lpServiceArgs/* = NULL*/)
{
	SC_HANDLE hSCM		= NULL;
	SC_HANDLE hService	= NULL;
	BOOL bSuccess		= FALSE;

	DWORD dwNumServiceArgs = 0;

	hSCM = ::OpenSCManager(	NULL,
		NULL,
		SC_MANAGER_CONNECT );
	if ( !hSCM )
	{
		//ErrorPrinter( _TEXT("OpenSCManager") );
		return FALSE;
	}

	hService = ::OpenService(	hSCM,
		m_szServiceName,
		SERVICE_ALL_ACCESS );						
	if ( !hService ) 
	{
		//ErrorPrinter( _TEXT("OpenService") );
		goto cleanup;
	}

#if 0
	SERVICE_STATUS status;
	bSuccess = ::StartService( hService, 0, NULL );
	if ( bSuccess )
		bSuccess = TRUE;
#else
	if( lpServiceArgs != NULL )
	{
		//dwNumServiceArgs = (DWORD)GetNumberOfArgs( lpServiceArgs, _T(' '));
		vector<wstring> _v = SplitArgs(lpServiceArgs, _T(' '));
		dwNumServiceArgs = _v.size();

		wchar_t ** ppArg = new wchar_t*[dwNumServiceArgs];
		for( DWORD i = 0; i < dwNumServiceArgs; i++ )
		{
			ppArg[i] = new wchar_t[_v[i].size() + 1];
			_swprintf( ppArg[i], _T("%s"), _v[i].c_str());
		}
		bSuccess = ::StartService( hService, dwNumServiceArgs, (LPCWSTR *)ppArg );

		for( DWORD i = 0; i < dwNumServiceArgs; i++ )
			delete ppArg[i];
		delete[] ppArg;
	}
	else
		bSuccess = ::StartService( hService, dwNumServiceArgs, &lpServiceArgs );
	if ( bSuccess )
		bSuccess = TRUE;

#endif

cleanup:
	if ( hService ) 
		CloseServiceHandle( hService );
	if ( hSCM ) 
		CloseServiceHandle( hSCM );

	return bSuccess;
}

BOOL CubeNTServiceManager::Stop(void)
{
	BOOL bRet;

	bRet = ServiceControl( SERVICE_CONTROL_STOP );
	if ( bRet )
		_tprintf( _T("%s > SERVICE_CONTROL_STOP Succeed."), m_szServiceName );
	else
		_tprintf( _T("%s > SERVICE_CONTROL_STOP Failed."), m_szServiceName );

	return bRet;
}

BOOL CubeNTServiceManager::Pause(void)
{
	BOOL bRet;

	bRet = ServiceControl( SERVICE_CONTROL_PAUSE );
	if ( bRet )
		_tprintf( _T("%s > SERVICE_CONTROL_PAUSE Succeed."), m_szServiceName );
	else
		_tprintf( _T("%s > SERVICE_CONTROL_PAUSE Failed."), m_szServiceName );

	return bRet;
}

BOOL CubeNTServiceManager::Continue(void)
{
	BOOL bRet;

	bRet = ServiceControl( SERVICE_CONTROL_CONTINUE );
	if ( bRet )
		_tprintf( _T("%s > SERVICE_CONTROL_CONTINUE Succeed."), m_szServiceName );
	else
		_tprintf( _T("%s > SERVICE_CONTROL_CONTINUE Failed."), m_szServiceName );

	return bRet;
}

BOOL CubeNTServiceManager::ServiceControl( DWORD dwControl )
{
	SC_HANDLE hSCM		= NULL;
	SC_HANDLE hService	= NULL;
	BOOL bSuccess		= FALSE;

	hSCM = ::OpenSCManager(	NULL,
		NULL,
		SC_MANAGER_CONNECT );
	if ( !hSCM )
	{
		//ErrorPrinter( _TEXT("OpenSCManager") );
		return FALSE;
	}

	hService = ::OpenService(	hSCM,
		m_szServiceName,
		SERVICE_ALL_ACCESS );						
	if ( !hService ) 
	{
		//ErrorPrinter( _TEXT("OpenService") );
		goto cleanup;
	}

	SERVICE_STATUS status;
	bSuccess = ::ControlService( hService, dwControl, &status );
	if ( bSuccess )
		bSuccess = TRUE;

cleanup:
	if ( hService ) 
		CloseServiceHandle( hService );
	if ( hSCM ) 
		CloseServiceHandle( hSCM );

	return bSuccess;
}

BOOL CubeNTServiceManager::GetCurrentStat( DWORD &dwState )
{
	SC_HANDLE		hSCM;
	SC_HANDLE		hService;
	BOOL			bRet = FALSE;
	SERVICE_STATUS	ss;

	hSCM = ::OpenSCManager( NULL, NULL, GENERIC_READ );
	if( hSCM == NULL )
		return FALSE;

	hService = ::OpenService( hSCM, m_szServiceName, SERVICE_QUERY_STATUS );
	if ( hService == NULL )
	{
		::CloseServiceHandle( hSCM );
		return FALSE;
	}

	if ( ::QueryServiceStatus( hService, &ss ) )
	{
		dwState = ss.dwCurrentState;
		bRet = TRUE;
	}

	if ( hService ) 
		::CloseServiceHandle( hService );

	if ( hSCM ) 
		::CloseServiceHandle( hSCM );

	return bRet ;
}