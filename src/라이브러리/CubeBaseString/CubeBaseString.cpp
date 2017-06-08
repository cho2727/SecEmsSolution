#include "common/CubeBaseString.h"

namespace CubeBase
{
	void	l_split_filename( char *path, char *dir, char *fname );
	char *	l_strchr( char *ptr, char deli );
	void	l_trucate_string_ju( char *str1, char *str2 );
	
	
	
	void l_split_filename( char *path, char *dir, char *fname )
	{
	int i;
	/*
		struct	stat	stbuf;
	*/

	fname[0] = 0;
	if( ( strcmp( path, "."  ) == 0 )
	 || ( strcmp( path, ".." ) == 0 ) )
		{
			strcpy( dir, path );
			return;
		}

	/* for unix
		stat( path, &stbuf );
		if( S_ISDIR(stbuf.st_mode)==1 )
		{
				strcpy( dir, path );
				return;
		}
	*/

		dir[0] = 0;
		for( i=(int)strlen(path)-1 ; i>=0 ; i-- )
		{
			if( path[i]=='/' || path[i]=='\\' )
			{
				strncpy( dir, path, i+1 );
				dir[i+1] = 0;
				strcpy( fname, &path[i+1] );
				return;
			}
		}
		dir[0] = '.';
		strcpy( fname, path );
		return;
	}



	char *
		l_strchr( char *ptr, char deli )
	{
		char	*tmpptr1;
		char	*tmpptr2;
		char	*tmpptr3;

		tmpptr1 = strchr( ptr, deli );
		if( tmpptr1==0 )
			return( (char *)0 );

		for( tmpptr2=ptr ; tmpptr2!=tmpptr1; tmpptr2++ )
		{
			if( *tmpptr2=='"' )
			{
				tmpptr3 = strchr( tmpptr2+1, '"' );
				if( tmpptr3==0 )
					return( (char *)0 );
				memcpy( tmpptr2, tmpptr2+1, tmpptr3-tmpptr2-1 );
				*tmpptr3   = 0;
				*(tmpptr3-1) = 0;
				return( tmpptr3+1 );
			}
		}

		return( tmpptr1 );
	}

	void
		l_trucate_string_ju( char *str1, char *str2 )
	{
		char	*ptr;

		ptr = strstr( str1, str2 );
		if( ptr==0 )
			return;

		if( strlen(ptr)==strlen(str2) )
			str1[ ptr-str1 ] = 0;
		else
			strcpy( str1+(ptr-str1), ptr+strlen(str2) );

		l_trucate_string_ju( str1, str2 );

		return;
	}

// 	void GetModulePath(wchar_t* strModulePath)
// 	{
// 		HMODULE	hModule;
// 		hModule = ::GetModuleHandle( NULL );
// 
// 		wchar_t szExeFileName[MAX_PATH];
// 		// VERIFY( ::GetModuleFileName( hModule ,szExeFileName, _MAX_PATH ) );
// 
// 		DWORD dwRet = GetModuleFileName( hModule ,szExeFileName, _MAX_PATH );
// 		wchar_t Drive[ _MAX_DRIVE ];
// 		wchar_t  Path[ _MAX_PATH ];
// 		wchar_t  FileName[ _MAX_FNAME ];
// 		wchar_t  Ext[ _MAX_EXT ];
// 
// 		_wsplitpath( szExeFileName , Drive , Path , FileName , Ext );
// 		wsprintf(strModulePath, L"%s\\%s", Drive, Path);
// 	}

	void	GetModulePath(char strModulePath[])
	{
		char szDirve[MAX_PATH],szDir[MAX_PATH];
		char szExeFileName[MAX_PATH];
		GetModuleFileNameA(0, szExeFileName, _MAX_PATH);
		_splitpath(szExeFileName, szDirve, szDir, NULL, NULL);

		if(strModulePath != NULL)
		{
			sprintf(strModulePath, "%s%s", szDirve, szDir); // ���� �̸� ����
		}
	}


	void	GetModulePath(wchar_t strModulePath[])
	{
		wchar_t szDirve[MAX_PATH],szDir[MAX_PATH];
		wchar_t szExeFileName[MAX_PATH];

		GetModuleFileName(0, szExeFileName, _MAX_PATH);
		_wsplitpath(szExeFileName, szDirve, szDir, NULL, NULL);

		if(strModulePath != NULL)
		{
			wsprintf(strModulePath, L"%s%s", szDirve, szDir); // ���� �̸� ����
		}
	}

