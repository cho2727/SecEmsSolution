// CubeGuard.h: interface for the CubeGuard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__CUBE_GUARD_H__)
#define __CUBE_GUARD_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CubeLocker;
class CubeGuard  
{
public:
	CubeGuard ( CubeLocker*	lock_type , unsigned int auto_lock = 1)	;
	virtual ~CubeGuard ( ) ;	
public:
	void		aquire ( ) ;
	void		release( ) ;

private:
	CubeLocker*		locker_		;
};

#endif // !defined(__CUBE_GUARD_H__)
