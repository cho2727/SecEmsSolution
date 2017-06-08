// Thread.cpp: implementation of the CThread class.
//
//////////////////////////////////////////////////////////////////////

#include "thread/CubeThread.h"
#include <process.h>


// #ifdef _DEBUG
// #undef THIS_FILE
// static char THIS_FILE[]=__FILE__;
// #define new DEBUG_NEW
// #endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CubeThread::CubeThread()
	:  m_hThread(INVALID_HANDLE_VALUE)
{
	//meg_box_ = new CubeBox<CubeData>(CUBEBOXSIZE);
}

CubeThread::~CubeThread()
{
	// 	if(!msg_box_)
	// 		delete meg_box_;

	if (m_hThread != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_hThread);
	}

	//lint -e{1540} m_hThread neither freed nor zeroed in destructor
}

HANDLE CubeThread::GetHandle() const
{
	return m_hThread;
}

void CubeThread::Start()
{
	m_hThread = INVALID_HANDLE_VALUE;

	if (m_hThread == INVALID_HANDLE_VALUE)
	{
		unsigned int threadID = 0;

		m_hThread = (HANDLE)::_beginthreadex(0, 0, ThreadFunction, (void*)this, 0, &threadID);
	}
}

DWORD CubeThread::Wait() const
{
	return Wait(INFINITE);
}

DWORD CubeThread::Wait(DWORD timeoutMillis) const
{
	DWORD dwRet = 0;
	dwRet = ::WaitForSingleObject(m_hThread, timeoutMillis);

	return dwRet;
}

unsigned int __stdcall CubeThread::ThreadFunction(void *pV)
{
	int result = 0;

	CubeThread* pThis = (CubeThread*)pV;

	if (pThis)
	{
		//      try
		//      {
		result = pThis->Run();
		//      }
		//      catch(...)
		//      {
		//      }
	}

	return result;
}

void CubeThread::Terminate(
	DWORD exitCode /* = 0 */)
{
	if (!::TerminateThread(m_hThread, exitCode))
	{
		// TODO we could throw an exception here...
	}
}

void CubeThread::FreeHandle()
{
	if (m_hThread != INVALID_HANDLE_VALUE)
	{
		::CloseHandle( m_hThread );
		m_hThread = INVALID_HANDLE_VALUE;
	}
}