	void	GetModuleFile(char strModuleFile[])
	{
		if(strModuleFile == NULL)
			return;

		char szExeFileName[MAX_PATH];
		GetModuleFileNameA(0, szExeFileName, _MAX_PATH);
		_splitpath(szExeFileName, NULL, NULL, strModuleFile, NULL);
	}

	void	GetModuleFile(wchar_t strModuleFile[])
	{
		if(strModuleFile == NULL)
			return;

		wchar_t szExeFileName[MAX_PATH];
		GetModuleFileName(0, szExeFileName, _MAX_PATH);
		_wsplitpath(szExeFileName, NULL, NULL, strModuleFile, NULL);
	}



	//wchar_t ���� char ���� ����ȯ �Լ�
	char * ConvertWCtoC(wchar_t* str)
	{
		//��ȯ�� char* ���� ����
		char* pStr ; 

		//�Է¹��� wchar_t ������ ���̸� ����
		int strSize = WideCharToMultiByte(CP_ACP, 0,str,-1, NULL, 0,NULL, NULL);

		//char* �޸� �Ҵ�
		pStr = new char[strSize];

		//�� ��ȯ 
		WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0,0);

		return pStr;
	}

	///////////////////////////////////////////////////////////////////////
	//char ���� wchar_t ���� ����ȯ �Լ�
	wchar_t* ConverCtoWC(char* str)
	{
		//wchar_t�� ���� ����
		wchar_t* pStr;
		//��Ƽ ����Ʈ ũ�� ��� ���� ��ȯ
		int strSize = MultiByteToWideChar(CP_ACP, 0,str, -1, NULL, NULL);

		//wchar_t �޸� �Ҵ�
		pStr = new WCHAR[strSize];
		//�� ��ȯ
		MultiByteToWideChar(CP_ACP, 0,str, strlen(str)+1, pStr, strSize);

		return pStr;
	}



	//wchar_t ���� char ���� ����ȯ �Լ�
	char * ConvertT2A(wchar_t* str, char pStr[])
	{
		//��ȯ�� char* ���� ����
		//�Է¹��� wchar_t ������ ���̸� ����
		int strSize = WideCharToMultiByte(CP_ACP, 0,str,-1, NULL, 0,NULL, NULL);

		//�� ��ȯ 
		WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0,0);

		return pStr;
	}

	///////////////////////////////////////////////////////////////////////
	//char ���� wchar_t ���� ����ȯ �Լ�
	wchar_t* ConvertA2T(char* str, wchar_t pStr[])
	{
		//��Ƽ ����Ʈ ũ�� ��� ���� ��ȯ
		int strSize = MultiByteToWideChar(CP_ACP, 0,str, -1, NULL, NULL);

		//�� ��ȯ
		MultiByteToWideChar(CP_ACP, 0,str, strlen(str)+1, pStr, strSize);

		return pStr;
	}



	void Trim(char* str)
	{
		Rtrim(str);
		Ltrim(str);
	}

	void Rtrim(char* str)
	{
		register	unsigned int	i;

		for( i=0 ; i<strlen(str) ; i++ )
		{
			if( str[i] == '#' || str[i] == ';' )
			{
				str[i] = 0;
				break;
			}
		}

		for( i=(int)strlen(str)-1 ; i>=0 ; i-- )
		{
			if( ( str[i] !=  ' ' )
				&& ( str[i] != '\t' )
				&& ( str[i] != '\n' ) )
			{
				str[i+1] = 0;
				break;
			}
		}

	}

	void Ltrim(char* str)
	{
		register	unsigned int	i;
		char		str2[1024];

		strcpy( str2, str );

		for( i=0 ; i<strlen(str2) ; i++ )
		{
			if( str2[i] == '#' || str2[i] == ';' )
			{
				str[0] = 0;
				return;
			}

			if( ( str[i] !=  ' ' )
				&& ( str[i] != '\t' )
				&& ( str[i] != '\n' ) )
				break;
		}

		strcpy( str, str2+i );

		return;
	}

	int Alltrim( char* str, unsigned int len )
	{
		register	int	i;
		int		noofconv = 0;	/* null �� ġȯ�� ������ ���� (return ������ ���)	*/

		if( len <= 0 )
			return( noofconv );	/* 0 (ġȯ ����)	*/

		/* ���� ������ ���ڰ� �����̸� null �� ����			*/
		/* ��Ʈ���� �Ϻθ����� �� �Լ��� ȣ���� ��츦 ���	*/
		if( str[len-1] == ' ' )
		{
			str[len-1] = 0;
			noofconv++;
		}

		for( i=len-2; i>=0 ; i-- )
		{
			/* ���� ���ڰ� �����̰� �ڰ� null �̸� ���� ���ڸ� null �� ġȯ		*/
			if( str[i]==' ' && str[i+1]==0 )
			{
				str[i] = 0;
				noofconv++;
			}
		}

		return( noofconv );	/* null �� ġȯ�� ���ڼ��� return	*/
	}


	char* GetNextFieldData( char *ptr, char deli, char *fdata )
	{
		int		i, j;
		char	*retptr;
		int		isQuoOpen;

		if( ptr == NULL )
		{
			if( fdata != NULL )
				fdata[0] = 0;
			return( NULL );
		}
		if( fdata == NULL )
		{
			return( NULL );
		}

#if	0
		for( i=0 ; ptr[i] && ptr[i]!=deli ; ++i )
		{
			fdata[i] = ptr[i];
		}
		fdata[i] = 0;

		if( ptr[i] == deli )
			retptr = &ptr[i+1];
		else
			retptr = NULL;
#else
		isQuoOpen = 0;		// �ֵ���ǥ ���� ����
		for( i=0,j=0 ; ; ++i )
		{
			if( ptr[i] == NULL )
				break;
			if( isQuoOpen == 0 )
			{
				if( ptr[i] == deli )
					break;
			}
			if( ptr[i] == '"' )
			{
				if( ptr[i+1] == '"' )
				{
					++i;
				}
				else
				{
					if( isQuoOpen == 1 )
						isQuoOpen = 0;
					else
						isQuoOpen = 1;
					continue;
				}
			}
			fdata[j++] = ptr[i];
		}
		fdata[j] = 0;

		if( ptr[i] == deli )
			retptr = &ptr[i+1];
		else
			retptr = NULL;
#endif

		return( retptr );		// return next position
	}

	// ���� �ð��� ���ڿ� ���� 'YYYYMMDDhhmmss' ���·� ����
	char*		GetDatetime2StringA(char str[])
	{
		struct  tm      *ltm;
		time_t          curt;
		struct	_timeb	tp;

		_ftime( &tp );
		curt = time( (time_t *)&tp.time );
		ltm = localtime( &curt );
		sprintf( str, "%04d%02d%02d%02d%02d%02d%03ld",
			ltm->tm_year+1900,
			ltm->tm_mon+1,
			ltm->tm_mday,
			ltm->tm_hour,
			ltm->tm_min,
			ltm->tm_sec,
			tp.millitm );

		return str;
	}

	// ���� �ð��� ���ڿ� ���� 'YYYY-MM-DDThh:mm:ss.xxx' ���·� ����
	char*		GetDatetime2StringB(char str[])
	{
		struct  tm      *ltm;
		time_t          curt;
		struct	_timeb	tp;

		_ftime( &tp );
		curt = time( (time_t *)&tp.time );
		ltm = localtime( &curt );
		sprintf( str, "%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d.%3.3d",
			ltm->tm_year+1900,
			ltm->tm_mon+1,
			ltm->tm_mday,
			ltm->tm_hour,
			ltm->tm_min,
			ltm->tm_sec,
			tp.millitm );

		return str;
	}

	struct tm*		GetCurrentDatetime()
	{
		time_t          curt;
		struct	_timeb	tp;

		_ftime( &tp );
		curt = time( (time_t *)&tp.time );
		return localtime( &curt );
	}

	 //< str�� �������� '20161007' �̰ų� '20161001123007'
	time_t GetString2Time(char* str)
	{
		if(str == NULL)
			return (0);

		if( strlen(str) == 8 ) //< '20161007' �����̸�
		{
			return GetString2TimeA(str);
		}
		else if(strlen(str) == 14)
		{
			return GetString2TimeB(str);
		}

		return (0);
	}

	time_t		GetDateTimeT(short year, short month, short day, short hour, short minute, short second)
	{
		struct  tm      ltm;
		ltm.tm_year = year - 1900;
		ltm.tm_mon  = month;
		ltm.tm_mon--;
		ltm.tm_mday = day;
		ltm.tm_hour = hour;
		ltm.tm_min = minute;
		ltm.tm_sec = second;

		return( mktime(&ltm) );

	}

	// �ش� ���ڿ�(str)�� time_t ���� ����
	time_t		GetString2TimeA(char* str)
	{
		struct  tm      ltm;

		if( str==NULL || strlen(str)<8 )
		{
			return( 0 );
		}

		if( str[0] == '0' || str[0] == ' ' )
			return( 0 );

		ltm.tm_year = (str[0]-'0')*1000+(str[1]-'0')*100+(str[2]-'0')*10+(str[3]-'0') - 1900;
		ltm.tm_mon  = (str[4]-'0')*10+(str[5]-'0');
		ltm.tm_mon--;
		ltm.tm_mday = (str[ 6]-'0')*10+(str[ 7]-'0');
		ltm.tm_hour = 12;
		ltm.tm_min = 0;
		ltm.tm_sec = 0;

		return( mktime(&ltm) );
	}

	// �ش� ���ڿ�(str)�� time_t ���� ����
	time_t		GetString2TimeB(char* str)
	{
		struct  tm      ltm;

		if( str==NULL || strlen(str)<14 )
		{
			return( 0 );
		}

		if( str[0] == '0' || str[0] == ' ' )
			return( 0 );

		ltm.tm_year = (str[0]-'0')*1000+(str[1]-'0')*100+(str[2]-'0')*10+(str[3]-'0') - 1900;
		ltm.tm_mon  = (str[4]-'0')*10+(str[5]-'0');
		ltm.tm_mon--;
		ltm.tm_mday = (str[ 6]-'0')*10+(str[ 7]-'0');
		ltm.tm_hour = (str[ 8]-'0')*10+(str[ 9]-'0');
		ltm.tm_min  = (str[10]-'0')*10+(str[11]-'0');
		ltm.tm_sec  = (str[12]-'0')*10+(str[13]-'0');

		return( mktime(&ltm) );
	}
	// �ش� ��¥(tm)�� �ش��ϴ� 'YYYYMMDDhhmmss' ������ ���ڿ��� ����
	char*		GetDateTime2StringA( time_t tm, char newDataTime[] )
	{
		struct  tm      *ltm;
		time_t          tmptm;

		if( (int)tm == -1 )
		{
			newDataTime[0] = 0;
			return( newDataTime );
		}

		tmptm = tm;
		ltm = localtime( &tmptm );
		sprintf( newDataTime, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d",
			ltm->tm_year+1900,
			ltm->tm_mon+1,
			ltm->tm_mday,
			ltm->tm_hour,
			ltm->tm_min,
			ltm->tm_sec );

		return( newDataTime );
	}

	// �ش� ��¥(tm)�� �ش��ϴ� 'YYYY-MM-DDThh:mm:ss.xxx' ������ ���ڿ��� ����
	char*		GetDateTime2StringB( time_t tm, char newDataTime[] )
	{
		struct  tm      *ltm;
		time_t          tmptm;

		if(newDataTime == NULL)
			return newDataTime;

		if( tm < 0 )
		{
			newDataTime[0] = 0;
			return( newDataTime );
		}

		tmptm = tm;
		ltm = localtime( &tmptm );
		if(ltm == NULL)
		{
			return (newDataTime);
		}

		sprintf( newDataTime, "%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d",
			ltm->tm_year+1900,
			ltm->tm_mon+1,
			ltm->tm_mday,
			ltm->tm_hour,
			ltm->tm_min,
			ltm->tm_sec );

		return( newDataTime );
	}

	// �ش� ��¥(tm)�� addDays�� ��¥�� ���� ���ڿ� �ش��ϴ� 'YYYYMMDDhhmmss'�� ���ڿ��� ����
	char*		GetDateTime2StringA( char* tm, int addDays, char newDataTime[] )
	{
		time_t	ft;

		ft = GetString2Time( tm );
		return GetDateTime2StringA( ft+(addDays*24*60*60), newDataTime );
	}

	// �ش� ��¥(tm)�� addDays�� ��¥�� ���� ���ڿ� �ش��ϴ� 'YYYY-MM-DDThh:mm:ss'�� ���ڿ��� ����
	char*		GetDateTime2StringB( char* tm, int addDays, char newDataTime[] )
	{
		time_t	ft;

		ft = GetString2Time( tm );
		return GetDateTime2StringB( ft+(addDays*24*60*60), newDataTime );
	}


	// ttimestr�� ���ڿ� ������ 'YYYY-MM-DDThh:mm:ss' �̰ų� 'YYYY-MM-DDThh:mm:ss.xxx' �� ����� time_t���� ����
	time_t		GetString2DateTime(char* ttimestr)
	{
		struct  tm  ltm;

		if(ttimestr == NULL)
			return ( 0 );

		if( strlen(ttimestr) != strlen("YYYY-MM-DDThh:mm:ss") && 
			strlen(ttimestr) != strlen("YYYY-MM-DDThh:mm:ss.xxx"))
			return( 0 );

		ltm.tm_year = (ttimestr[0]-'0')*1000+(ttimestr[1]-'0')*100+(ttimestr[2]-'0')*10+(ttimestr[3]-'0') - 1900;
		ltm.tm_mon  = (ttimestr[5]-'0')*10+(ttimestr[ 6]-'0');
		ltm.tm_mon--;
		ltm.tm_mday = (ttimestr[ 8]-'0')*10+(ttimestr[ 9]-'0');
		ltm.tm_hour = (ttimestr[11]-'0')*10+(ttimestr[12]-'0');
		ltm.tm_min  = (ttimestr[14]-'0')*10+(ttimestr[15]-'0');
		ltm.tm_sec  = (ttimestr[17]-'0')*10+(ttimestr[18]-'0');

		return( mktime(&ltm) );
	}


	void	GetArray2String(unsigned char* array, int len, char str[])
	{
		char strTemp[4];
		for(int i=0; i<len; i++)
		{
			sprintf(strTemp, "%02x", array[i]);
			strcat(str, strTemp);
		}
	}


	time_t		GetNextDayTime(short v_hour, short v_min)						// ���� ��.��
	{
		struct  tm *tm;
		time_t  cclock, tclock;

		time(&cclock);

		tm = localtime(&cclock);
		tm->tm_hour = v_hour;
		tm->tm_min = v_min;
		tm->tm_sec = 0;

		tclock = mktime(tm);

		if ( cclock>=tclock ) {
			return tclock + (24 * 60 * 60);
		} else {
			return tclock;
		}
	}
	
	time_t		GetNextDayTime(short v_hour, short v_min, int add_hour)		// ���� ��.��
	{
		struct  tm *tm;
		time_t  cclock, tclock;

		time(&cclock);

		tm = localtime(&cclock);
		tm->tm_hour = v_hour;
		tm->tm_min = v_min;
		tm->tm_sec = 0;

		tclock = mktime(tm);

		if ( cclock>=tclock ) {
			return tclock + (add_hour * 60 * 60);
		} else {
			return tclock;
		}
	}

	time_t	GetNextMonthTime(short v_day, short v_hour, short v_min, int add_month)
	{
		struct  tm *tm;
		time_t  cclock, tclock;

		time(&cclock);

		tm = localtime(&cclock);
		tm->tm_mon = tm->tm_mon + add_month;
		tm->tm_mday = v_day;
		tm->tm_hour = v_hour;
		tm->tm_min = v_min;
		tm->tm_sec = 0;

		if((tm->tm_mon + 1)	> 12)
		{
			tm->tm_year++;
			tm->tm_mon %= 12;
		}
		
		tclock = mktime(tm);

		return tclock;
	}

	time_t		GetStartFixTime(int interval)		// ���� ��.��
	{
		struct  	tm *tm;
		time_t		clock, tclock, aclock;
		char 		stmr[64];

		time(&clock);
		tm = localtime(&clock);

		if ( interval<60 ) {
#if 0
			if ( interval>=tm->tm_sec ) aclock = interval; 
			else aclock = 0;
#else
			aclock = (int)(tm->tm_sec/interval) * interval;
#endif

			tm->tm_sec = 0;
			tclock = mktime(tm) + aclock + interval;
			tm->tm_sec = 0;
			tclock = mktime(tm) + aclock + interval;
		} else if ( interval>=60 && interval<3600 ){
#if 0
			if ( (interval/60)>=tm->tm_min ) aclock = interval;
			else aclock = 0;
#else
			aclock = (int)(tm->tm_min*60/interval) * interval;
#endif
			tm->tm_sec = 0;
			tm->tm_min = 0;
			tclock = mktime(tm) + aclock + interval;
		} else if ( interval>=3600 && interval<86400 ){
#if 0
			if ( (interval/3600)>=tm->tm_hour ) aclock = interval;
			else aclock = 0;
#else
			aclock = (int)(tm->tm_hour*3600/interval) * interval;
#endif

			tm->tm_sec = 0;
			tm->tm_min = 0;
			tm->tm_hour = 0;

			tclock = mktime(tm) + aclock + interval;
		} else {
			tclock = 0;
		}

		return tclock;
	}

	int		GetSecondNextFixMin(int value)		// ���� ��.��
	{
		int interval = 0;
		struct tm *tp;
		time_t clock;

		time(&clock);
		tp = localtime(&clock);

		interval = (value - ((tp->tm_min + value) % value)) * 60 - tp->tm_sec;

		return interval;
	}


	enum _SCHMETHOD
	{
		SM_NONE = 0,
		SM_SEC,
		SM_MIN,
		SM_HOUR,
		SM_DAY,
		SM_WEEK,
		SM_MON
	};

	time_t NextScheduleTime(int method, int period, int wday, int day, int hour, int min, int sec)
	{
		int ph = 0;
		time_t now_time = time(NULL);
		time_t base_time = 0;
		time_t diff_time = 0;
		time_t schedule_time = 0;
		struct tm *base_tm;

		base_tm = localtime(&now_time);
		base_tm->tm_mon = 0;
		base_tm->tm_mday = 1;
		base_tm->tm_hour = 0;
		base_tm->tm_min = 0;
		base_tm->tm_sec = 0;

		base_time = mktime(base_tm);

		if(method == SM_NONE)
			return 0;

		if(method == SM_SEC)
		{
			return now_time + period;
		}

		diff_time = now_time - base_time;

		if(method == SM_MIN)
		{
			ph = period * 60;
			schedule_time = base_time + (diff_time + (ph - (diff_time % ph) + sec));
			if( schedule_time - ph > now_time)
			{
				return schedule_time - ph;
			}

			return schedule_time;
		}
		else if(method == SM_HOUR)
		{
			ph = period * 60 * 60;
			schedule_time = base_time + (diff_time + (ph - (diff_time % ph) + (min*60) + sec));
			if( schedule_time - ph >  now_time)
			{
				return schedule_time - ph;
			}

			return schedule_time;
		}
		else if(method == SM_DAY)		// 
		{
			ph = period * 60 * 60 * 24;

			schedule_time = base_time + (diff_time + (ph - (diff_time % ph) + ((hour * 60 + min)*60) + sec));
			if( schedule_time - ph >  now_time)
			{
				return schedule_time - ph;
			}

			return schedule_time;
		}
		else if(method == SM_WEEK)
		{
			// �ʹ� �����.
		}
		else if(method == SM_MON)
		{
			struct tm *mon_tm;
			mon_tm = localtime(&now_time);
			int		remainMonth = (mon_tm->tm_mon + 1)%period;

			if(remainMonth == 0)
			{
				mon_tm->tm_mday = day; mon_tm->tm_hour = hour; mon_tm->tm_min = min; mon_tm->tm_sec = sec;
				schedule_time = mktime(mon_tm);
				if(schedule_time<= now_time)
				{
					mon_tm->tm_mon += period;
					schedule_time = mktime(mon_tm);
				}
			}
			else
			{
				mon_tm->tm_mon += (period - remainMonth);
				mon_tm->tm_mday = day; mon_tm->tm_hour = hour; mon_tm->tm_min = min; mon_tm->tm_sec = sec;
				schedule_time = mktime(mon_tm);
			}

			return schedule_time;
		}

		return 0;
	}

	/// @brief Registry���� string value �о���� �Լ�
	/// @return Ű�� �������� ������ -1�� ������.
	/// @param[in]  hKey        ���� ���� �����ϴ� Ű, { HKEY_LOCAL_MACHINE, HKEY_CLASSES_ROOT, ...} ���� �� ���.
	/// @param[in]  lpSubKey    ���� ���� �����ϴ� subkey path
	/// @param[in]  lpValueKey  ���� ���� �̸�
	int ReadStringValueInRegistry( HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, TCHAR*	szValue)
	{
		HKEY hSubKey = NULL;
		// open the key
		if ( ::RegOpenKeyEx( hKey, lpSubKey, 0, KEY_READ, &hSubKey ) == ERROR_SUCCESS )
		{
			DWORD buf_size = 0;
			// ���ڿ��� ũ�⸦ ���� �о�´�.
			if ( ::RegQueryValueEx( hSubKey, lpValueKey, NULL, NULL, NULL, &buf_size ) == ERROR_SUCCESS )
			{
				// �޸� �Ҵ��ϰ�...,
				TCHAR *pBuf = new TCHAR [ buf_size + 1 ];

				ZeroMemory(pBuf, sizeof(TCHAR)*buf_size + 1);
				// ���� ���� �о�´�.
				if ( ::RegQueryValueEx( hSubKey, lpValueKey, NULL, NULL, (LPBYTE)pBuf, &buf_size ) == ERROR_SUCCESS )
				{
					pBuf[ buf_size ] = NULL;
					wcscpy(szValue, pBuf);
				}
				// to avoid leakage
				delete [] pBuf;
			}
			// Ű �ݱ�
			::RegCloseKey( hSubKey );
		}
		else
		{
			return -1;
		}
		//return strValue;
		return 0;
	}



	/// @brief Registry���� DWORD value �о���� �Լ�.
	/// @return Ű�� �������� ������ dwDefault �� ������.
	/// @return Ű�� �������� ������ -1�� ������.
	/// @param[in]  hKey        ���� ���� �����ϴ� Ű, { HKEY_LOCAL_MACHINE, HKEY_CLASSES_ROOT, ...} ���� �� ���.
	/// @param[in]  lpSubKey    ���� ���� �����ϴ� subkey path
	/// @param[in]  lpValueKey  ���� ���� �̸�
	DWORD ReadDWORDValueInRegistry( HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, int dwDefault /*= -1*/ )
	{
		DWORD dwValue = (DWORD)dwDefault;
		HKEY hSubKey = NULL;
		if ( ::RegOpenKeyEx( hKey, lpSubKey, 0, KEY_READ, &hSubKey ) == ERROR_SUCCESS )
		{
			DWORD buf_size = sizeof(DWORD); 
			// DWORD �� �о����
			if ( ::RegQueryValueEx( hSubKey, lpValueKey, NULL, NULL, (LPBYTE)&dwValue, &buf_size ) == ERROR_SUCCESS )
			{
				// ����
			}

			// Ű �ݱ�
			::RegCloseKey( hSubKey );
		}

		return dwValue;
	}

	/// @brief Registry���� string value ���� �Լ�
	/// @param[in]  hKey        �� ���� �����ϴ� Ű, { HKEY_LOCAL_MACHINE, HKEY_CLASSES_ROOT, ...} ���� �� ���.
	/// @param[in]  lpSubKey    �� ���� �����ϴ� subkey path
	/// @param[in]  lpValueKey  �� ���� �̸�
	/// @param[in]  lpValue     �� ��
	int WriteStringValueInRegistry( HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, LPCTSTR lpValue )
	{
		HKEY hSubKey = NULL;

		// open the key
		if ( ::RegOpenKeyEx( hKey, lpSubKey, 0, KEY_ALL_ACCESS, &hSubKey ) == ERROR_SUCCESS)
		{
			DWORD cbSize = (DWORD)wcslen(lpValue) * sizeof(TCHAR) + 1;
			BYTE *pBuf = new BYTE [ cbSize ];
			::ZeroMemory( pBuf, cbSize );
			::CopyMemory( pBuf, lpValue, cbSize - 1 );
			//result = RegSetValue(hKey, NULL, REG_SZ, TEXT("�ӽð�"), 0);
			::RegSetValueEx( hSubKey, lpValueKey, NULL, REG_SZ, pBuf, cbSize ); 

			// Ű �ݱ�
			::RegCloseKey( hSubKey );
			delete [] pBuf;
			return 0;
		}

		return -1;
	}

	/// @briefRegistry���� DWORD value ���� �Լ�
	/// @param[in]  hKey        �� ���� �����ϴ� Ű, { HKEY_LOCAL_MACHINE, HKEY_CLASSES_ROOT, ...} ���� �� ���.
	/// @param[in]  lpSubKey    �� ���� �����ϴ� subkey path
	/// @param[in]  lpValueKey  �� ���� �̸�
	/// @param[in]  dwValue     �� ��
	int WriteDWORDValueInRegistry( HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, DWORD dwValue )
	{
		HKEY hSubKey = NULL;

		// open the key
		if ( ::RegOpenKeyEx( hKey, lpSubKey, 0, KEY_ALL_ACCESS, &hSubKey ) == ERROR_SUCCESS)
		{

			::RegSetValueEx( hSubKey, lpValueKey, NULL, REG_DWORD, (LPBYTE)&dwValue, sizeof(DWORD) );
			// Ű �ݱ�
			::RegCloseKey( hSubKey );

			return 0;
		}

		return -1;
	}

	BOOL CreateStringValueInRegistry(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, LPCTSTR lpValue)
	{
		HKEY hSubKey = NULL;

		// open the key
		if ( ::RegOpenKeyEx( hKey, lpSubKey, 0, KEY_ALL_ACCESS, &hSubKey ) != ERROR_SUCCESS )
		{
			if( ::RegCreateKeyEx(hKey, lpSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hSubKey, NULL) != ERROR_SUCCESS )
				return FALSE;
		}

		DWORD cbSize = (DWORD)wcslen(lpValue) * sizeof(TCHAR) + 1;
		BYTE *pBuf = new BYTE [ cbSize ];
		::ZeroMemory( pBuf, cbSize );
		::CopyMemory( pBuf, lpValue, cbSize - 1 );
		//result = RegSetValue(hKey, NULL, REG_SZ, TEXT("�ӽð�"), 0);
		::RegSetValueEx( hSubKey, lpValueKey, NULL, REG_SZ, pBuf, cbSize ); 

		// Ű �ݱ�
		::RegCloseKey( hSubKey );
		delete [] pBuf;

		return TRUE;
	}
	
	BOOL CreateDWORDValueInRegistry(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, DWORD dwValue)
	{
		HKEY hSubKey = NULL;

		// open the key
		if ( ::RegOpenKeyEx( hKey, lpSubKey, 0, KEY_ALL_ACCESS, &hSubKey ) != ERROR_SUCCESS )
		{
			if( ::RegCreateKeyEx(hKey, lpSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hSubKey, NULL) != ERROR_SUCCESS )
				return FALSE;
		}

		RegSetValueEx( hSubKey, lpValueKey, NULL, REG_DWORD, (LPBYTE)&dwValue, sizeof(DWORD) );
		// Ű �ݱ�
		::RegCloseKey( hSubKey );

		return TRUE;
	}

	BOOL DeleteRegistryValue( HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValue )
	{
		HKEY hSubKey = NULL;
		BOOL bRet = FALSE;

		if ( ::RegOpenKeyEx( hKey, lpSubKey, 0, KEY_ALL_ACCESS, &hSubKey ) == ERROR_SUCCESS)
		{
			if( RegDeleteValue( hSubKey, lpValue) == ERROR_SUCCESS )
				bRet = TRUE;

			// Ű �ݱ�
			::RegCloseKey( hSubKey );

			return 0;
		}

		return -1;
	}

	BOOL DeleteRegistryKey( HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey )
	{
		HKEY hSubKey = NULL;
		BOOL bRet = FALSE;

		if ( ::RegOpenKeyEx( hKey, lpSubKey, 0, KEY_ALL_ACCESS, &hSubKey ) == ERROR_SUCCESS)
		{
			if( RegDeleteKey( hSubKey, lpValueKey) == ERROR_SUCCESS )
				bRet = TRUE;
			
			// Ű �ݱ�
			::RegCloseKey( hSubKey );

			return 0;
		}

		return -1;
	}


}


