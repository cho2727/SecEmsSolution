#include "sec/SecBaseConfig.h"
#include "sec/SecSocketInit.h"
#include "sec/SecErrorCode.h"
#include "common/CubeBaseString.h"

#import<msxml6.dll>

#pragma comment(lib, "ws2_32.lib")
#ifdef _DEBUG
#pragma comment(lib, "CubeBaseStringd.lib")
#else
#pragma comment(lib, "CubeBaseString.lib")
#endif


class SecBaseInit
{
public:
	SecBaseInit(void)
	{

	}
private:
	~SecBaseInit(void)
	{

	}

public:
	static SecBaseInit* GetInstance()
	{
		static SecBaseInit	_Instance;
		return &_Instance;
	}

private:
	SocketInit		sock_init_;
	CCoInitialize	ole_init_;
};


SecBaseConfig::SecBaseConfig(void)
	: real_port_(0), control_port_(0), message_port_(0)
{
	memset(server_addr_, 0x00, sizeof(server_addr_));
	memset(server_code_, 0x00, sizeof(server_code_));
	ConfigLoad();
}


SecBaseConfig::~SecBaseConfig(void)
{
}

int					SecBaseConfig::ConfigLoad()
{
	SecBaseInit::GetInstance();

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
	if(hr == VARIANT_FALSE)
	{
		return SEC_CONFIG_LOAD_FAIL;
	}

	MSXML2::IXMLDOMElementPtr	pDocNode = xmlPars->GetdocumentElement();
	if(NULL == pDocNode)
		return SEC_CONFIG_DOC_FAIL;

	try
	{
		// MSXML2::IXMLDOMNodeListPtr pNodeList = pDocNode->selectNodes("//sec_server/server");
		MSXML2::IXMLDOMElementPtr pFindNode = pDocNode->selectSingleNode("//sec_server/server");
		if(pFindNode != NULL)
		{
			_variant_t varValue;

			varValue = pFindNode->getAttribute("code");
			if (varValue.vt != VT_NULL)
				strcpy_s(server_code_, varValue.operator _bstr_t());
			varValue.Clear();

			varValue = pFindNode->getAttribute("addr");
			if (varValue.vt != VT_NULL)
				strcpy_s(server_addr_, varValue.operator _bstr_t());
			varValue.Clear();

			varValue = pFindNode->getAttribute("real_port");
			if (varValue.vt != VT_NULL)
				real_port_ = atoi(varValue.operator _bstr_t());
			varValue.Clear();

			varValue = pFindNode->getAttribute("control_port");
			if (varValue.vt != VT_NULL)
				control_port_ = atoi(varValue.operator _bstr_t());
			varValue.Clear();

			varValue = pFindNode->getAttribute("event_port");
			if (varValue.vt != VT_NULL)
				message_port_ = atoi(varValue.operator _bstr_t());
			varValue.Clear();
		}
		else
		{
			return SEC_CONFIG_NOT_DATA;
		}
	}
	catch (...)
	{
		return SEC_CONFIG_EXCEPTION;
	}

	return SEC_OK;
}

int					SecBaseConfig::ConfigLoad(char* szFileName)
{

	return 0;
}