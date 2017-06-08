#pragma once

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
	}
};