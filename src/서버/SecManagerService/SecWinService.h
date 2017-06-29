#pragma once
#include "service/CubeNTService.h"

class SecWinService : public CubeNTService
{
public:
	SecWinService(PTSTR szName, LPTSTR szDisplay, DWORD dwType = SERVICE_WIN32_OWN_PROCESS);
	~SecWinService(void);

	virtual void Run();

	DECLARE_SERVICE( SecWinService, SecMainService );


protected :
	virtual void OnStop();
	virtual bool PreInit();
	virtual bool Init();
	virtual void Release();

protected :
	BOOL	m_bStop;
};
