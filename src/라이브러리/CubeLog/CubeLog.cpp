#include <Windows.h>
#include "log/CubeLog.h"
#include "CubeLocker.h"
#include "CubeGuard.h"
#include "CubeFileImpl.h"
#include <stdio.h>
#include <atlconv.h>
#include <AtlBase.h>

CubeLog*			CubeLog::ploager_ = 0 ;
unsigned int		CubeLog::guarder_ = 1 ;

CubeLog::CubeLog()
{ 
	wchar_t slmpath[1024];
	wchar_t szDirve[256],szDir[256],szFileName[256];
	wchar_t programpath[1024];       
	GetModuleFileNameW(0,programpath,1024);

	_wsplitpath(programpath,szDirve,szDir,szFileName,NULL);

	wsprintfW(slmpath,L"%s%slog\\%s.log",szDirve,szDir, szFileName );

	plogImpl_ = new CubeFileImpl( slmpath ,1 )			;
	pthreading_model_	=	new CubeLocker						;
	ndelete_impl_ = 1											;
	unsigned long	 pid	    =	  ::GetCurrentProcessId( )	;
#if 0
	std::string		 process_name;
	plogImpl_->get_process_name( pid , process_name );
	strcpy ( process_name_ , process_name.c_str() );
#endif
}


CubeLog::CubeLog		(wchar_t * szLogFileName)
{
	wchar_t slmpath[1024];
	wchar_t szDirve[256],szDir[256],szFileName[256];
	wchar_t programpath[1024];       
	GetModuleFileNameW(0,programpath,1024);

	_wsplitpath(programpath,szDirve,szDir,szFileName,NULL);

	wsprintfW(slmpath,L"%s%s\\log\\%s.log",szDirve,szDir, szLogFileName );

	plogImpl_ = new CubeFileImpl( slmpath ,1 )			;
	pthreading_model_	=	new CubeLocker						;
	ndelete_impl_ = 1											;
	unsigned long	 pid	    =	  ::GetCurrentProcessId( )	;
}

CubeLog::~CubeLog()
{
	if ( ndelete_impl_ && plogImpl_ )
		delete plogImpl_;
	delete pthreading_model_;
}

CubeLog*		
	CubeLog::instance( void )  
{
	if ( CubeLog::ploager_ == 0 && CubeLog::guarder_  )
	{
		CubeLog::guarder_ = 0 ;
		CubeLog::ploager_ = new CubeLog;
		atexit( &(CubeLog::exit_instance) );
	}
	else if ( CubeLog::guarder_ == 0 && CubeLog::ploager_ == 0 )
	{
		while ( CubeLog::ploager_ == 0 ){}
	}
	return CubeLog::ploager_;
}

void			
	CubeLog::exit_instance ( ) 
{
	if ( CubeLog::ploager_ != 0 )
	{
		delete CubeLog::ploager_;
		CubeLog::ploager_ = 0;
	}
}


void	 
	CubeLog::implemention	( CubeLogImpl*  new_impl )
{
	if ( ndelete_impl_ )
		if ( plogImpl_ != 0 )
			delete plogImpl_ ;
	ndelete_impl_ = 0 ;
	plogImpl_ = new_impl ;
}

void
	CubeLog::write_log ( const char* src_msg_ , ... ) 
{
	//#if defined (W32LOG_ON) 
	char user_buf [512];
	va_list ap;
	va_start (  ap , src_msg_ ) ;
	vsprintf ( user_buf , src_msg_ , (va_list)ap )	;
	va_end ( ap );
	char			 log_message[4086]; 	
	unsigned long	 tid        =     ::GetCurrentThreadId ( )   ;
	unsigned long	 pid	    =	  ::GetCurrentProcessId( ) ;
	//wsprintfA  ( log_message , "%s%x %d %s %s" , "0x" , tid , pid , process_name_ , user_buf ) ;
	wsprintfA  ( log_message , "0x%x %d %s" , tid , pid , user_buf ) ;
	try
	{
		plogImpl_->write_log( log_message );
	}
	catch(...)
	{

	}
	//#endif
}



void	 CubeLog::hexa(const unsigned char* pBuf,int size)
{
	char buf[32];
	std::string strFullLogging;
	for ( int i = 0 ; i < size ; i++)
	{
		sprintf(buf,"[%x]",pBuf[i]);
		strFullLogging+=buf;
	}

	plogImpl_->write_log(strFullLogging.c_str());
}



void
	CubeLog::write_log ( const wchar_t* src_msg_ , ... ) 
{
	wchar_t buf[512];
	if(wcslen(src_msg_) > 512)
		return;

	va_list arg;
	va_start (  arg , src_msg_ ) ;
	vswprintf(buf, src_msg_, arg );
	va_end ( arg );
	wchar_t			 log_message[4096]; 	
	unsigned long	 tid        =     ::GetCurrentThreadId ( )   ;
	unsigned long	 pid	    =	  ::GetCurrentProcessId( ) ;
	//wsprintfW  ( log_message , L"%s%x %d %S %s" , L"0x" , tid , pid , process_name_ , buf ) ;
	wsprintfW  ( log_message , L"0x%x %d %s", tid, pid, buf ) ;
	try
	{
		plogImpl_->write_log( log_message );
	}
	catch(...)
	{

	}
}

void
	CubeLog::write_debuger ( const char* src_msg_ , ... ) 
{ 
	char user_buf [512];
	va_list ap;
	va_start (  ap , src_msg_ ) ;
	vsprintf_s ( user_buf , src_msg_ , (va_list)ap )	;
	va_end ( ap );

	char			 log_message[1024]; 	
	char			 file_name  [256 ];
	unsigned long	 tid			=     ::GetCurrentThreadId ( )   ;
	unsigned long	errorline		=	 __LINE__					 ;
	strcpy_s ( file_name , __FILE__ )									 ;
	sprintf  ( log_message , "%s%x %s error:%d  %s" , "0x", tid , file_name , errorline ,  user_buf ) ;
	plogImpl_->write_log( log_message );
}

void 
	CubeLog::write_tracer	( const char* src_msg_ , ... ) 
{
	plogImpl_->write_log( src_msg_ );
}

void	 
	CubeLog::file_path	( const char*		file_path )
{
	//	plogImpl_->file_path (	);
}
