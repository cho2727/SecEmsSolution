#include "sec/SecMultiSock.h"
#include "sec/SecClientSock.h"
#include "sec/SecMessageCode.h"
#include "sec/SecErrorCode.h"
#include "log/CubeLog.h"

#define				MAX_CONNECT_TIMEOUT				1000
SecMultiSock::SecMultiSock(void)
	: real_sock_(NULL), control_sock_(NULL), message_sock_(NULL), worker_(NULL)
	, real_port_(0), control_port_(0), message_port_(0)
{
}


SecMultiSock::~SecMultiSock(void)
{
	Uninit();
}


SecMultiSock* SecMultiSock::GetInstance()
{
	static SecMultiSock	_Instance;
	return &_Instance;
}

int		SecMultiSock::Init(ushort usSrcNodeCode, ushort usSrcCopyNo, char* szSrcProcName
	, char* addr, int realPort, int conPort, int msgPort, PacketDataProcPtr pFunc)
{
	int iRet = 0;
	strcpy_s(server_addr_, addr);
	real_port_ = realPort;
	control_port_ = conPort;
	message_port_ = msgPort;

	node_code_ = usSrcNodeCode;
	copy_no_ = usSrcCopyNo;
	strcpy_s(proc_name_, szSrcProcName);

	PacketDataProcFunc_ = pFunc;

	iRet = RealSockConnect( server_addr_, real_port_ );

	iRet = ControlSockConnect( server_addr_, control_port_ );

	iRet = MessageSockConnect( server_addr_, message_port_ );
	WLOG("Server Port(addr:%s real:%d control:%d message:%d)\n", server_addr_, real_port_, control_port_, message_port_);
	return iRet;
}

int		SecMultiSock::Init(ushort usSrcNodeCode, ushort usSrcCopyNo, char* szSrcProcName
	, char* addr, int realPort, int conPort, int msgPort, CubeBoxThread* worker, PacketDataProcPtr pFunc)
{
	int iRet = 0;
	strcpy_s(server_addr_, addr);
	real_port_ = realPort;
	control_port_ = conPort;
	message_port_ = msgPort;

	node_code_ = usSrcNodeCode;
	copy_no_ = usSrcCopyNo;
	strcpy_s(proc_name_, szSrcProcName);

	PacketDataProcFunc_ = pFunc;
	worker_ = worker;

	iRet = RealSockConnect( server_addr_, real_port_ );

	iRet = ControlSockConnect( server_addr_, control_port_ );

	iRet = MessageSockConnect( server_addr_, message_port_ );
	WLOG("Server Port(addr:%s real:%d control:%d message:%d)\n",server_addr_, real_port_, control_port_, message_port_);
	return iRet;
}

int		SecMultiSock::SendRealData(ushort usDestNodeCode, ushort usDestCopyNo, char* szDestProcName, ushort nReqMsg
	, int nCnt, char* pData, int iDataSize)
{
	if( real_sock_ == NULL )
		return SEC_SOCK_NOT_CONNECT;

	return real_sock_->SendMsgData(usDestNodeCode, usDestCopyNo, szDestProcName, nReqMsg, nReqMsg, nCnt, pData, iDataSize);
}

int		SecMultiSock::SendRealData(ushort usSrcNodeCode, ushort usSrcCopyNo, char* szSrcProcName
	, ushort usDestNodeCode, ushort usDestCopyNo, char* szDestProcName, ushort nReqMsg, int nCnt, char* pData, int iDataSize)
{
	if( real_sock_ == NULL )
		return SEC_SOCK_NOT_CONNECT;

	return real_sock_->SendMsgData(usSrcNodeCode, usSrcCopyNo, szSrcProcName, usDestNodeCode, usDestCopyNo, szDestProcName, nReqMsg, nReqMsg, nCnt, pData, iDataSize);
}

int		SecMultiSock::SendControlData(ushort usDestNodeCode, ushort usDestCopyNo, char* szDestProcName, ushort nReqMsg
	, int nCnt, char* pData, int iDataSize)
{
	if( control_sock_ == NULL )
		return SEC_SOCK_NOT_CONNECT;

	return control_sock_->SendMsgData(usDestNodeCode, usDestCopyNo, szDestProcName, nReqMsg, nReqMsg, nCnt, pData, iDataSize);
}

int		SecMultiSock::SendControlData(ushort usSrcNodeCode, ushort usSrcCopyNo, char* szSrcProcName
	, ushort usDestNodeCode, ushort usDestCopyNo, char* szDestProcName, ushort nReqMsg, int nCnt, char* pData, int iDataSize)
{
	if( control_sock_ == NULL )
		return SEC_SOCK_NOT_CONNECT;

	return control_sock_->SendMsgData(usSrcNodeCode, usSrcCopyNo, szSrcProcName, usDestNodeCode, usDestCopyNo, szDestProcName, nReqMsg, nReqMsg, nCnt, pData, iDataSize);
}

int		SecMultiSock::SendMessageData(ushort usDestNodeCode, ushort usDestCopyNo, char* szDestProcName, ushort nReqMsg
	, int nCnt, char* pData, int iDataSize)
{
	if( message_sock_ == NULL )
		return SEC_SOCK_NOT_CONNECT;

	return message_sock_->SendMsgData(usDestNodeCode, usDestCopyNo, szDestProcName, nReqMsg, nReqMsg, nCnt, pData, iDataSize);
}

int		SecMultiSock::SendMessageData(ushort usSrcNodeCode, ushort usSrcCopyNo, char* szSrcProcName
	, ushort usDestNodeCode, ushort usDestCopyNo, char* szDestProcName, ushort nReqMsg, int nCnt, char* pData, int iDataSize)
{
	if( message_sock_ == NULL )
		return SEC_SOCK_NOT_CONNECT;

	return message_sock_->SendMsgData(usSrcNodeCode, usSrcCopyNo, szSrcProcName, usDestNodeCode, usDestCopyNo, szDestProcName, nReqMsg, nReqMsg, nCnt, pData, iDataSize);
}

