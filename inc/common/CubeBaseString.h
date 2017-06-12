#pragma once

#include <time.h>
#include <tchar.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <direct.h>
#include <Windows.h>

#ifndef UPC_Export
#ifdef UPC_DLL_EXPORTS
#define UPC_Export __declspec(dllexport)
#else
#define UPC_Export __declspec(dllimport)
#endif
#endif


namespace CubeBase
{
	UPC_Export void	GetModulePath(char strModulePath[]);
	UPC_Export void	GetModulePath(wchar_t strModulePath[]);

	UPC_Export void	GetModuleFile(char strModuleFile[]);
	UPC_Export void	GetModuleFile(wchar_t strModuleFile[]);

	UPC_Export char* ConvertWCtoC(wchar_t* str);
	UPC_Export wchar_t* ConverCtoWC(char* str);

	UPC_Export char* ConvertT2A(wchar_t* str, char pStr[]);
	UPC_Export wchar_t* ConvertA2T(char* str, wchar_t pStr[]);

	UPC_Export void Trim( char* str );
	UPC_Export void Rtrim( char* str );
	UPC_Export void Ltrim( char* str );
	UPC_Export int Alltrim( char* str, unsigned int len );
	UPC_Export char* GetNextFieldData( char *ptr, char deli, char *fdata );


	UPC_Export char* GetDatetime2StringA(char str[]);
	UPC_Export char* GetDatetime2StringB(char str[]);
	UPC_Export struct tm*	GetCurrentDatetime();

	UPC_Export	time_t GetDateTimeT(short year, short month, short day, short hour, short minute, short second);

	UPC_Export time_t GetString2Time(char* str);
	UPC_Export time_t GetString2TimeA(char* str);
	UPC_Export time_t GetString2TimeB(char* str);

	UPC_Export	char*	GetDateTime2StringA( time_t tm, char newDataTime[] );
	UPC_Export	char*	GetDateTime2StringB( time_t tm, char newDataTime[] );

	UPC_Export	char*	GetDateTime2StringA( char* tm, int addDays, char newDataTime[] );
	UPC_Export	char*	GetDateTime2StringB( char* tm, int addDays, char newDataTime[] );
	
	UPC_Export time_t 	GetString2DateTime(char* ttimestr);

	UPC_Export	void	GetArray2String(unsigned char* array, int len, char str[]);

	// UPC_Export char* 	GetStringFullPath(char inpath[], char fullpath[]); //< 무엇을 위한 함수인가?

	UPC_Export time_t  NextScheduleTime(int method, int period, int wday, int day, int hour, int min, int sec);

	UPC_Export time_t 	GetNextDayTime(short v_hour, short v_min);						// 다음 시.분
	UPC_Export time_t 	GetNextDayTime(short v_hour, short v_min, int add_hour);		// 다음 시.분 + interval(시)
	UPC_Export time_t 	GetNextMonthTime(short v_day, short v_hour, short v_min, int add_month);		// 다음 시.분 + interval(시)

	UPC_Export time_t 	GetStartFixTime(int interval);		// interval의 간격으로 수행(예-20이면 다음의 20초-만약 현재 초가 32이면 40초로 셋팅됨)
																		// 만약 60이면 다음분의 0초
	UPC_Export int 	GetSecondNextFixMin(int value);		// 다음 시.분

	UPC_Export int ReadStringValueInRegistry( HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, TCHAR*	szValue);
	UPC_Export DWORD ReadDWORDValueInRegistry( HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, int dwDefault /*= -1*/ );
	UPC_Export int WriteStringValueInRegistry( HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, LPCTSTR lpValue );
	UPC_Export int WriteDWORDValueInRegistry( HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, DWORD dwValue );

	UPC_Export BOOL DeleteRegistryValue( HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValue );
	UPC_Export BOOL DeleteRegistryKey( HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey );

	UPC_Export BOOL CreateStringValueInRegistry(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, LPCTSTR lpValue);
	UPC_Export BOOL CreateDWORDValueInRegistry(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, DWORD dwValue);
};

