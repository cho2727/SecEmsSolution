// CubeFileImpl.cpp: implementation of the CubeFileImpl class.
//
//////////////////////////////////////////////////////////////////////

#include <Windows.h>
#include "CubeFileImpl.h"
#include <fstream>
#include <direct.h>
#include <time.h>
#include <sys/timeb.h>
#include <io.h>
#include "CubeGuard.h"

#define MAX_LOG_FILE_SIZE  1024 * 1024 * 5
#define MAX_FILE_CHECK_COUNT		1000

#define LOG_DELETE_CYCLE		7

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
///! need boost regular exp
CubeFileImpl::CubeFileImpl ( )
	:file_name_( L"C:\\log\\logx.log" ) , daily_update_ ( 1 ), file_check_count_(0), file_time_(0)
{
	make_path	( L"C:\\log\\logx.log" ) ;
}

CubeFileImpl::CubeFileImpl( const wchar_t* file_name , unsigned int daily_update )
	:file_name_ ( file_name ) , daily_update_ ( daily_update ), file_check_count_(0), file_time_(0)
{
	make_path	( file_name_.c_str () );
	set_module_name();
}

CubeFileImpl::~CubeFileImpl()
{	
}

void				CubeFileImpl::set_module_name()
{
	int iStartIndex = file_name_.rfind(L"\\");
	int iLastIndex = file_name_.rfind(L".");

	std::wstring strFileName;	
	if(iStartIndex > 0 && iLastIndex > 0)
	{
		iStartIndex += 1;
		strFileName = file_name_.substr(iStartIndex, iLastIndex - iStartIndex);
		
		int strSize = WideCharToMultiByte(CP_ACP, 0,strFileName.c_str(),-1, NULL, 0,NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, strFileName.c_str(), -1, szModuleName_, strSize, 0,0);
	}
}

void	    CubeFileImpl::write_log		( const wchar_t* log_data  )
{
	CubeGuard	guard	( &locker_ )	;
	std::wofstream	file_stream			;	
	make_stream		( file_stream )		;	
	std::wstring	time_string			;
	day_time		( time_string )		;
	time_string.append( L"\t" )			;
	time_string.append( log_data )		;
	//time_string.append( L"\n" )			;
	try
	{
		file_stream.write( time_string.c_str() , time_string.length() );	
		file_stream.close();
	}
	catch (...) 
	{
		file_stream.close();
	}
}

void	    
	CubeFileImpl::write_log ( const char* log_data  )  
{
	CubeGuard	guard	( &locker_ )	;
	std::ofstream	file_stream			;	
	make_stream		( file_stream )		;	
	std::string		time_string			;
	day_time		( time_string )		;
	time_string.append( "\t" )			;
	time_string.append( log_data )		;
	//time_string.append( "\n" )			;
	try
	{
		file_stream.write( time_string.c_str() , time_string.length() );		
		file_stream.close();		
	}
	catch (...) 
	{
		file_stream.close();
	}
}


void				
	CubeFileImpl::day_time (std::string& time_string )
{
#if 0
	time_t	current_time = 0 ;
	time(&current_time)		 ;
	tm*     time_struct		 =		localtime( &current_time )   ;
	char	buffer[512]		 ;
	wsprintfA( buffer , "%04d-%02d-%02d %02d:%02d:%02d" , time_struct->tm_year+1900 , time_struct->tm_mon + 1 , time_struct->tm_mday , time_struct->tm_hour , time_struct->tm_min , time_struct->tm_sec );
	time_string.assign( buffer );
#else
	int millisec;
	struct timeb timebuffer;
	time_t	current_time = 0 ;
	ftime(&timebuffer);
	current_time = timebuffer.time;
	millisec = timebuffer.millitm;
	tm*     time_struct		 =		localtime( &current_time )   ;
	char	buffer[512]		 ;
	wsprintfA( buffer , "%04d-%02d-%02d %02d:%02d:%02d.%03d" , time_struct->tm_year+1900 , time_struct->tm_mon + 1 , time_struct->tm_mday , time_struct->tm_hour , time_struct->tm_min , time_struct->tm_sec,  millisec);
	time_string.assign( buffer );
#endif
	
}

void CubeFileImpl::day_time		( std::wstring& time_string )
{
#if 0
	time_t	current_time = 0 ;
	time(&current_time)		 ;
	tm*     time_struct		 =		localtime( &current_time )   ;	
	wchar_t	buffer[512]		 ;
	wsprintfW( buffer , L"%04d-%02d-%02d %02d:%02d:%02d" , time_struct->tm_year+1900 , time_struct->tm_mon + 1 , time_struct->tm_mday , time_struct->tm_hour , time_struct->tm_min , time_struct->tm_sec );
	time_string.assign( buffer );
#else
	int millisec;
	struct timeb timebuffer;
	time_t	current_time = 0 ;
	ftime(&timebuffer);
	current_time = timebuffer.time;
	millisec = timebuffer.millitm;
	tm*     time_struct		 =		localtime( &current_time )   ;
	wchar_t	buffer[512]		 ;
	wsprintfW( buffer , L"%04d-%02d-%02d %02d:%02d:%02d.%03d" , time_struct->tm_year+1900 , time_struct->tm_mon + 1 , time_struct->tm_mday , time_struct->tm_hour , time_struct->tm_min , time_struct->tm_sec, millisec );
	time_string.assign( buffer );
#endif
}



