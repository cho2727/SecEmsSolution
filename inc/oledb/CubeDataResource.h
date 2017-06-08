#pragma once
// #include "CubeLocker.h"
#include <atldbcli.h>

#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif

class CubeLocker;
class CubeDynamicCommand;
class CubeDataSource;

class WELLS_Export CubeDataResource
{
public:	
	virtual ~CubeDataResource(void);

	static CubeDataResource*	GetInstance();

	CubeDynamicCommand*		GetCommand(const wchar_t* szServerName,const wchar_t* szDBName,const wchar_t* szQuery, bool bDBType = false);
	CubeDynamicCommand*		GetCommand(const wchar_t* szDBName,const wchar_t* szQuery, bool bDBType = false);

	ATL::CSession			GetSession(const wchar_t* szDBName);

private:
	CubeDataResource(void);
	//int Open(const wchar_t* szConnection);
	//void Close();

private:
	CubeDataSource* data_source_;	
};

#define WS_DB_MGR			CubeDataResource::GetInstance()
