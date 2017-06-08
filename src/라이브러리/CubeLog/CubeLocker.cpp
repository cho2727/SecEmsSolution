#include <Windows.h>
#include "CubeLocker.h"

#ifdef _DEBUG
#include "stdio.h"
#endif


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