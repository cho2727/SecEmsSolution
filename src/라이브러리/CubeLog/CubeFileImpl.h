// CubeFileImpl.h: interface for the CubeFileImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__CUBE_FILE_IMPL_H__)
#define __CUBE_FILE_IMPL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CubeLogImpl.h"
#include "CubeLocker.h"

class CubeFileImpl : public CubeLogImpl  
{
public:
	typedef		CubeLogImpl	Parent;

public:
	CubeFileImpl( );
	CubeFileImpl( const wchar_t* file_name , unsigned int daily_update = 1 );
	virtual ~CubeFileImpl();

public:
	virtual void	    write_log		( const char* log_data  )  ;
	virtual void	    write_log		( const wchar_t* log_data  )  ;
protected:
	void				make_stream		( std::wofstream& file_stream );
	void				make_path		( const wchar_t* file_path ) ;
	void				make_stream		( std::ofstream&  file_name );
	void				divert_filepath ( const std::wstring&  file_path ); 
	void				file_name		( std::wstring& time_ ) ; 
	void				file_rename		( std::wstring& time_ );
	void				day_time		( std::string& time_ ) ;
	void				day_time		( std::wstring& time_ ) ;
	void				log_rename_save	(void);
	void				log_file_delete(int day_);
	void				set_module_name();

private:
	std::wstring		file_name_       ; //< 현재 시간에 근거한 파일 이름
	unsigned int		daily_update_    ;
	CubeLocker			locker_		     ;
	int					file_check_count_;
	int					file_time_		;
	char				szModuleName_[32]		;
};

#endif // !defined(__CUBE_FILE_IMPL_H__)
