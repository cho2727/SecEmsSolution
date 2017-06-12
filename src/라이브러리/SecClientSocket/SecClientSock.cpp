#include "Sec/SecClientSock.h"
#include "thread/CubeData.h"
#include "log/CubeLog.h"
#include "sec/SecCommTypes.h"
#include "sec/SecMessageCode.h"
#include "sec/SecErrorCode.h"
#include "SecDataParser.h"
#include "sec/SecErrorCode.h"


#ifdef _DEBUG
#pragma comment(lib, "CubeLogd.lib")
#pragma comment(lib, "CubeThreadd.lib")
#pragma comment(lib, "CubeSocketd.lib")
#else
#pragma comment(lib, "CubeLog.lib")
#pragma comment(lib, "CubeThread.lib")
#pragma comment(lib, "CubeSocket.lib")
#endif

#define m_dwThreadWait			10

#define WS_SEND_DATA_TIMEOUT		1000

SecClientSock::SecClientSock(void)
	: sock_receiver_(NULL), sock_worker_(NULL), usSocketType_(SOCKET_CLIENT), clientType_(WORKER_NONE), PacketDataProcFunc_(NULL)
{
	memset(&stSrcProc_, 0x00, sizeof(wemsGPN_st));
	memset(&stDestProc_, 0x00, sizeof(wemsGPN_st));
}

SecClientSock::SecClientSock(SOCKET socket, ushort usNodeCode, ushort usCopyNo, const char* szProcName)
	: sock_receiver_(NULL), sock_worker_(NULL), usSocketType_(SOCKET_SERVER), clientType_(WORKER_NONE), PacketDataProcFunc_(NULL)
{
	Attach(socket);
	//sock_SetConnct();

	sock_receiver_ = new SecSockReceiver(*this);
	//sock_worker_ = new SecSockWorker(*this);
	memset(&stSrcProc_, 0x00, sizeof(wemsGPN_st));
	memset(&stDestProc_, 0x00, sizeof(wemsGPN_st));

	SetSrcProc(usNodeCode, usCopyNo, (char*)szProcName);
}

SecClientSock::~SecClientSock(void)
{
}

int SecClientSock::ClientInit(ushort usNodeName, ushort usCopyNo, char* szProcName, char* lpszHostAddress, UINT nHostPort, int timeoutsec, PacketDataProcPtr pFunc)
{
	int nRet = -1;
	try
	{
		if((nRet = sock_Init(lpszHostAddress, nHostPort, timeoutsec)) == 0) // �ʱ�ȭ
		{
			sock_receiver_ = new SecSockReceiver(*this);
			sock_worker_ = new SecSockWorker(*this);			
			// ���� ���� ����
			SetSrcProc(usNodeName, usCopyNo, szProcName);
			PacketDataProcFunc_ = pFunc;

			clientType_ = WORKER_OWN;
		}
	}
	catch(std::exception& ex)
	{
		WLOG("Excption : %0x\n", ex);
	}

	return nRet;
}


int SecClientSock::ClientInit(ushort usNodeName, ushort usCopyNo, char* szProcName, char* lpszHostAddress, UINT nHostPort, int timeoutsec, CubeBoxThread* worker, PacketDataProcPtr pFunc)
{
	int nRet = -1;
	try
	{
		if((nRet = sock_Init(lpszHostAddress, nHostPort, timeoutsec)) == 0) // �ʱ�ȭ
		{
			sock_receiver_ = new SecSockReceiver(*this);
			sock_worker_ = worker;			
			// ���� ���� ����
			SetSrcProc(usNodeName, usCopyNo, szProcName);
			PacketDataProcFunc_ = pFunc;

			clientType_ = WORKER_OTHER;
		}
	}

	catch(std::exception& ex)
	{
		WLOG("Excption : %0x\n", ex);
	}
	return nRet;
}

int SecClientSock::ClientInit(CubeBoxThread* worker)
{
	int nRet = -1;
	try
	{
		sock_receiver_ = new SecSockReceiver(*this);
		sock_worker_ = worker;
		//PacketDataProcFunc_ = pFunc;
		clientType_ = WORKER_OTHER;
	}
	catch(std::exception& ex)
	{
		WLOG("Excption : %0x\n", ex);
	}
	return nRet;
}

