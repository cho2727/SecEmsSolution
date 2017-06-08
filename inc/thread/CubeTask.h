#pragma once
// #include "CubeThread.h"
// #include "CubeBoxThread.h"

#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif


#if 0
class WELLS_Export CubeTask : public CubeThread
{
public:
	CubeTask(void){}
	virtual ~CubeTask(void){}

	virtual void	Activate() = 0;
	virtual void	Abort() = 0;
};


class WELLS_Export CubeBoxTask : public CubeBoxThread
{
public:
	CubeBoxTask(void){}
	virtual ~CubeBoxTask(void){}

	virtual void	Activate() = 0;
	virtual void	Abort() = 0;
};
#else

class WELLS_Export CubeTask// : public CubeThread
{
public:
	CubeTask(void){}
	virtual ~CubeTask(void){}

	virtual void	Activate() = 0;
	virtual void	Abort() = 0;
};


#endif
