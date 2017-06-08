// CubeGuard.h: interface for the CubeGuard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__CUBE_LOCKER_H__)
#define __CUBE_LOCKER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

#endif // !defined(__CUBE_LOCKER_H__)