int SecClientSock::ClientInit(PacketDataProcPtr pFunc)
{
	int nRet = -1;
	try
	{
		sock_receiver_ = new SecSockReceiver(*this);
		sock_worker_ = new SecSockWorker(*this);
		PacketDataProcFunc_ = pFunc;
		clientType_ = WORKER_OWN;
	}
	catch(std::exception& ex)
	{
		WLOG("Excption : %0x\n", ex);
	}
	return nRet;
}


void SecClientSock::Activate()
{
	//m_shutdownEvent.Reset();
	m_eventExit.Reset();

	Start();
	if(sock_receiver_)
		sock_receiver_->Start();

	if(clientType_ == WORKER_OWN)
	{
		if(sock_worker_)
			sock_worker_->Start();
	}
}

void SecClientSock::Abort()
{
	// m_shutdownEvent.Set();
	m_eventExit.Set();
	if(sock_receiver_)
	{
		sock_receiver_->Wait();
		delete sock_receiver_;
		sock_receiver_ = NULL;
	}

	if(clientType_ == WORKER_OWN)
	{
		if(sock_worker_)
		{
			sock_worker_->Wait();
			delete sock_worker_;
			sock_worker_ = NULL;
		}
	}
	
	Wait();
}


void SecClientSock::SetSrcProc(ushort usNodeCode, ushort usCopyNo, char* szProcName)
{
	this->stSrcProc_.stNodeName = *(wemsNodeName_st*)&usNodeCode;
	this->stSrcProc_.stProcName.iCopy = usCopyNo;
	strcpy_s(this->stSrcProc_.stProcName.szProcName, szProcName);
}

void SecClientSock::SetDestProc(ushort usNodeCode, ushort usCopyNo, char* szProcName)
{
	this->stDestProc_.stNodeName = *(wemsNodeName_st*)&usNodeCode;
	this->stDestProc_.stProcName.iCopy = usCopyNo;
	strcpy_s(this->stDestProc_.stProcName.szProcName, szProcName);
}

int  SecClientSock::GetProcInfo(wemsProcInfo_st& proc_info)
{
	proc_info.usNodeCode = *(ushort*)&this->stSrcProc_.stNodeName;
	proc_info.usCopyNo = this->stSrcProc_.stProcName.iCopy;
	strcpy_s(proc_info.szProcName, this->stSrcProc_.stProcName.szProcName);

	return 0;
}

