#include "CubeDataSource.h"
#import<msxml4.dll>

#if 0
void	GetDBConfigPath(char* strModulePath)
{
	if(!strModulePath)
		return;

	HMODULE	hModule;
	hModule = ::GetModuleHandleA( NULL );

	char szExeFileName[MAX_PATH];

	DWORD dwRet = GetModuleFileNameA( hModule ,szExeFileName, _MAX_PATH );
	char	Drive[ _MAX_DRIVE ];
	char	Path[ _MAX_PATH ];
	char	FileName[ _MAX_FNAME ];
	char	Ext[ _MAX_EXT ];

	_splitpath( szExeFileName , Drive , Path , FileName , Ext );

	sprintf(strModulePath, "%s\%s\\config\\wemsdbconfig.xml", Drive, Path);
}
#endif

void	GetDBConfigPath(wchar_t* strModulePath)
{
	HMODULE	hModule;
	hModule = ::GetModuleHandle( NULL );

	wchar_t szExeFileName[MAX_PATH];
	DWORD dwRet = GetModuleFileName( hModule ,szExeFileName, _MAX_PATH );
	wchar_t Drive[ _MAX_DRIVE ];
	wchar_t  Path[ _MAX_PATH ];
	wchar_t  FileName[ _MAX_FNAME ];
	wchar_t  Ext[ _MAX_EXT ];

	_wsplitpath( szExeFileName , Drive , Path , FileName , Ext );

	wsprintf(strModulePath, L"%s%sconfig\\db_config.xml", Drive, Path);
}


CubeDataSource::CubeDataSource(void)
{
}


CubeDataSource::~CubeDataSource(void)
{
	session_.Close();
	data_source_.Close();	
}


int CubeDataSource::GetConnectionString(const wchar_t* szServerName, const wchar_t* szDBName, wchar_t* szConnString)		// db_config.xml
{
	int ret = 0;
	USES_CONVERSION;

	MSXML2::IXMLDOMDocument2Ptr	xmlPars;

	HRESULT hr = xmlPars.CreateInstance(__uuidof(DOMDocument));
	if(FAILED(hr))
		return -1;

	wchar_t szFileFullName[MAX_PATH];
	GetDBConfigPath(szFileFullName);

	hr = xmlPars->load(W2A(szFileFullName));
	if(hr == 0)
	{		
		return -1;
	}

	MSXML2::IXMLDOMElementPtr	pDocNode = xmlPars->GetdocumentElement();
	if(NULL == pDocNode)
		return -1;

	MSXML2::IXMLDOMNodeListPtr pNodeList = pDocNode->selectNodes("//upc_config/area");

	if(pNodeList == NULL)
	{
		pDocNode->Release();
		return -1;
	}

	int nCount = pNodeList->length;

	try
	{
		wchar_t	szProvider[16] = {0,};
		wchar_t	szSource[32] = {0,};
		wchar_t	szName[32] = {0,};
		wchar_t	szUser[16] = {0,};
		wchar_t	szPassword[32] = {0,};
		wchar_t	szFailover[32] = {0,};
		for(int i=0; i<nCount; i++)
		{
			MSXML2::IXMLDOMElementPtr pFindNode = pNodeList->Getitem(i);
			BSTR bstrAttName = ::SysAllocString(L"server_name");
			_variant_t varValue = pFindNode->getAttribute(bstrAttName);
			if (bstrAttName) ::SysFreeString(bstrAttName);

			if (varValue.vt != VT_NULL)
			{
				if(wcscmp(szServerName, (varValue.operator _bstr_t())) == 0)
				{
					MSXML2::IXMLDOMNodeListPtr pChildList;
					varValue.Clear();
					ret = pFindNode->get_childNodes(&pChildList);
					if(ret !=0 )
					{
						pFindNode->Release();
						throw;
					}

					int slot_count = pChildList->length;
					try
					{
						for(int j=0; j<slot_count; j++)
						{
							MSXML2::IXMLDOMElementPtr pSlotNode = pChildList->Getitem(j);
							bstrAttName = ::SysAllocString(L"dbType");
							varValue = pSlotNode->getAttribute(bstrAttName);
							if (bstrAttName) ::SysFreeString(bstrAttName);

							if (varValue.vt != VT_NULL)
							{
								if(wcscmp(szDBName, (varValue.operator _bstr_t())) == 0)
								{
									varValue.Clear();

									MSXML2::IXMLDOMNodePtr pNode = pSlotNode->selectSingleNode("provider");
									if(pNode == NULL)
									{
										pSlotNode->Release();
										throw;
									}
									wcscpy(szProvider, pNode->Gettext());

									pNode = pSlotNode->selectSingleNode("data_source");
									if(pNode == NULL)
									{
										pSlotNode->Release();
										throw;
									}
									wcscpy(szSource, pNode->Gettext());

									pNode = pSlotNode->selectSingleNode("db_name");
									if(pNode == NULL)
									{
										pSlotNode->Release();
										throw;
									}
									wcscpy(szName, pNode->Gettext());

									pNode = pSlotNode->selectSingleNode("user_id");
									if(pNode == NULL)
									{
										pSlotNode->Release();
										throw;
									}
									wcscpy(szUser, pNode->Gettext());

									pNode = pSlotNode->selectSingleNode("user_password");
									if(pNode == NULL)
									{
										pSlotNode->Release();
										throw;
									}
									wcscpy(szPassword, pNode->Gettext());

									pNode = pSlotNode->selectSingleNode("failover");
									if(pNode == NULL)
									{
										pSlotNode->Release();
										throw;
									}
									wcscpy(szFailover, pNode->Gettext());

									wsprintf(szConnString, L"Provider=%s;User ID=%s;Password=%s;Initial Catalog=%s;Data Source=%s;Failover Partner=%s;"
										, szProvider
										, szUser
										, szPassword
										, szName
										, szSource
										, szFailover);
									ret = 0;
								}
							}
						}
					}
					catch (...)
					{
						pFindNode->Release();
						pChildList->Release();
						throw;
					}
				}
			}
			else
			{
				return -9;
			}
		}
	}
	catch (...)
	{
		pNodeList->Release();
		return -99;
	}

	return ret;
}



