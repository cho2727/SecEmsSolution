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
	CManualEvent			m_shutdownEvent;

	wssocket				m_listenerSocket; //< ���� ����
	int						m_nPolicyPort;		//< ���� ��Ʈ

};