int SecClientSock::Run()
{
	DWORD				dwRet = 0x00;
	int					nCount = 0;
	wemsPacket_st*		SndPacketData;
	wemsDataPacket_st*	SndData;

	int					iMsgCode = 0;
	int					iSentSize = 0;
	int					iSendSize = 0;

	// ť���� �����͸� ������ �۽�
	while(true)
	{
		dwRet = ::WaitForSingleObject( m_eventExit.GetEvent(), m_dwThreadWait );
		if( dwRet  == WAIT_OBJECT_0 ) //�̺�Ʈ �߻���
		{
			break;
		}	

		CubeData* pData;
		try
		{
			if(this->MsgRecv(pData) == 0)
			{
				SndPacketData = (wemsPacket_st*) pData->get_buffer();
				iSendSize = (int)pData->buffer_size();

				try
				{
					iSentSize = sock_SendMsg(pData->get_buffer(), iSendSize, WS_SEND_DATA_TIMEOUT);
				}
				catch(CWSocketException& ex)
				{
					WLOG("src(%s:%02d) --> dest(%s:%02d) size(%d) idx(%d/%d) data snd fail(socket close:err:%d)\n"
						, SndPacketData->stPacketHeader.SrcProc.szProcName
						, SndPacketData->stPacketHeader.SrcProc.usCopyNo
						, SndPacketData->stPacketHeader.DestProc.szProcName
						, SndPacketData->stPacketHeader.DestProc.usCopyNo
						, SndPacketData->stPacketHeader.usPktLength
						, SndPacketData->stPacketHeader.usPktIdx
						, SndPacketData->stPacketHeader.usTotPktCnt, ex.GetErrorCode());
					break; // ���⸦ ��� ó���ؾ� �ϳ���????
				}

				if(SndPacketData->stPacketHeader.usPktIdx == 1)
				{
					SndData = (wemsDataPacket_st*)((char*)pData->get_buffer() + WEMSTCP_HEADER_SZ);
					iMsgCode = SndData->stDataHeader.usRequestFc;
				}

				if(iSentSize == iSendSize)
				{
					// �޽��� ���� ����(TCP)
#if 0
 					WLOG("src(%s:%02d) --> dest(%s:%02d) code(0x%0x) size(%d) idx(%d/%d) data snd suc\n"
 						, SndPacketData->stPacketHeader.SrcProc.szProcName
 						, SndPacketData->stPacketHeader.SrcProc.usCopyNo
 						, SndPacketData->stPacketHeader.DestProc.szProcName
 						, SndPacketData->stPacketHeader.DestProc.usCopyNo
 						, iMsgCode
 						, SndPacketData->stPacketHeader.usPktLength
 						, SndPacketData->stPacketHeader.usPktIdx
 						, SndPacketData->stPacketHeader.usTotPktCnt);
#endif
// 
				}
				else
				{
					// �޽��� ���� ����(TCP)
 					WLOG("src(%s:%s:%02d) --> dest(%s:%s:%02d) code(0x%0x) size(%d) idx(%d/%d) data snd fail\n"
 						, SndPacketData->stPacketHeader.SrcProc.szProcName
 						, SndPacketData->stPacketHeader.SrcProc.usCopyNo
 						, SndPacketData->stPacketHeader.DestProc.szProcName
 						, SndPacketData->stPacketHeader.DestProc.usCopyNo
 						, iMsgCode
 						, SndPacketData->stPacketHeader.usPktLength
 						, SndPacketData->stPacketHeader.usPktIdx
 						, SndPacketData->stPacketHeader.usTotPktCnt);
// 
				}

				delete pData;
			}
		}
		catch(CWSocketException& ex)
		{
			WLOG("snd msgq send exception(err:%d)\n", ex.GetErrorCode());
			::Sleep(100);
		}
	}

	if(!IsCreated()) // ������ �������� �ʾҴٸ�.
	{
		Close();
	}

	WLOG("Src(%s:%02d) Dest(%s:%02d) send thread run exit\n"
		, stSrcProc_.stProcName.szProcName
		, stSrcProc_.stProcName.iCopy
		, stDestProc_.stProcName.szProcName
		, stDestProc_.stProcName.iCopy);

	return 0;
}


//
// ������ ���� API
int		SecClientSock::SendMsgData(ushort usDestNodeCode, ushort usDestCopyNo, char* szDestProcName, ushort nReqMsg, ushort nResmsg
	, int nCnt, char* pData, int iDataSize)
{
	int nRet = 0;

	if(!IsCreated())
		return SEC_SOCK_CLOSE;
 
 	CubeData::CubeDataVec vData;
 	wemsGPN_st	*stProcName = GetSrcProc();
 
	nRet = SecDataParser::GetSecDataList(vData, *(ushort*)&stProcName->stNodeName, stProcName->stProcName.iCopy, (char*)stProcName->stProcName.szProcName,  usDestNodeCode
		, usDestCopyNo, szDestProcName, nReqMsg, nResmsg, nCnt, pData, iDataSize);
 
 	if(nRet == 0 && vData.size() > 0)
 	{
 		this->MsgSend((vData)); 
 	}
 	else
 	{
 		WLOG("src(%s:%02d) --> dest(%s:%02d) code(0x%0x) msg(%s) queue send fail\n"
 			, (char*)stProcName->stProcName.szProcName
 			, stProcName->stProcName.iCopy
 			, szDestProcName
 			, usDestCopyNo
 			, nReqMsg);
 	}
	return nRet;
}