void		SecMultiSock::Uninit()
{
	RealSockAbort();
	ControlSockAbort();
	MessageSockAbort();
}

void	SecMultiSock::SockConnectCheck()
{
	int		iRet;

	if(real_sock_)
	{
		if(!real_sock_->IsConnected())
		{
			RealSockAbort();
			iRet = RealSockConnect( server_addr_, real_port_ );
		}
	}
	else
	{
		iRet = RealSockConnect( server_addr_, real_port_ );
	}

	if(control_sock_)
	{
		if(!control_sock_->IsConnected())
		{
			ControlSockAbort();
			iRet = ControlSockConnect( server_addr_, control_port_ );
		}
	}
	else
	{
		iRet = ControlSockConnect( server_addr_, control_port_ );
	}

	if(message_sock_)
	{
		if(!message_sock_->IsConnected())
		{
			MessageSockAbort();
			iRet = MessageSockConnect( server_addr_, message_port_ );
		}
	}
	else
	{
		iRet = MessageSockConnect( server_addr_, message_port_ );
	}
	
}

int		SecMultiSock::RealSockConnect( char* addr, int port )
{
	int		iRet;

	if(port <= 0)
		return SEC_OK;

	real_sock_ = new SecClientSock();

	if(worker_)
	{
		iRet = real_sock_->ClientInit(node_code_, copy_no_, proc_name_
			, addr, port, MAX_CONNECT_TIMEOUT, worker_, PacketDataProcFunc_);
	}
	else
	{
		iRet = real_sock_->ClientInit(node_code_, copy_no_, proc_name_
			, addr, port, MAX_CONNECT_TIMEOUT, PacketDataProcFunc_);
	}
	
	if(iRet != 0)
	{
		if(real_sock_)
		{
			delete real_sock_;
			real_sock_ = NULL;
		}
		WLOG("Real Sock Connect Fail(ret:%d)\n", iRet);
	}
	else
	{
		real_sock_->SetDestProc(node_code_, SEC_ONLY_COPYNUM, SEC_REAL_PROC);
		real_sock_->Activate();
		real_sock_->SendMsgData(node_code_, SEC_ONLY_COPYNUM, SEC_REAL_PROC, FC_PROC_REG_REQS, FC_PROC_REG_REQS, 0, NULL, 0);
		WLOG("Real Sock Connect Succ\n");
	}

	return iRet;
}

int		SecMultiSock::ControlSockConnect( char* addr, int port )
{
	int		iRet;

	if(port <= 0)
		return SEC_OK;

	control_sock_ = new SecClientSock();

	if(worker_)
	{
		iRet = control_sock_->ClientInit(node_code_, copy_no_, proc_name_
			, addr, port, MAX_CONNECT_TIMEOUT, worker_, PacketDataProcFunc_);
	}
	else
	{
		iRet = control_sock_->ClientInit(node_code_, copy_no_, proc_name_
			, addr, port, MAX_CONNECT_TIMEOUT, PacketDataProcFunc_);
	}

	if(iRet != 0)
	{
		if(control_sock_)
		{
			delete control_sock_;
			control_sock_ = NULL;
		}
		WLOG("Control Sock Connect Fail(ret:%d)\n", iRet);
	}
	else
	{
		control_sock_->SetDestProc(node_code_, SEC_ONLY_COPYNUM, SEC_CON_PROC);
		control_sock_->Activate();
		control_sock_->SendMsgData(node_code_, SEC_ONLY_COPYNUM, SEC_CON_PROC, FC_PROC_REG_REQS, FC_PROC_REG_REQS, 0, NULL, 0);
		WLOG("Control Sock Connect Succ\n");
	}

	return iRet;
}

int		SecMultiSock::MessageSockConnect( char* addr, int port )
{
	int		iRet;

	if(port <= 0)
		return SEC_OK;

	message_sock_ = new SecClientSock();

	if(worker_)
	{
		iRet = message_sock_->ClientInit(node_code_, copy_no_, proc_name_
			, addr, port, MAX_CONNECT_TIMEOUT, worker_, PacketDataProcFunc_);
	}
	else
	{
		iRet = message_sock_->ClientInit(node_code_, copy_no_, proc_name_
			, addr, port, MAX_CONNECT_TIMEOUT, PacketDataProcFunc_);
	}

	if(iRet != 0)
	{
		if(message_sock_)
		{
			delete message_sock_;
			message_sock_ = NULL;
		}
		WLOG("Message Sock Connect Fail(ret:%d)\n", iRet);
	}
	else
	{
		message_sock_->SetDestProc(node_code_, SEC_ONLY_COPYNUM, SEC_MSG_PROC);
		message_sock_->Activate();
		message_sock_->SendMsgData(node_code_, SEC_ONLY_COPYNUM, SEC_MSG_PROC, FC_PROC_REG_REQS, FC_PROC_REG_REQS, 0, NULL, 0);

		WLOG("Message Sock Connect Succ\n");
	}

	return iRet;
}

void	SecMultiSock::RealSockAbort()
{
	if(real_sock_)
	{
		real_sock_->Abort();
		delete real_sock_;
		real_sock_ = NULL;
	}
}

void	SecMultiSock::ControlSockAbort()
{
	if(control_sock_)
	{
		control_sock_->Abort();
		delete control_sock_;
		control_sock_ = NULL;
	}
}

void	SecMultiSock::MessageSockAbort()
{
	if(message_sock_)
	{
		message_sock_->Abort();
		delete message_sock_;
		message_sock_ = NULL;
	}
}