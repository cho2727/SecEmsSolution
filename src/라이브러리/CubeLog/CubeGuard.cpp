// CubeGuard.cpp: implementation of the CubeGuard class.
//
//////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include "CubeGuard.h"
#include "CubeLocker.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CubeGuard::CubeGuard ( CubeLocker*	lock_type ,  unsigned int auto_lock )
	:locker_ ( lock_type )
{
	if ( auto_lock )
		locker_->lock();
}

CubeGuard::~CubeGuard()
{
	locker_->unlock();
}

void		
	CubeGuard ::aquire ( ) 
{
	locker_->lock();
}

void
	CubeGuard ::release( ) 
{
	locker_->unlock();
}