int	SecClientSock::SendMsgData(ushort usSrcNodeCode, ushort usSrcCopyNo, char* szSrcProcName
	, ushort usDestNodeCode, ushort usDestCopyNo, char* szDestProcName
	, ushort nReqMsg, ushort nResmsg, int nCnt, char* pData, int iDataSize)
{
	int nRet = 0;

	if(!IsCreated())
		return SEC_SOCK_CLOSE;

	CubeData::CubeDataVec vData;

	nRet = SecDataParser::GetSecDataList(vData, usSrcNodeCode, usSrcCopyNo, szSrcProcName
		, usDestNodeCode, usDestCopyNo, szDestProcName, nReqMsg, nResmsg, nCnt, pData, iDataSize);

	if(nRet == 0 && vData.size() > 0)
	{
		this->MsgSend((vData)); 
	}
	else
	{
		WLOG("src(%s:%02d) --> dest(%s:%02d) code(0x%0x) msg(%s) queue send fail\n"
			, (char*)szSrcProcName
			, usSrcCopyNo
			, szDestProcName
			, usDestCopyNo
			, nReqMsg);
	}
	return nRet;
}
////////////////////////////////////////////////////////////////////


SecClientSock::SecSockReceiver::SecSockReceiver(/*wssession*/SecClientSock& client) : client_sock_(client)
{

}

SecClientSock::SecSockReceiver::~SecSockReceiver(void)
{
}

int SecClientSock::SecSockReceiver::Run()
{
	DWORD				dwRet = 0x00;
	wemsPacket_st		RcvPacketData;
	int					nRet = 0;

 	while(true)
 	{
 		dwRet = ::WaitForSingleObject( client_sock_.m_eventExit.GetEvent(), m_dwThreadWait );
 		if( dwRet  == WAIT_OBJECT_0 ) //�̺�Ʈ �߻���
 		{
 			break;
 		}

		if(!client_sock_.IsCreated())
		{
			continue;
		}
			
 		memset(&RcvPacketData, 0x00, sizeof(wemsPacket_st));
 		if(client_sock_.SocketType() == SOCKET_CLIENT) // Ŭ���̾�Ʈ �����̸�
 		{
 			nRet = ClientRecvPacket(&RcvPacketData);
 			if(nRet == SEC_OK)
 			{
 				RcvPacketProc(&RcvPacketData);
 			}
 			else if(nRet < SEC_OK)
 			{
				if(nRet == SEC_SOCK_TIMEOUT)
					continue;

 				client_sock_.Close(); // ���� ����
 			}
 		}
 		else if(client_sock_.SocketType() == SOCKET_SERVER)
 		{
 			nRet = ClientRecvPacket(&RcvPacketData);
 			if(nRet == SEC_OK)
 			{
 				RcvPacketProc(&RcvPacketData);
 			}
 			else if(nRet < SEC_OK)
 			{
				if(nRet == SEC_SOCK_TIMEOUT)
					continue;

 				client_sock_.Close(); // ���� ����
 			}
 		}
 	}

	WLOG("Src(%s:%02d) Dest(%s:%02d) recv thread run exit\n"
		, client_sock_.GetSrcProc()->stProcName.szProcName
		, client_sock_.GetSrcProc()->stProcName.iCopy
		, client_sock_.GetDestProc()->stProcName.szProcName
		, client_sock_.GetDestProc()->stProcName.iCopy);

	return 0;
}

int SecClientSock::SecSockReceiver::RcvPacketProc(wemsPacket_st *pRcvPacket )
{
	wemsDataPacket_st*	RcvData;
	RcvData = (wemsDataPacket_st*) pRcvPacket->pData;
	wemsGPN_st	*stProcName = client_sock_.GetSrcProc();
	char			szNodeName[WEMS_MAX_NODE_NAME_STR_SZ] = {0,};

	if(RcvData->stDataHeader.usRequestFc == FC_PROC_REG_REQS)
	{
		WLOG("proc(%s:%02d) register\n"
			, pRcvPacket->stPacketHeader.SrcProc.szProcName
			, pRcvPacket->stPacketHeader.SrcProc.usCopyNo);

		BOOL isConnect = TRUE;
		if(isConnect)
		{
			client_sock_.SetDestProc(pRcvPacket->stPacketHeader.SrcProc.usNodeCode, 
				pRcvPacket->stPacketHeader.SrcProc.usCopyNo, 
				pRcvPacket->stPacketHeader.SrcProc.szProcName);
		}
		else
		{
			client_sock_.Close(); // ���� ����
		}

		free(pRcvPacket->pData); 
	}
	else
	{
		// ó�� �����忡 ����
		//SecClientSock::SecSockWorker* pThread = client_sock_.GetDataProcThread();
		CubeBoxThread* workThread = client_sock_.GetDataProcThread();
		if(workThread != NULL)
		{
			// ó�������忡�� �޸� ����
			CubeData* pData = new CubeData();
			pData->SetSrcProcInfo(pRcvPacket->stPacketHeader.SrcProc.usNodeCode, pRcvPacket->stPacketHeader.SrcProc.usCopyNo, pRcvPacket->stPacketHeader.SrcProc.szProcName);
			pData->SetDestProcInfo(pRcvPacket->stPacketHeader.DestProc.usNodeCode, pRcvPacket->stPacketHeader.DestProc.usCopyNo, pRcvPacket->stPacketHeader.DestProc.szProcName);
			pData->set_swell_buffer(pRcvPacket->pData, pRcvPacket->stPacketHeader.usPktLength);
			workThread->MsgSend(pData);
		}
		else
		{
			WLOG("rcv data not proc (proc_thr is null)");
			free(pRcvPacket->pData); 
		}
	}

	return 0;
}

