#pragma once
#include "thread/CubeTask.h"

#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif

#include <Windows.h>

class WELLS_Export CubeThread : public CubeTask
{
public :

	CubeThread();

	virtual ~CubeThread();

	HANDLE GetHandle() const;

	DWORD Wait() const;

	DWORD Wait(DWORD timeoutMillis) const;

	void Start();

	void Terminate(
		DWORD exitCode = 0);

	void FreeHandle();

private :

	virtual int Run() = 0;

	static unsigned int __stdcall ThreadFunction(void *pV);

	HANDLE m_hThread;

	// No copies do not implement
	CubeThread(const CubeThread &rhs);
	CubeThread &operator=(const CubeThread &rhs);
};

