#ifndef __CUBE_LOG_H__
#define __CUBE_LOG_H__

#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif

class CubeLogImpl;
class CubeLocker ;

class WELLS_Export CubeLog 
{
public :
	virtual ~CubeLog		( );
	CubeLog		(wchar_t * szLogFileName);

public :
	static CubeLog*		instance( void )  ;
	static void			exit_instance ( ) ;
private : 


public :
	void	 write_log			( const char*		src_msg_ , ... ) ;	
	void	 write_debuger		( const char*		src_msg_ , ... ) ;
	void	 write_tracer		( const char*		src_msg_ , ... ) ;
	void	 file_path		( const char*		file_path ) ;
	void	 hexa(const unsigned char* pBuf,int size);

	void	 write_log			( const wchar_t*	src_msg_ , ... ) ;
	void	 write_debuger		( const wchar_t*		src_msg_ , ... ) ;
	void	 write_tracer		( const wchar_t*		src_msg_ , ... ) ;
	void	 file_path		( const wchar_t*		file_path ) ;

	void	 implemention	( CubeLogImpl *		new_impl  ) ;

protected :
	CubeLog			();
	CubeLog			(const CubeLog*	pw32);

protected : 
	static CubeLog*				ploager_ ;
	static unsigned int			guarder_ ;
	CubeLogImpl*				plogImpl_ ;
	CubeLocker*					pthreading_model_ ;
	unsigned int				ndelete_impl_	 ;
	char						host_name_ [256]  ;
	char						process_name_[256];
};

#endif	// __CUBE_LOG_H__

#define			WLOG(fmt, ...)			CubeLog::instance()->write_log(fmt, __VA_ARGS__)
#define			DLOG(fmt, ...)			CubeLog::instance()->write_debuger(fmt, __VA_ARGS__)
#define			TLOG(fmt, ...)			CubeLog::instance()->write_tracer(fmt, __VA_ARGS__)