int SecClientSock::SecSockReceiver::ClientRecvPacket( wemsPacket_st *pRcvPacket )
{
	int			nRet = 0;
	int			nRecvSize = 0;
	int			nTotalDataSize = 0;
	wemsDataPacket_st*	RcvData;
	char*		pOffset = NULL;
	ushort		usPacketCount;
	ushort		usPacketIndex;
	ushort		usPacketDataSize = 0;

	int					iMsgCode = 0;
	try
	{
		if(!client_sock_.IsCreated())		// ������ ...
			return SEC_SOCK_CLOSE;

		int nReceivedSize = 0;
		nReceivedSize = client_sock_.sock_RecvMsg((char*)&pRcvPacket->stPacketHeader, WEMSTCP_HEADER_SZ, WEMSTCP_RCV_TIMEOUT); // ���� ���Ž� ��������� �д´�.
		if(nReceivedSize == WEMSTCP_HEADER_SZ && pRcvPacket->stPacketHeader.usPktLength > WEMSTCP_HEADER_SZ)
		{
			usPacketCount = pRcvPacket->stPacketHeader.usTotPktCnt;
			usPacketIndex = pRcvPacket->stPacketHeader.usPktIdx;
			nTotalDataSize = pRcvPacket->stPacketHeader.usTotPktCnt * WEMSTCP_PACKET_SZ;
			usPacketDataSize += pRcvPacket->stPacketHeader.usPktLength;

			pRcvPacket->pData = (char*)malloc(nTotalDataSize);
			memset(pRcvPacket->pData, 0x00, nTotalDataSize);
			pOffset = pRcvPacket->pData;

			do 
			{
				if(usPacketIndex > 1)
				{
					nReceivedSize = client_sock_.sock_RecvMsg((char*)&pRcvPacket->stPacketHeader, WEMSTCP_HEADER_SZ, WEMSTCP_RCV_TIMEOUT);
					if(nReceivedSize != WEMSTCP_HEADER_SZ)
					{
						// ��������.
						WLOG("rcv socket recevied size error");
						return SEC_SOCK_RCV_SIZE_ERR;
					}

					usPacketIndex = pRcvPacket->stPacketHeader.usPktIdx;
				}
				nRecvSize = pRcvPacket->stPacketHeader.usPktLength - WEMSTCP_HEADER_SZ;
				nReceivedSize = client_sock_.sock_RecvMsg((char*)pOffset, nRecvSize, WEMSTCP_RCV_TIMEOUT);
				if(nReceivedSize == nRecvSize)
				{
					pOffset += nRecvSize;
					RcvData = (wemsDataPacket_st*) pRcvPacket->pData;
					iMsgCode = RcvData->stDataHeader.usRequestFc;

#if 0
 					WLOG("src(%s:%02d) --> dest(%s:%02d) code(0x%0x) size(%d) idx(%d/%d) data rcv suc\n"
 						, pRcvPacket->stPacketHeader.SrcProc.szProcName
 						, pRcvPacket->stPacketHeader.SrcProc.usCopyNo
 						, pRcvPacket->stPacketHeader.DestProc.szProcName
 						, pRcvPacket->stPacketHeader.DestProc.usCopyNo
 						, iMsgCode
 						, pRcvPacket->stPacketHeader.usPktLength
 						, pRcvPacket->stPacketHeader.usPktIdx
 						, pRcvPacket->stPacketHeader.usTotPktCnt);
#endif
				}
				else
				{
					// ��������.
					WLOG("socket recevied size error");
					return SEC_SOCK_RCV_SIZE_ERR;
				}
				usPacketIndex++;
			} while (pRcvPacket->stPacketHeader.usPktIdx < usPacketCount);

			pRcvPacket->stPacketHeader.usPktLength = usPacketDataSize;
		}
		else
		{
			// ��������.
			WLOG("socket recevied size error");
			return SEC_SOCK_RCV_SIZE_ERR;
		}
	}
	catch(CWSocketException& ex)
	{
		if(ex.GetErrorCode() != WSAETIMEDOUT)
		{
			//���α׷� ����
			WLOG("proc(%s:%02d) rcv socket closed event(err:%d)\n"
				, client_sock_.GetSrcProc()->stProcName.szProcName
				, client_sock_.GetSrcProc()->stProcName.iCopy
				, ex.GetErrorCode());
			nRet = SEC_SOCK_CLOSE; // ���� Ŭ���� ����
		}
		else
		{
			nRet = SEC_SOCK_TIMEOUT; // ���� Ÿ�Ӿƿ� ���� (����)
		}
	}

	return nRet;
}

