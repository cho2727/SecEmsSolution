#include "CubeDataSource.h"
#include "common/CubeBaseString.h"
#import<msxml3.dll>


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
	MSXML2::IXMLDOMDocument2Ptr	xmlPars;

	HRESULT hr = xmlPars.CreateInstance(__uuidof(DOMDocument));
	if(FAILED(hr))
		return -1;

	char szFileFullName[MAX_PATH] = {0,};
	char szModulePath[MAX_PATH] = {0,};
	CubeBase::GetModulePath(szModulePath);
	sprintf(szFileFullName, "%sconfig\\db_config.xml", szModulePath);

	hr = xmlPars->load(szFileFullName);
	if(hr == 0)
	{
		return -1;
	}

	MSXML2::IXMLDOMElementPtr	pDocNode = xmlPars->GetdocumentElement();
	if(NULL == pDocNode)
		return -1;

	MSXML2::IXMLDOMNodeListPtr pNodeList = pDocNode->selectNodes("//upc_config/area");

	if(pNodeList == NULL)
		return -1;

	int nCount = pNodeList->length;

	try
	{
		ret = -1;
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
						return -1;
					}

					int slot_count = pChildList->length;
					for(int j=0; j<slot_count; j++)
					{
						MSXML2::IXMLDOMElementPtr pSlotNode = pChildList->Getitem(j);
						MSXML2::IXMLDOMNodeListPtr pSlotList;
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
									return -10;
								}
								wcscpy(szProvider, pNode->Gettext());

								pNode = pSlotNode->selectSingleNode("data_source");
								if(pNode == NULL)
								{
									return -11;
								}
								wcscpy(szSource, pNode->Gettext());

								pNode = pSlotNode->selectSingleNode("db_name");
								if(pNode == NULL)
								{
									return -12;
								}
								wcscpy(szName, pNode->Gettext());

								pNode = pSlotNode->selectSingleNode("user_id");
								if(pNode == NULL)
								{
									return -13;
								}
								wcscpy(szUser, pNode->Gettext());

								pNode = pSlotNode->selectSingleNode("user_password");
								if(pNode == NULL)
								{
									return -14;
								}
								wcscpy(szPassword, pNode->Gettext());

#if 0

								pNode = pSlotNode->selectSingleNode("failover");
								if(pNode == NULL)
								{
									return -15;
								}
								wcscpy(szFailover, pNode->Gettext());
#endif

								wsprintf(szConnString, L"Provider=%s;User ID=%s;Password=%s;Initial Catalog=%s;Data Source=%s;Failover Partner=%s;"
									, szProvider
									, szUser
									, szPassword
									, szName
									, szSource
									, szFailover);

								ret = 0;
								return ret;
							}
						}
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
		return -99;
	}

	//CoUninitialize();
	return ret;
}



int CubeDataSource::GetConnectionString(const wchar_t* szDBName, wchar_t* szConnString) // db_config.xml
{
	int ret = 0;
	MSXML2::IXMLDOMDocument2Ptr	xmlPars;

	HRESULT hr = xmlPars.CreateInstance(__uuidof(DOMDocument));
	if(FAILED(hr))
		return -1;

	char szFileFullName[MAX_PATH] = {0,};
	char szModulePath[MAX_PATH] = {0,};
	CubeBase::GetModulePath(szModulePath);
	sprintf(szFileFullName, "%sconfig\\db_config.xml", szModulePath);

	hr = xmlPars->load(szFileFullName);
	if(hr == 0)
	{
		return -1;
	}

	MSXML2::IXMLDOMElementPtr	pDocNode = xmlPars->GetdocumentElement();
	if(NULL == pDocNode)
		return -1;

	MSXML2::IXMLDOMNodeListPtr pNodeList = pDocNode->selectNodes("//upc_config/database");
	if(pNodeList == NULL)
		return -1;

	int nCount = pNodeList->length;

	try
	{
		ret = -1;
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
				if(wcscmp(szDBName, (varValue.operator _bstr_t())) == 0) // szDBName = db_name
				{
					varValue.Clear();
					MSXML2::IXMLDOMNodePtr pNode = pFindNode->selectSingleNode("provider");
					if(pNode == NULL)
					{
						return -10;
					}
					wcscpy(szProvider, pNode->Gettext());

					pNode = pFindNode->selectSingleNode("source");
					if(pNode == NULL)
					{
						return -11;
					}
					wcscpy(szSource, pNode->Gettext());

					pNode = pFindNode->selectSingleNode("name");
					if(pNode == NULL)
					{
						return -12;
					}
					wcscpy(szName, pNode->Gettext());

					pNode = pFindNode->selectSingleNode("user");
					if(pNode == NULL)
					{
						return -13;
					}
					wcscpy(szUser, pNode->Gettext());

					pNode = pFindNode->selectSingleNode("password");
					if(pNode == NULL)
					{
						return -14;
					}
					wcscpy(szPassword, pNode->Gettext());

					pNode = pFindNode->selectSingleNode("failover");
					if(pNode == NULL)
					{
						return -15;
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
					return ret;
				}
			}
			else
			{
				return -9;
			}
		} // for¹®
	}
	catch (...)
	{
		return -99;
	}

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

