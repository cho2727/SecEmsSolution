#pragma once
#include "sec/SecClientSock.h"
#include "sec/SecCommTypes.h"

#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif

//class SecClientSock;
class WELLS_Export SecMultiSock
{
public:
	SecMultiSock(void);
	// SecMultiSock(bool realUse,);
	~SecMultiSock(void);

	static SecMultiSock* GetInstance();

	int		Init(ushort usSrcNodeCode, ushort usSrcCopyNo, char* szSrcProcName, char* addr, int realPort, int conPort, int msgPort, PacketDataProcPtr pFunc);
	int		Init(ushort usSrcNodeCode, ushort usSrcCopyNo, char* szSrcProcName, char* addr, int realPort, int conPort, int msgPort, CubeBoxThread* worker, PacketDataProcPtr pFunc);

	int		SendRealData(ushort usDestNodeCode, ushort usDestCopyNo, char* szDestProcName, ushort nReqMsg
		, int nCnt, char* pData, int iDataSize);
	int		SendRealData(ushort usSrcNodeCode, ushort usSrcCopyNo, char* szSrcProcName
		, ushort usDestNodeCode, ushort usDestCopyNo, char* szDestProcName, ushort nReqMsg, int nCnt, char* pData, int iDataSize);

	int		SendControlData(ushort usDestNodeCode, ushort usDestCopyNo, char* szDestProcName, ushort nReqMsg
		, int nCnt, char* pData, int iDataSize);
	int		SendControlData(ushort usSrcNodeCode, ushort usSrcCopyNo, char* szSrcProcName
		, ushort usDestNodeCode, ushort usDestCopyNo, char* szDestProcName, ushort nReqMsg, int nCnt, char* pData, int iDataSize);

	int		SendMessageData(ushort usDestNodeCode, ushort usDestCopyNo, char* szDestProcName, ushort nReqMsg
		, int nCnt, char* pData, int iDataSize);

	int		SendMessageData(ushort usSrcNodeCode, ushort usSrcCopyNo, char* szSrcProcName
		, ushort usDestNodeCode, ushort usDestCopyNo, char* szDestProcName, ushort nReqMsg, int nCnt, char* pData, int iDataSize);

	void	SockConnectCheck();

	void	Uninit();

private:
	void	RealSockAbort();
	void	ControlSockAbort();
	void	MessageSockAbort();

	int		RealSockConnect( char* addr, int port );
	int		ControlSockConnect( char* addr, int port );
	int		MessageSockConnect( char* addr, int port );

private:
	SecClientSock*	real_sock_;
	SecClientSock*	control_sock_;
	SecClientSock*	message_sock_;

	PacketDataProcPtr PacketDataProcFunc_;
	CubeBoxThread*		worker_;
	int					real_port_;
	int					control_port_;
	int					message_port_;

	ushort				node_code_;
	ushort				copy_no_;
	char				proc_name_[MAX_PROCNAME_SZ];

	char				server_addr_[MAX_ADDR_SZ];
};

