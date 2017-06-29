#pragma once

#include <time.h>
#include <tchar.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <direct.h>
#include <Windows.h>

#ifndef STRING_Export
#ifdef  CUBEBASESTRING_EXPORTS
#define STRING_Export __declspec(dllexport)
#else
#define STRING_Export __declspec(dllimport)
#endif
#endif


namespace CubeBase
{
	STRING_Export void	GetModulePath(char strModulePath[]);
	STRING_Export void	GetModulePath(wchar_t strModulePath[]);

	STRING_Export void	GetModuleFile(char strModuleFile[]);
	STRING_Export void	GetModuleFile(wchar_t strModuleFile[]);

	STRING_Export char* ConvertWCtoC(wchar_t* str);
	STRING_Export wchar_t* ConverCtoWC(char* str);

	STRING_Export char* ConvertT2A(wchar_t* str, char pStr[]);
	STRING_Export wchar_t* ConvertA2T(char* str, wchar_t pStr[]);

	STRING_Export void Trim( char* str );
	STRING_Export void Rtrim( char* str );
	STRING_Export void Ltrim( char* str );
	STRING_Export int Alltrim( char* str, unsigned int len );
	STRING_Export char* GetNextFieldData( char *ptr, char deli, char *fdata );


	STRING_Export char* GetDatetime2StringA(char str[]);
	STRING_Export char* GetDatetime2StringB(char str[]);
	STRING_Export struct tm*	GetCurrentDatetime();

	STRING_Export	time_t GetDateTimeT(short year, short month, short day, short hour, short minute, short second);

	STRING_Export time_t GetString2Time(char* str);
	STRING_Export time_t GetString2TimeA(char* str);
	STRING_Export time_t GetString2TimeB(char* str);

	STRING_Export	char*	GetDateTime2StringA( time_t tm, char newDataTime[] );
	STRING_Export	char*	GetDateTime2StringB( time_t tm, char newDataTime[] );

	STRING_Export	char*	GetDateTime2StringA( char* tm, int addDays, char newDataTime[] );
	STRING_Export	char*	GetDateTime2StringB( char* tm, int addDays, char newDataTime[] );
	
	STRING_Export time_t 	GetString2DateTime(char* ttimestr);

	STRING_Export	void	GetArray2String(unsigned char* array, int len, char str[]);

	// STRING_Export char* 	GetStringFullPath(char inpath[], char fullpath[]); //< ������ ���� �Լ��ΰ�?

	STRING_Export time_t  NextScheduleTime(int method, int period, int wday, int day, int hour, int min, int sec);

	STRING_Export time_t 	GetNextDayTime(short v_hour, short v_min);						// ���� ��.��
	STRING_Export time_t 	GetNextDayTime(short v_hour, short v_min, int add_hour);		// ���� ��.�� + interval(��)
	STRING_Export time_t 	GetNextMonthTime(short v_day, short v_hour, short v_min, int add_month);		// ���� ��.�� + interval(��)

	STRING_Export time_t 	GetStartFixTime(int interval);		// interval�� �������� ����(��-20�̸� ������ 20��-���� ���� �ʰ� 32�̸� 40�ʷ� ���õ�)
																		// ���� 60�̸� �������� 0��
	STRING_Export int 	GetSecondNextFixMin(int value);		// ���� ��.��

	STRING_Export int ReadStringValueInRegistry( HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, TCHAR*	szValue);
	STRING_Export DWORD ReadDWORDValueInRegistry( HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, int dwDefault /*= -1*/ );
	STRING_Export int WriteStringValueInRegistry( HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, LPCTSTR lpValue );
	STRING_Export int WriteDWORDValueInRegistry( HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, DWORD dwValue );

	STRING_Export BOOL DeleteRegistryValue( HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValue );
	STRING_Export BOOL DeleteRegistryKey( HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey );

	STRING_Export BOOL CreateStringValueInRegistry(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, LPCTSTR lpValue);
	STRING_Export BOOL CreateDWORDValueInRegistry(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, DWORD dwValue);
};

