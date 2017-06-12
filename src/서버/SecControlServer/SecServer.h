#pragma once
#include "sec/SecClientSock.h"
#include "sec/SecServerSock.h"
#include "SecControlWorker.h"
#define SECSERVER			SecServer::GetInstance()

class SecServerSock;
class SecServer
{
public:
	typedef list<SecClientSock*>		ClientList;

private:
	SecServer(void);
public:
	~SecServer(void);

public:
	static SecServer* GetInstance();

	int ServiceInit(int nCopyNo, char* szConfigFile);
	int ServiceRun();
	void ServiceDestroy();

	void SetEvent();
	void			SetOwnProc(ushort usNodeCode, ushort usCopyNo, char* szProcName);
	wemsGPN_st*		GetOwnProc() {return &ownProc_; }
	CubeBoxThread*	GetControlWorker() { return &control_worker_; }

	void AddClient(SecClientSock* client);
	SecClientSock*	GetClient(ushort usNodeCode, ushort usCopyNo, const char* szProcName);
	void DelClient();
	void CheckClient();

private:
	HANDLE				service_handle_;
	SecServerSock		server_sock_;
	ClientList			client_list_;
	SecControlWorker	control_worker_;
	wemsGPN_st			ownProc_;			//< 자신의 프로세스 이름
};

