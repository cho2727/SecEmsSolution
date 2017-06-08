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
	wssocket				listen_socket_; //< ���� ����
	int						server_port_;		//< ���� ��Ʈ

};

