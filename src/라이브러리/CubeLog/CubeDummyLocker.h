#if !defined(__CUBE_DUMMY_LOCKER_H__)
#define __CUBE_DUMMY_LOCKER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CubeDummyLocker  
{
public:
	CubeDummyLocker();
	virtual ~CubeDummyLocker();

public:
	void lock	( );
	void unlock	( );
};

#endif // !defined(__CUBE_DUMMY_LOCKER_H__)