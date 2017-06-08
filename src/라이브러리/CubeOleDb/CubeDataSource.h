#pragma once

#include <atldbcli.h>

class CubeDataSource
{
public:
	CubeDataSource(void);
	virtual ~CubeDataSource(void);

	HRESULT Open(const wchar_t* szServerName, const wchar_t* szDBName);
	HRESULT Open(const wchar_t* szDBName);
	CSession	GetSession();

	int GetConnectionString(const wchar_t* szServerName, const wchar_t* szDBName, wchar_t* szConnString);
	int GetConnectionString(const wchar_t* szDBName, wchar_t* szConnString);

private:
	CDataSource		data_source_;
	CSession		session_;
};

inline CSession	CubeDataSource::GetSession(){
	return session_;
}


