#pragma once

#include "thread/wsboxthread.h"
#include "thread/ManualEvent.h"
#include "socket/wssession.h"
#include "common/wscommtypes.h"
#include "proc/wsprocinfo.h"


#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif

typedef int (__stdcall *PacketDataProcPtr)(wemsGPN_st* stGpnProc, wemsDataPacket_st* pRcvData);

class wsreceiver;
class WELLS_Export wsclientsock : public wsboxthread, public wssession
{
public:
	wsclientsock(void);
	wsclientsock(SOCKET socket);
	~wsclientsock(void);

	void Activate();
	void Abort();

	virtual int Run();

	//----------------------------
	class  wsreceiver;
	friend class wsreceiver;
	wsreceiver* m_receiver;
	//----------------------------

	//----------------------------
	class  wsdataprocthr;
	friend class wsdataprocthr;
	wsdataprocthr* m_procthr;
	//----------------------------

	CManualEvent	  m_eventExit;

	///////////////////////////////
	//int ClientInit(char* proc_name, int copy_no, int timeoutsec);
	int ClientInit(ushort usNodeName, ushort usCopyNo, char* szProcName, char* lpszHostAddress, UINT nHostPort, int timeoutsec, PacketDataProcPtr pFunc); //< 자신의 프로세스 정보를 ...
	
	void SetDestProc(ushort usNodeCode, ushort usCopyNo, char* szProcName);
	void SetSrcProc(ushort usNodeCode, ushort usCopyNo, char* szProcName);
	int  GetProcInfo(wemsProcInfo_st& proc_info);

	int	SendMsgData(ushort usDestNodeCode, ushort usDestCopyNo, char* szDestProcName, ushort nReqMsg, ushort nResmsg
		, int nCnt, char* pData, int iDataSize);

	wsdataprocthr* GetDataProcThread(){ return m_procthr; }

public:
	wemsGPN_st*		GetSrcProc() {return &stSrcProc_; }
	wemsGPN_st*		GetDestProc() {return &stDestProc_; }

	ushort			SocketType(){return usSocketType_;}
		
	PacketDataProcPtr	PacketDataProcFunc_; // 함수 포인터

private:
	wemsGPN_st	stDestProc_;
	wemsGPN_st	stSrcProc_;
	ushort	usSocketType_;
};

class wsclientsock::wsreceiver : public wsthread
{
public:
	//wsreceiver(void);
	wsreceiver(/*wssession*/wsclientsock& client);
	~wsreceiver(void);

protected:
	virtual int Run();

	///////////////////////////
	int ClientRecvPacket( wemsPacket_st *pRcvPacket );
#if 0
	int ServerRecvPacket( wemsPacket_st *pRcvPacket );
#endif
	int RcvPacketProc(wemsPacket_st *pRcvPacket );
private:
	wsclientsock&		m_client_sock;
};


class wsclientsock::wsdataprocthr : public wsboxthread
{
public:
	wsdataprocthr(/*wssession*/wsclientsock& client);
	~wsdataprocthr(void);

protected:
	virtual int Run();
	
private:	
	wsclientsock&		m_client_sock;
};
