#pragma once
#include "sec/SecClientSock.h"
#define SECSERVER			SecServer::GetInstance()


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

private:
	HANDLE			service_handle_;
	ClientList		client_list_;
	wemsGPN_st		ownProc_;			//< 자신의 프로세스 이름
};