int CubeDataSource::GetConnectionString(const wchar_t* szDBName, wchar_t* szConnString) // db_config.xml
{
	int ret = 0;
	USES_CONVERSION;
	MSXML2::IXMLDOMDocument2Ptr	xmlPars;

	HRESULT hr = xmlPars.CreateInstance(__uuidof(DOMDocument));
	if(FAILED(hr))
		return -1;

	wchar_t szFileFullName[MAX_PATH];
	GetDBConfigPath(szFileFullName);

	hr = xmlPars->load(W2A(szFileFullName));
	if(hr == 0)
	{
		xmlPars->Release();
		return -1;
	}

	MSXML2::IXMLDOMElementPtr	pDocNode = xmlPars->GetdocumentElement();
	if(NULL == pDocNode)
	{
		xmlPars->Release();
		return -1;
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList = pDocNode->selectNodes("//upc_config/database");
	if(pNodeList == NULL)
	{
		pDocNode->Release();
		xmlPars->Release();
		return -1;
	}

	int nCount = pNodeList->length;

	try
	{
		wchar_t	szProvider[16] = {0,};
		wchar_t	szSource[32] = {0,};
		wchar_t	szName[32] = {0,};
		wchar_t	szUser[16] = {0,};
		wchar_t	szPassword[32] = {0,};
		wchar_t	szFailover[32] = {0,};
		for(int i=0; i<nCount; i++)
		{
			MSXML2::IXMLDOMElementPtr pFindNode = pNodeList->Getitem(i);
			BSTR bstrAttName = ::SysAllocString(L"type");
			_variant_t varValue = pFindNode->getAttribute(bstrAttName);
			if (bstrAttName) ::SysFreeString(bstrAttName);

			if (varValue.vt != VT_NULL)
			{
				try
				{
					if(wcscmp(szDBName, (varValue.operator _bstr_t())) == 0) // szDBName = db_name
					{
						varValue.Clear();
						MSXML2::IXMLDOMNodePtr pNode = pFindNode->selectSingleNode("provider");
						if(pNode == NULL)
						{
							throw;
						}
						wcscpy(szProvider, pNode->Gettext());
						pNode->Release();
						pNode = pFindNode->selectSingleNode("source");
						if(pNode == NULL)
						{
							throw;
						}
						wcscpy(szSource, pNode->Gettext());
						pNode->Release();
						pNode = pFindNode->selectSingleNode("name");
						if(pNode == NULL)
						{
							throw;
						}
						wcscpy(szName, pNode->Gettext());
						pNode->Release();
						pNode = pFindNode->selectSingleNode("user");
						if(pNode == NULL)
						{
							throw;
						}
						wcscpy(szUser, pNode->Gettext());
						pNode->Release();
						pNode = pFindNode->selectSingleNode("password");
						if(pNode == NULL)
						{
							throw;
						}
						wcscpy(szPassword, pNode->Gettext());
						pNode->Release();
						pNode = pFindNode->selectSingleNode("failover");
						if(pNode == NULL)
						{
							throw;
						}
						wcscpy(szFailover, pNode->Gettext());
						pNode->Release();
						wsprintf(szConnString, L"Provider=%s;User ID=%s;Password=%s;Initial Catalog=%s;Data Source=%s;Failover Partner=%s;"
							, szProvider
							, szUser
							, szPassword
							, szName
							, szSource
							, szFailover);

						ret = 0;
					}
				}
				catch (...)
				{
					pFindNode->Release();
					throw;
				}
			}
			else
			{
				pFindNode->Release();
				throw;
			}
			if(pNodeList != NULL)
				pFindNode->Release();
		} // for¹®
	}
	catch (...)
	{
		ret = -99;
	}

	if(pNodeList != NULL)
		pNodeList->Release();

	if(pDocNode != NULL)
		pDocNode->Release();

	if(xmlPars != NULL)
		xmlPars->Release();

	return ret;
}

HRESULT CubeDataSource::Open(const wchar_t* szServerName, const wchar_t* szDBName)
{
	wchar_t conn_str[256];

	HRESULT hr = GetConnectionString(szServerName, szDBName, conn_str);

	if(hr < 0)
		return hr;

	hr = data_source_.OpenFromInitializationString(conn_str);

	if(FAILED(hr))
	{
		data_source_.Close();
		return hr;
	}

	hr = session_.Open(data_source_);
	if(FAILED(hr))
	{
		session_.Close();
		data_source_.Close();
		return hr;
	}

	return hr;
}


HRESULT CubeDataSource::Open(const wchar_t* szDBName)
{
	wchar_t conn_str[256];

	HRESULT hr = GetConnectionString(szDBName, conn_str);

	if(hr < 0)
		return hr;

	hr = data_source_.OpenFromInitializationString(conn_str);

	if(FAILED(hr))
	{
		data_source_.Close();
		return hr;
	}

	hr = session_.Open(data_source_);
	if(FAILED(hr))
	{
		session_.Close();
		data_source_.Close();
		return hr;
	}

	return hr;
}

