#pragma once
#include "thread/CubeBoxThread.h"
#include "socket/wssession.h"
#include "sec/SecCommTypes.h"
#include "thread/ManualEvent.h"

#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif

//typedef int (__stdcall *PacketDataProcPtr)(wemsGPN_st* stGpnProc, wemsDataPacket_st* pRcvData);
typedef int (__stdcall *PacketDataProcPtr)(wemsGPN_st* stSrcProc, wemsGPN_st* stDestProc, wemsDataPacket_st* pRcvData);


class WELLS_Export SecClientSock : public CubeBoxThread, public wssession
{
public:
	SecClientSock(void);
	//SecClientSock(SOCKET socket);
	SecClientSock(SOCKET socket, ushort usNodeCode, ushort usCopyNo, const char* szProcName);
	~SecClientSock(void);

	void Activate();
	void Abort();

	virtual int Run();

	//----------------------------
	class  SecSockReceiver;
	friend class SecSockReceiver;
	SecSockReceiver* sock_receiver_;
	//----------------------------

	//----------------------------
 	class  SecSockWorker;
 	friend class SecSockWorker;
// 	SecSockWorker* sock_worker_;
	CubeBoxThread* sock_worker_;

	//----------------------------

	CManualEvent	  m_eventExit;

	///////////////////////////////
	//int ClientInit(char* proc_name, int copy_no, int timeoutsec);
	int ClientInit(ushort usNodeName, ushort usCopyNo, char* szProcName, char* lpszHostAddress, UINT nHostPort, int timeoutsec, PacketDataProcPtr pFunc); //< 자신의 프로세스 정보를 ...
	int ClientInit(ushort usNodeName, ushort usCopyNo, char* szProcName, char* lpszHostAddress, UINT nHostPort, int timeoutsec, CubeBoxThread* worker, PacketDataProcPtr pFunc); //< 클라이언트이고 자신의 worker 생성된 쓰레드를 사용
	int ClientInit(CubeBoxThread* worker, PacketDataProcPtr pFunc); // 서버 모드이고 자신의 worker 생성된 쓰레드를 사용
	int ClientInit(PacketDataProcPtr pFunc); // 서버 모드이고 SecSockWorker 쓰레드 사용

	void SetDestProc(ushort usNodeCode, ushort usCopyNo, char* szProcName);
	void SetSrcProc(ushort usNodeCode, ushort usCopyNo, char* szProcName);
	int  GetProcInfo(wemsProcInfo_st& proc_info);

	int	SendMsgData(ushort usDestNodeCode, ushort usDestCopyNo, char* szDestProcName, ushort nReqMsg, ushort nResmsg
		, int nCnt, char* pData, int iDataSize);

	int	SendMsgData(ushort usSrcNodeCode, ushort usSrcCopyNo, char* szSrcProcName
		, ushort usDestNodeCode, ushort usDestCopyNo, char* szDestProcName
		, ushort nReqMsg, ushort nResmsg, int nCnt, char* pData, int iDataSize);

	CubeBoxThread* GetDataProcThread(){ return sock_worker_; }

public:
	wemsGPN_st*		GetSrcProc() {return &stSrcProc_; }
	wemsGPN_st*		GetDestProc() {return &stDestProc_; }

	ushort			SocketType(){return usSocketType_;}

	PacketDataProcPtr	PacketDataProcFunc_; // 함수 포인터

private:
	wemsGPN_st	stDestProc_;
	wemsGPN_st	stSrcProc_;
	ushort		usSocketType_;

	int			clientType_;
};

class SecClientSock::SecSockReceiver : public CubeThread
{
public:
	//SecSockReceiver(void);
	SecSockReceiver(/*wssession*/SecClientSock& client);
	~SecSockReceiver(void);

protected:
	virtual int Run();

	void Activate(){}
	void Abort(){}

	///////////////////////////
	int ClientRecvPacket( wemsPacket_st *pRcvPacket );
#if 0
	int ServerRecvPacket( wemsPacket_st *pRcvPacket );
#endif
	int RcvPacketProc(wemsPacket_st *pRcvPacket );
private:
	SecClientSock&		client_sock_;
};


class SecClientSock::SecSockWorker : public CubeBoxThread
{
public:
	SecSockWorker(/*wssession*/SecClientSock& client);
	~SecSockWorker(void);

protected:
	virtual int Run();

	void Activate(){}
	void Abort(){}

private:	
	SecClientSock&		client_sock_;
};