void	
	CubeFileImpl::make_path ( const wchar_t* file_path ) 
{
	std::wstring path_string;
	//std::wstring substr , lost_path;
	if ( file_path == 0 )
		return ;
	int iLastIndex = 0;
	path_string.assign( file_path );
	size_t index = 0 ;
	for (;;)
	{
		index = path_string.find( L"\\" , index ) ;
		if ( index == -1 )
		{
			std::wstring substr = path_string.substr ( 0 , index   );
			if ( -1 == substr.find(L"." , index ) )
			{
				return;
			}
			
			_wmkdir( substr.c_str() );
			break;
		}
		index++;
		std::wstring substr = path_string.substr ( 0 , index    );
		_wmkdir( substr.c_str() );
		iLastIndex = index;
	}
}


void				
	CubeFileImpl::make_stream		( std::ofstream& file_stream )
{
	std::wstring afile_name;
	if ( daily_update_ )
	{
		file_name ( afile_name );
		if(file_check_count_++ > MAX_FILE_CHECK_COUNT)
		{
			file_check_count_ = 0;
			std::ifstream in_stream(afile_name, std::ifstream::ate | std::ifstream::binary);
			if(in_stream.tellg() > MAX_LOG_FILE_SIZE)
			{
				in_stream.close();
				std::wstring bfile_name;
				file_rename(bfile_name);
				::MoveFile(afile_name.c_str(), bfile_name.c_str());
			}
			else
			{
				in_stream.close();
			}
		}

		file_stream.open ( afile_name.c_str() , std::ios::out |std::ios_base::app);
	}
	else
	{
		file_stream.open ( afile_name.c_str() , std::ios::out |std::ios_base::app);
	}
}

void				
CubeFileImpl::make_stream		( std::wofstream& file_stream )
{
	std::wstring afile_name;
	if ( daily_update_ )
	{
		file_name ( afile_name );
		if(file_check_count_++ > MAX_FILE_CHECK_COUNT)
		{
			file_check_count_ = 0;
			std::ifstream in_stream(afile_name, std::ifstream::ate | std::ifstream::binary);
			if(in_stream.tellg() > MAX_LOG_FILE_SIZE)
			{
				in_stream.close();
				std::wstring bfile_name;
				file_rename(bfile_name);
				::MoveFile(afile_name.c_str(), bfile_name.c_str());
			}
			else
			{
				in_stream.close();
			}
		}
		file_stream.open ( afile_name.c_str() , std::ios::out |std::ios_base::app);		
	}
	else
	{
		file_stream.open ( afile_name.c_str() , std::ios::out |std::ios_base::app);
	}
}


void
CubeFileImpl :: file_name ( std::wstring& time_ )
{
	wchar_t	  buffer	 [1024]		;
	memset	( buffer, 0x00, 1024 )  ;
	time_t	current_time  = 0		;	
	time    ( &current_time )		;
	tm*		 time_struct		=	 localtime( &current_time ) ;	
	wsprintfW ( buffer , L"_%04d%02d%02d" , time_struct->tm_year+1900 , time_struct->tm_mon + 1 , time_struct->tm_mday ) ;
	int ndate = (time_struct->tm_year+1900)*10000 + (time_struct->tm_mon + 1) * 100 + time_struct->tm_mday;
	if(file_time_ != ndate)
	{
		file_time_ = ndate;
		log_file_delete(LOG_DELETE_CYCLE);	// 로그보간 주기는 7일로 설정함.
	}

	int nindex = 0 ;
	int nLastIndex = 0 ;
	while ( -1 != ( nindex = file_name_.find( L"." ,nindex + 1 ) ) )
	{
		nLastIndex = nindex;
		time_.assign( file_name_ );
	}
	time_.insert( nLastIndex , buffer );
}

void				
CubeFileImpl :: file_rename		( std::wstring& time_ )
{
	wchar_t	  buffer	 [1024]		;
	memset	( buffer, 0x00, 1024 )  ;
	time_t	current_time  = 0		;	
	time    ( &current_time )		;
	tm*		 time_struct		=	 localtime( &current_time ) ;	
	wsprintfW ( buffer , L"_%04d%02d%02d_%02d%02d%02d" , time_struct->tm_year+1900 , time_struct->tm_mon + 1 , time_struct->tm_mday
		, time_struct->tm_hour, time_struct->tm_min, time_struct->tm_sec) ;
	int nindex = 0 ;
	int nLastIndex = 0 ;
	while ( -1 != ( nindex = file_name_.find( L"." ,nindex + 1 ) ) )
	{
		nLastIndex = nindex;
		time_.assign( file_name_ );
	}

	time_.insert( nLastIndex , buffer );
}

void				
CubeFileImpl :: log_file_delete(int day_)
{
	char szFullPattern[1024];
	char szDirve[256],szDir[256],szFileName[256];
	char	szPathName[256];
	char programpath[1024];
	GetModuleFileNameA(0,programpath,1024);
	_splitpath(programpath,szDirve,szDir,szFileName,NULL);

	sprintf(szPathName, "%s%slog", szDirve, szDir);
	sprintf(szFullPattern, "%s\\*.log", szPathName);

	struct _finddata_t c_file;
	intptr_t hFile = _findfirst(szFullPattern, &c_file);
	if(hFile == -1) return;

	do 
	{
		if(c_file.attrib & _A_SUBDIR)
		{
			// directory 처리
		}
		else if(c_file.attrib & _A_HIDDEN)
		{
			// hidden file 처리
		}
		else
		{
			// 로그 파일이 자신의 것이고....
			if(strstr(c_file.name, szModuleName_) != NULL)
			{
				time_t l_time = time(NULL);
				int diff_day = (l_time - c_file.time_create)/(60*60*24);
				if(diff_day > day_)
				{
					// 파일 삭제
					char file_name[_MAX_PATH];
					sprintf(file_name, "%s\\%s", szPathName, c_file.name);
					remove(file_name);
				}
			}
		}
	} while(_findnext(hFile, &c_file)==0);
	_findclose(hFile);
}