#pragma once

#include <string>
#include <vector>
using namespace std;

#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif

#define SERVOCE_NAME_SZ		64
#define SVC_NUMEVENTS		4
#define STATE_NO_CHANGE		0xFFFFFFFF

class WELLS_Export CubeNTService
{
public:
	CubeNTService(LPCTSTR szName, LPCTSTR szDisplay, DWORD dwType);
	~CubeNTService(void);

	DWORD   GetStatus()      { return m_dwState; }
	DWORD   GetControls()    { return m_dwControlsAccepted; }
	LPCTSTR GetName()        { return m_szName; }
	LPCTSTR GetDisplayName() { return m_szDisplay; }

	// 서비스 메인 함수.
	void ServiceMainFunction(	DWORD argc, 
		LPTSTR* argv,
		LPHANDLER_FUNCTION lpHandlerProc, 
		LPTHREAD_START_ROUTINE lpStartAddress );
	// 서비스 제어 처리 함수.
	void ServiceHandler( DWORD dwControl );

protected :
	bool SetupServiceCtrlHandler( LPHANDLER_FUNCTION lpHandlerProc );
	virtual void ExcuteWatcherThread( LPTHREAD_START_ROUTINE lpStartAddress );
	virtual DWORD WatcherThreadProc( LPVOID lParam );

	void SetStatus(DWORD dwNewState,
		DWORD dwNewCheckpoint = STATE_NO_CHANGE,
		DWORD dwNewHint       = STATE_NO_CHANGE,
		DWORD dwNewControls   = STATE_NO_CHANGE,
		DWORD dwExitCode      = NO_ERROR,
		DWORD dwSpecificExit  = 0);


	// Override Function
protected :
	virtual bool PreInit();
	virtual bool Init();
	virtual void Release();

	virtual void OnPause()			{};
	virtual void OnContinue()		{};
	virtual void OnShutDown()		{};
	virtual void OnStop()			{};
	virtual void Run()				{};
	virtual void ParseComandLine( DWORD argc, LPTSTR* argv )	{};

protected :
	CRITICAL_SECTION		m_cs;

	// Service Information
	SERVICE_STATUS_HANDLE	m_hServiceStatus;
	DWORD					m_dwState;
	DWORD					m_dwControlsAccepted;
	DWORD					m_dwCheckpoint;
	DWORD					m_dwWaitHint;

	// Tracks state currently being worked on in Handler
	DWORD					m_dwRequestedControl;

	// Control Events
	HANDLE					m_hEvents[SVC_NUMEVENTS];
	HANDLE					m_hWatcherThread;

	// 서비스 이름
	TCHAR					m_szName[ SERVOCE_NAME_SZ + 1 ];
	// 서비스 표시 이름
	TCHAR					m_szDisplay[ SERVOCE_NAME_SZ + 1 ];
	// 서비스 형식
	DWORD					m_dwType;

	// 서비스 호출 인자 개수
	DWORD					m_dwArgc;
	vector<wstring>			m_arrArgv;
	enum EVENTS { SVC_STOP, SVC_PAUSE, SVC_CONTINUE, SVC_SHUTDOWN };
};

#define DECLARE_SERVICE(class_name, service_name) \
public: \
	static class_name##* m_pThis; \
	static void WINAPI service_name##Main(DWORD argc, LPTSTR* argv); \
	static void WINAPI service_name##Handler(DWORD dwControl); \
	static DWORD WINAPI service_name##WatcherThreadProc(LPVOID lpParameter);

#define IMPLEMENT_SERVICE(class_name, service_name) \
	class_name##* class_name::m_pThis = NULL; \
	void WINAPI class_name::service_name##Main(DWORD argc, LPTSTR* argv) \
{ \
	m_pThis->ServiceMainFunction(argc, argv, \
	(LPHANDLER_FUNCTION)service_name##Handler, \
	(LPTHREAD_START_ROUTINE)service_name##WatcherThreadProc); \
} \
	void WINAPI class_name::service_name##Handler(DWORD dwControl) \
{ \
	m_pThis->ServiceHandler(dwControl); \
} \
	DWORD WINAPI class_name::service_name##WatcherThreadProc(LPVOID lpParameter) \
{ \
	return m_pThis->WatcherThreadProc( lpParameter ); \
}

// 서비스 목록 구성
#define BEGIN_SERVICE_MAP \
	SERVICE_TABLE_ENTRY svcTable[] = {

#define SERVICE_MAP_ENTRY(class_name, service_name) \
{_T(#service_name), (LPSERVICE_MAIN_FUNCTION)class_name::service_name##Main},

#define END_SERVICE_MAP \
{NULL, NULL}}; \
	StartServiceCtrlDispatcher(svcTable);
