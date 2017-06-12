#pragma once
#include "thread/CubeBoxThread.h"
#include "thread/ManualEvent.h"
class SecControlWorker : public CubeBoxThread
{
public:
	SecControlWorker(void);
	~SecControlWorker(void);

	void Activate();
	void Abort();

protected:
	virtual int Run();

private:
	CManualEvent		EventExit_;
};

