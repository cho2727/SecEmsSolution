#include "oledb/CubeDataResource.h"
#include "oledb/CubeDynamicCommand.h"
#include "CubeDataSource.h"
#include "CubeLocker.h"
#include "CubeGuard.h"

#if defined(_DEBUG)
#pragma comment(lib, "CubeBaseStringd.lib")
#else
#pragma comment(lib, "CubeBaseString.lib")
#endif

CubeLocker			locker_;

CubeDataResource::CubeDataResource(void)
	:data_source_(NULL)
{
	//HRESULT hr = CoInitialize( NULL );
	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED );
}


CubeDataResource::~CubeDataResource(void)
{
	if(data_source_ != NULL)
		delete data_source_;

	data_source_ = NULL;
	CoUninitialize();
}

CubeDataResource*	CubeDataResource::GetInstance()
{
	static CubeDataResource _Instance;
	return &_Instance;
}

#if 0
int CubeDataResource::Open(const wchar_t* szConnection)
{
	data_source_ = new CubeDataSource();
	int ret = data_source_->Open(szConnection);

	return ret;
}
#endif

CubeDynamicCommand*		CubeDataResource::GetCommand(const wchar_t* szServerName,const wchar_t* szDBName,const wchar_t* szQuery, bool bDBType)
{
	CubeGuard	guard	( &locker_ )	;
	data_source_ = new CubeDataSource();
	int ret = data_source_->Open(szServerName, szDBName);

	if(ret != 0)
	{
		return NULL;
	}

	CubeDynamicCommand* comm = new CubeDynamicCommand(szQuery);	
	HRESULT hr = comm->Open(data_source_->GetSession(), szQuery);
	if(FAILED(hr))
	{
		//comm->Close();
		delete data_source_;
		data_source_ = NULL;
		delete comm;
		return NULL;
	}

	delete data_source_;
	data_source_ = NULL;
	return comm;
}


CubeDynamicCommand*		CubeDataResource::GetCommand(const wchar_t* szDBName,const wchar_t* szQuery, bool bDBType)
{
	CubeGuard	guard	( &locker_ );
	data_source_ = new CubeDataSource();
	int ret = data_source_->Open(szDBName);

	if(ret != 0)
	{
		return NULL;
	}

	CubeDynamicCommand* comm = new CubeDynamicCommand(szQuery);	
	HRESULT hr = comm->Open(data_source_->GetSession(), szQuery);
	if(FAILED(hr))
	{
		//comm->Close();
		delete data_source_;
		data_source_ = NULL;
		delete comm;
		return NULL;
	}

	delete data_source_;
	data_source_ = NULL;
	return comm;
}


ATL::CSession			CubeDataResource::GetSession(const wchar_t* szDBName)
{
	CubeGuard	guard	( &locker_ );
	if(data_source_ == NULL)
	{
		data_source_ = new CubeDataSource();
		int ret = data_source_->Open(szDBName);
	}

	return data_source_->GetSession();
}