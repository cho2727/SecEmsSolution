#pragma once

#include "thread/ManualEvent.h"
#include "thread/CubeThread.h"
#include "socket/wssocket.h"

#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif

typedef int (__stdcall *ServerAccept)(SOCKET client_socket);

class WELLS_Export SecServerSock : public CubeThread, public wssocket
{
public:
	SecServerSock(void);
	SecServerSock(int nPort, ServerAccept pFunc);
	~SecServerSock(void);


	void	Activate();
	void	Abort();

	virtual int Run();
	//////////////////////////////////////////////
	bool NotifyAccept(void);

private:
	CManualEvent			shutdown_event_;
	wssocket				listen_socket_; //< 리슨 소켓
	int						server_port_;		//< 서버 포트

	ServerAccept			serverAccept_;

};