/////////////////////////////////////////////////////////////////////////////
SecClientSock::SecSockWorker::SecSockWorker(SecClientSock& client) : client_sock_(client)
{

}

SecClientSock::SecSockWorker::~SecSockWorker(void)
{
}

int SecClientSock::SecSockWorker::Run()
{
	DWORD				dwRet = 0x00;
//	wemsDataPacket_st*	SndData;

	while(true)
	{
		dwRet = ::WaitForSingleObject( client_sock_.m_eventExit.GetEvent(), 1 );
		if( dwRet  == WAIT_OBJECT_0 ) //�̺�Ʈ �߻���
		{
			break;
		}

		CubeData* pData;
		try
		{
			if(this->MsgRecv(pData) == 0)
			{
				wemsGPN_st GpnSrcProc;
				wemsGPN_st GpnDestProc;

				memset(&GpnSrcProc, 0x00, sizeof(wemsGPN_st));
				memset(&GpnDestProc, 0x00, sizeof(wemsGPN_st));

				GpnSrcProc.stProcName.iCopy = pData->SrcCopyNumber();
				strcpy_s(GpnSrcProc.stProcName.szProcName, pData->SrcProcName());
				ushort		usNodeCode = pData->SrcNodeCode();
				GpnSrcProc.stNodeName = *(wemsNodeName_st*)&usNodeCode;


				GpnDestProc.stProcName.iCopy = pData->DestCopyNumber();
				strcpy_s(GpnDestProc.stProcName.szProcName, pData->DestProcName());
				usNodeCode = pData->DestNodeCode();
				GpnDestProc.stNodeName = *(wemsNodeName_st*)&usNodeCode;

				wemsDataPacket_st* pDataPacket = (wemsDataPacket_st*)pData->get_buffer();

				if(client_sock_.PacketDataProcFunc_ != NULL)
				{
					int iRet = client_sock_.PacketDataProcFunc_(&GpnSrcProc, &GpnDestProc, pDataPacket);
					if(iRet == SEC_OK)
					{

					}
					else
					{

					}
				}
				else
				{
					WLOG("PacketDataProcFunc_ is NULL\n");
				}

				if(pData)
				{
					delete pData;
					pData = NULL;
				}
			}
		}
		catch(CWSocketException& ex)
		{

		}
	}

	WLOG("Src(%s:%02d) Dest(%s:%02d) proc thread run exit\n"
		, client_sock_.GetSrcProc()->stProcName.szProcName
		, client_sock_.GetSrcProc()->stProcName.iCopy
		, client_sock_.GetDestProc()->stProcName.szProcName
		, client_sock_.GetDestProc()->stProcName.iCopy);

	return 0;
}