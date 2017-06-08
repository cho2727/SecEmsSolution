#include "StdAfx.h"
#include "service/CubeNTService.h"


char * _T2A( LPTSTR tStr )
{
	static char szText[1024];

	memset( szText, 0x00, sizeof( szText ) );
	WideCharToMultiByte( CP_ACP, 0, tStr, 1024, szText, 1024, NULL, NULL );

	return szText;
}

CubeNTService::CubeNTService(LPCTSTR szName, LPCTSTR szDisplay, DWORD dwType)
{
	m_dwType = dwType;
	_tcscpy( m_szName, szName );
	_tcscpy( m_szDisplay, szDisplay );

	m_hServiceStatus		= NULL;
	m_dwRequestedControl	= 0;

	m_hWatcherThread = NULL;

	m_dwState			= 0;
	m_dwWaitHint			= 0;
	m_dwCheckpoint		= 0;
	m_dwControlsAccepted	= 0;

	// �̺�Ʈ �ʱ�ȭ
	for(int i = 0 ; i < SVC_NUMEVENTS ; i++)
		m_hEvents[i] = NULL;

	InitializeCriticalSection( &m_cs );
}

CubeNTService::~CubeNTService(void)
{
	DeleteCriticalSection( &m_cs );
}

// ���� ���� �Լ�.
void CubeNTService::ServiceMainFunction( DWORD argc, LPTSTR* argv, LPHANDLER_FUNCTION lpHandlerProc, LPTHREAD_START_ROUTINE lpStartAddress )
{
	DWORD	dwError = 0;
	bool	bRet;

//#ifndef _DEBUG
#if 0
	__try
	{
#endif
 		m_dwArgc = argc;
 		for( DWORD dw = 1; dw < argc; dw++ )
 		{
 			m_arrArgv.push_back( argv[dw] );
 		}

		// ���� �ʱ�ȭ �۾� ����
		bRet = PreInit( );
		if( bRet == false )
			goto SERVICE_STOP_STEP ;

		// ���� ���� �ڵ鷯 ���
		bRet = SetupServiceCtrlHandler( lpHandlerProc );
		if( bRet == false )
			goto SERVICE_STOP_STEP ;

		// �̺�Ʈ ó�� ������ ����
		ExcuteWatcherThread( lpStartAddress ) ;
		if ( m_hWatcherThread == NULL )
			goto SERVICE_STOP_STEP ;

		// �ʱ�ȭ ����
		bRet = Init( );
		if( bRet == false )
			goto SERVICE_STOP_STEP ;

		Run( );
//#ifndef _DEBUG
#if 0
	}
	__except( dwError = GetExceptionCode(), EXCEPTION_EXECUTE_HANDLER )
	{
		if ( m_hServiceStatus )
			SetStatus( SERVICE_STOPPED, 0, 0, 0, dwError, 0 );
	}
#endif

SERVICE_STOP_STEP :
	Release();
	SetStatus( SERVICE_STOPPED, 0, 0, 0, dwError, 0 );
}

// ���� ���� �ڵ鷯 ��� �Լ�.
bool CubeNTService::SetupServiceCtrlHandler( LPHANDLER_FUNCTION lpHandlerProc )
{
	m_hServiceStatus = ::RegisterServiceCtrlHandler( m_szName, lpHandlerProc );
	if ( !m_hServiceStatus )
	{
		//stdout("!!! %s : Failed to register Service Control Handler.", _T2A( m_szName ) );

		return false;
	}

	SetStatus( SERVICE_START_PENDING, 1, 5000 );

	return true;
}

void CubeNTService::ExcuteWatcherThread( LPTHREAD_START_ROUTINE lpStartAddress )
{
	DWORD tid = 0;

	m_hWatcherThread = (HANDLE)_beginthreadex(	0, 
		0, 
		(unsigned (WINAPI*)(void*))lpStartAddress, 
		0, 
		0, 
		(unsigned int*)&tid );
	if ( !m_hWatcherThread )
	{

	}
}

DWORD CubeNTService::WatcherThreadProc( LPVOID lParam )
{
	DWORD	dwWait = 0;
	bool	bControlWait = true;

	while ( bControlWait )
	{
		dwWait = WaitForMultipleObjects( SVC_NUMEVENTS, m_hEvents, FALSE, INFINITE );
		switch( dwWait - WAIT_OBJECT_0 )
		{
		case SVC_STOP :
			OnStop();

			bControlWait = false;
			break;
		case SVC_PAUSE :
			OnPause();

			ResetEvent(m_hEvents[ SVC_PAUSE ]);
			break;
		case SVC_CONTINUE :
			OnContinue();

			ResetEvent(m_hEvents[ SVC_CONTINUE ]);
			break;
		case SVC_SHUTDOWN :
			OnShutDown();

			bControlWait = false;
			break;
		}
	}

	return 0;
}

