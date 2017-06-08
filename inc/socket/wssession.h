#pragma once
#include "wssocket.h";
// #include "common/wscommtypes.h"
#include "thread/AutoEvent.h"

#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif

#define SESSION_INVALID_SOCKET 1000
#define SESSION_TIMEOUT_SOCKET 1100

enum SocketType_e 
{ 
	SOCKET_NONE, 			/**< socket type none */
	SOCKET_CLIENT,			/**< socket type client */ 
	SOCKET_SERVER 			/**< socket type server */
};

enum ClientType_e
{
	WORKER_NONE,
	WORKER_OWN,
	WORKER_OTHER
};

class WELLS_Export wssession : public wssocket
{
public:
	wssession(void);
	
	~wssession(void);

// 	bool	IsConnected(){return m_bConnected;}
// 	void	sock_SetConnct(bool val = true) {m_bConnected = val;}
	int		sock_Init(char* lpszHostAddress, UINT nHostPort, DWORD dwTimeout);
	void	sock_Uninit();

	void	sock_Flushing();
	int		sock_SendMsg(const void* pBuffer, int nBufLen, DWORD dwTimeout);
	//int		sock_SendMsg(BYTE bNodeCode, BYTE bActCode, BYTE bCompCode, ushort nReqMsg, ushort nResmsg, int nCnt, char* pData, int iDataSize);

	int		sock_RecvMsg(void* pBuffer, int nBufLen, DWORD dwTimeout);

	wssession(const wssession &rhs);
	wssession &operator=(const wssession &rhs);


private:

#if 0
	int		setDataHeader(wemsDataHeader_st* stDataHeader, ushort nReqMsg, ushort nResmsg, int cnt);
	int		SendPacket(BYTE node_code, BYTE act_code, BYTE compress, char* pData, int iDataSize);
	int		MakeResultHeader(wemsPacketHeader_st *SendHeader, 
						BYTE node_code, 
						BYTE action_code, 
						ushort tot_packet_cnt, 
						ushort packet_idx, 
						BYTE sequence, 
						BYTE compress, 
						ushort length );
#endif

protected:
	CAutoEvent m_WSABlockEvent;

// private:
// 	bool        m_bConnected;
};

