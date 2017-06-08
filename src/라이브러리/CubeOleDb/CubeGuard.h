#pragma once

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