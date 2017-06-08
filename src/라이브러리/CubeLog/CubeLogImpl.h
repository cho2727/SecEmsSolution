// CubeLogImpl.h: interface for the CubeLogImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__CUBE_LOG_IMPL_H__)
#define __CUBE_LOG_IMPL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class CubeLogImpl  
{
public:
	CubeLogImpl ( );
	virtual ~CubeLogImpl();

public:
	virtual void	    write_log		( const wchar_t* log_data  )  =0;
	virtual void write_log			  ( const char* log_data  ) = 0 ;

#if 0 // get_process_name을 사용안할려구(나중에 할지 모름(2015-12-04)
	void		 get_process_name		( unsigned long pID , std::string& process_name );	
#endif

protected:

protected:
	std::string	 process_name_ ;

};

#endif // !defined(__CUBE_LOG_IMPL_H__)