// ���� �޽��� �ڵ鷯 �Լ�.
void CubeNTService::ServiceHandler( DWORD dwControl )
{
	if ( m_dwRequestedControl == dwControl )
		return;

	switch ( dwControl )
	{
		// ���� ���� �޽���
	case SERVICE_CONTROL_STOP :
		//DMSLOG( LOG_INFO, "CService::Handler > Received SERVICE_CONTROL_STOP message.");
		m_dwRequestedControl = dwControl;

		SetEvent( m_hEvents[ SVC_STOP ] );
		break;
		// ���� �Ͻ� ����
	case SERVICE_CONTROL_PAUSE :
		//DMSLOG( LOG_INFO, "CService::Handler > Received SERVICE_CONTROL_PAUSE message.");
		m_dwRequestedControl = dwControl;

		SetEvent( m_hEvents[ SVC_PAUSE ] );
		break;
		// ���� ��� 
	case SERVICE_CONTROL_CONTINUE :
		//DMSLOG( LOG_INFO, "CService::Handler > Received SERVICE_CONTROL_CONTINUE message.");
		// ���񽺰� �������� ��쿡�� ó��.
		if ( GetStatus() == SERVICE_RUNNING )
		{
			m_dwRequestedControl = dwControl;
			SetEvent( m_hEvents[ SVC_CONTINUE ] );
		}
		break;
		// �������� �ý��� ����
	case SERVICE_CONTROL_SHUTDOWN :
		//DMSLOG( LOG_INFO, "CService::Handler > Received SERVICE_CONTROL_SHUTDOWN message.");
		m_dwRequestedControl = dwControl;

		SetEvent( m_hEvents[ SVC_SHUTDOWN ] );
		break;
	case SERVICE_CONTROL_INTERROGATE :
		SetStatus( GetStatus() );
		break;
	default :
		m_dwRequestedControl = dwControl;
		break;
	}
}

// ���� ���� ���� �Լ�.
void CubeNTService::SetStatus(	DWORD dwNewState, DWORD dwNewCheckpoint /*= STATE_NO_CHANGE*/,
	DWORD dwNewHint /*= STATE_NO_CHANGE*/, DWORD dwNewControls /*= STATE_NO_CHANGE*/,
	DWORD dwExitCode /*= NO_ERROR*/, DWORD dwSpecificExit  /*= 0*/)
{
	if(dwNewState != SERVICE_STOPPED)
	{
		dwExitCode		= S_OK;
		dwSpecificExit	= 0;
	}

	if( dwNewState == SERVICE_STOPPED || dwNewState == SERVICE_PAUSED || dwNewState == SERVICE_RUNNING )
	{
		dwNewHint		= 0;
		dwNewCheckpoint	= 0;
	}
	else
	{
		if ( ( dwNewHint <= 0 ) || ( dwNewCheckpoint <= 0 ) )
		{
			// ERROR ó��
		}
	}

	EnterCriticalSection( &m_cs );

	m_dwState = dwNewState;

	if ( dwNewCheckpoint != STATE_NO_CHANGE )
		m_dwCheckpoint = dwNewCheckpoint;

	if ( dwNewHint != STATE_NO_CHANGE )
		m_dwWaitHint  = dwNewHint;

	if ( dwNewControls != STATE_NO_CHANGE )
		m_dwControlsAccepted = dwNewControls;

	SERVICE_STATUS ss = {
		m_dwType, 
		m_dwState, 
		m_dwControlsAccepted, 
		dwExitCode, 
		dwSpecificExit, 
		m_dwCheckpoint, 
		m_dwWaitHint 
	};

	LeaveCriticalSection( &m_cs );

	if ( !SetServiceStatus( m_hServiceStatus, &ss ) )
	{
	}
}

bool CubeNTService::PreInit()
{
	int	idx;

	for( idx = 0; idx < SVC_NUMEVENTS; idx++ )
	{
		m_hEvents[ idx ] = CreateEvent(NULL, FALSE, FALSE, NULL);

		// �̺�Ʈ ��ü�� �������� ���� ���.
		if ( !m_hEvents[ idx ] )
		{
			//DMSLOG( LOG_ERROR, "Failed to create Event Object[%d]", idx );

			return false;
		}
		else
		{
			//DMSLOG( LOG_INFO, "Succeeded in creating Event Object[%d]", idx );
		}
	}

	//DMSLOG( LOG_INFO, "CService :: Completed the preprocessing." );

	return true;
}

bool CubeNTService::Init()
{
	return true;
}

void CubeNTService::Release()
{
	int	idx;

	// ������ ����.
	if ( m_hWatcherThread )
	{
		//WaitForSingleObject( m_hWatcherThread, 10000 );
		WaitForSingleObject( m_hWatcherThread, 1000 );
		CloseHandle( m_hWatcherThread );
	}

	for( idx = 0; idx < SVC_NUMEVENTS; idx++ )
	{
		if ( m_hEvents[ idx ] )
		{
			CloseHandle( m_hEvents[ idx ] );
			m_hEvents[ idx ] = NULL ;
		}
	}
}
