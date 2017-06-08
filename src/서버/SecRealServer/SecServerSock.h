#pragma once

#include "thread/ManualEvent.h"
#include "thread/CubeThread.h"
#include "socket/wssocket.h"

class SecServerSock : public CubeThread, public wssocket
{
public:
	SecServerSock(void);
	~SecServerSock(void);


	int		Init();
	void	Activate();
	void	Abort();

	virtual int Run();
	//////////////////////////////////////////////
	bool NotifyAccept(void);

private:
	CManualEvent			shutdown_event_;
	wssocket				listen_socket_; //< 리슨 소켓
	int						server_port_;		//< 서버 포트

};

