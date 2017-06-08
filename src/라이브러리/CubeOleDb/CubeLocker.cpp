#include <Windows.h>
#include "CubeLocker.h"


CubeLocker::CubeLocker()
{
	InitializeCriticalSection ( &lock_ ) ;
}

CubeLocker::~CubeLocker()
{
	DeleteCriticalSection	  ( &lock_ ) ;
}


void CubeLocker::lock ()	 
{	
	EnterCriticalSection	  ( &lock_ ) ;
}

void CubeLocker::unlock ()
{
	LeaveCriticalSection	  ( &lock_ ) ;
}

void		CubeLocker::EnterMutex()
{

}
void		CubeLocker::LeaveMutex()
{

}
