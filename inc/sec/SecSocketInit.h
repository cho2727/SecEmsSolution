#pragma once
#include <stdio.h>
class SocketInit
{
public:
	SocketInit()
	{
		WSADATA wsadata;
		if (WSAStartup(MAKEWORD(2,2), &wsadata) != 0) {
			//TRACE("WSAStartup Error!!");
		}
	}

	virtual ~SocketInit()
	{
		WSACleanup();
		printf("WSACleanup\n");
	}
};



class CCoInitialize
{
public:
	CCoInitialize(void)
	{
		//ASSERT(SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED)));
		HRESULT result = CoInitialize( NULL );
	}
	~CCoInitialize(void)
	{
		CoUninitialize();
		printf("CoUninitialize\n");
	}
};

class CCoInitializeEx
{
public:
	CCoInitializeEx(void)
	{
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
	}
	~CCoInitializeEx(void)
	{
		CoUninitialize();
	}
};
