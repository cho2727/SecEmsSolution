#pragma once

#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif

#include <Windows.h>

class WELLS_Export CubeServiceManager
{
public:
	CubeServiceManager( LPTSTR szServiceName, LPTSTR szDisplay );
	~CubeServiceManager(void);

	void Install(	DWORD dwType		= SERVICE_WIN32_OWN_PROCESS,
		DWORD dwStart		= SERVICE_DEMAND_START,
		LPCTSTR lpDepends	= NULL, 
		LPCTSTR lpName		= NULL,
		LPCTSTR lpPassword	= NULL
		);

	void Remove( BOOL bForce = FALSE );

	BOOL IsInstalled( );

private :
	TCHAR	m_szServiceName[256];
	TCHAR	m_szDisplayName[256];

	BOOL	ServiceControl( DWORD dwControl );
public:
	BOOL Stop(void);
	BOOL Start(void);
	BOOL Pause(void);
	BOOL Continue(void);
	BOOL GetCurrentStat(DWORD &dwState);
};

