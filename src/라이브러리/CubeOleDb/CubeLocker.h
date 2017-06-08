#pragma once
#include <Windows.h>
class CubeLocker  
{
public:
	CubeLocker();
	virtual ~CubeLocker() ;


public:
	void lock	( ) ;
	void unlock ( ) ;

private:
	mutable CRITICAL_SECTION	lock_;

	void		EnterMutex();
	void		LeaveMutex();
};
