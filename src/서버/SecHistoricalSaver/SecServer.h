#pragma once
#include "sec/SecClientSock.h"
#include "sec/SecMultiSock.h"

#define SECSERVER			SecServer::GetInstance()


class SecServer
{
private:
	SecServer(void);
public:
	~SecServer(void);

public:
	static SecServer* GetInstance();

	int					ServiceInit(int nCopyNo, char* szConfigFile);
	int					ServiceRun();
	void				ServiceDestroy();

	void				SetEvent();
	void				SetOwnProc(ushort usNodeCode, ushort usCopyNo, char* szProcName);
	wemsGPN_st*			GetOwnProc() {return &ownProc_; }

	SecMultiSock*		GetMultiSock() { return &multi_sock_; }

private:
	HANDLE				service_handle_;
	wemsGPN_st			ownProc_;			//< 자신의 프로세스 이름

	SecMultiSock		multi_sock_;
};

