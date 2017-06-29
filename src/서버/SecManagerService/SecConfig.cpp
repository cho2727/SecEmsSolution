#include "StdAfx.h"
#include "SecConfig.h"
#include "common/CubeBaseString.h"
#include "process/SecProcManager.h"

#import<msxml6.dll>


SecConfig::SecConfig(void)
{
}


SecConfig::~SecConfig(void)
{
}

SecConfig* SecConfig::GetInstance()
{
	static SecConfig	_Instance;
	return &_Instance;
}

int			SecConfig::ConfigLoad(char* szFileName)
{
	return 0;
}


int				SecConfig::ManagerProcLoad()
{
	int iRet = 0;

	MSXML2::IXMLDOMDocument2Ptr	xmlPars;
	char		szModulePath[MAX_PATH] = {0,};
	char		szFullFileName[MAX_PATH] = {0,};

	HRESULT hr = xmlPars.CreateInstance(__uuidof(DOMDocument));
	if(FAILED(hr))
	{
		return SEC_CONFIG_INIT_FAIL;
	}

	CubeBase::GetModulePath(szModulePath);
	sprintf(szFullFileName, "%sconfig\\sec_server.xml", szModulePath);

	hr = xmlPars->load(szFullFileName);
	if(hr == 0)
	{
		return SEC_CONFIG_LOAD_FAIL;
	}

	MSXML2::IXMLDOMElementPtr	pDocNode = xmlPars->GetdocumentElement();
	if(NULL == pDocNode)
		return SEC_CONFIG_DOC_FAIL;

	try
	{
		MSXML2::IXMLDOMNodeListPtr pNodeList = pDocNode->selectNodes("//sec_server/manager/manaproc");

		if(pNodeList == NULL)
			return SEC_CONFIG_SEL_FAIL;
		

		if(pNodeList != NULL)
		{
			char		proc_name[MAX_PROCNAME_SZ] = {0, };
			char		config_file[PROC_CONFIG_SZ] = {0, };
			ushort		copy_no;
			char		temp[8];
			bool		auto_start;

			for(int i=0; i<pNodeList->length; i++)
			{
				MSXML2::IXMLDOMElementPtr pFindNode = pNodeList->Getitem(i);
				_variant_t varValue;

				varValue = pFindNode->getAttribute("name");
				if (varValue.vt != VT_NULL)
					strcpy_s(proc_name, varValue.operator _bstr_t());
				varValue.Clear();
				
				varValue = pFindNode->getAttribute("copyno");
				if (varValue.vt != VT_NULL)
					copy_no = (ushort) atoi(varValue.operator _bstr_t());
				varValue.Clear();

				varValue = pFindNode->getAttribute("autostart");
				if (varValue.vt != VT_NULL)
					strcpy_s(temp, varValue.operator _bstr_t());
				varValue.Clear();

				varValue = pFindNode->getAttribute("config");
				if (varValue.vt != VT_NULL)
					strcpy_s(config_file, varValue.operator _bstr_t());
				varValue.Clear();

				auto_start = toupper(temp[0]) == 'Y' ? true:false;

				WLOG("P(%s:%d) AUTO(%s) config(%s)\n", proc_name, copy_no, temp, config_file);
				SecProcMana::SecProcManager::GetInstance()->AddProgram(proc_name, 0, auto_start, config_file);
			}
		}
	}
	catch (...)
	{
		return SEC_CONFIG_EXCEPTION;
	}

	return iRet;
}