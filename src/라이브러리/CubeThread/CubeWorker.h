#pragma once
#include "CubeThread.h"

class WELLS_Export CubeWorker : public CubeThread
{
public:
	CubeWorker(void);
	virtual ~CubeWorker(void);

	virtual void	init() = 0;
	virtual void	close() = 0;
};

