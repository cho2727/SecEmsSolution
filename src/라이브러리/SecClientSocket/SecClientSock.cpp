#include "Sec/SecClientSock.h"
#include "thread/CubeData.h"
#include "log/CubeLog.h"
#include "sec/SecCommTypes.h"
#include "sec/SecMessageCode.h"
#include "SecDataParser.h"


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
		if((nRet = sock_Init(lpszHostAddress, nHostPort, timeoutsec)) == 0) // 초기화
		{
			sock_receiver_ = new SecSockReceiver(*this);
			sock_worker_ = new SecSockWorker(*this);			
			// 정의 정보 전송
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
		if((nRet = sock_Init(lpszHostAddress, nHostPort, timeoutsec)) == 0) // 초기화
		{
			sock_receiver_ = new SecSockReceiver(*this);
			sock_worker_ = worker;			
			// 정의 정보 전송
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

int SecClientSock::ClientInit(CubeBoxThread* worker, PacketDataProcPtr pFunc)
{
	int nRet = -1;
	try
	{
		sock_receiver_ = new SecSockReceiver(*this);
		sock_worker_ = worker;
		PacketDataProcFunc_ = pFunc;
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

//	char			szSrcNodeName[WEMS_MAX_NODE_NAME_STR_SZ];
//	char			szDestNodeName[WEMS_MAX_NODE_NAME_STR_SZ];
	//char			szDateTime[64] ={0,};

	int					iMsgCode = 0;
	int					iSentSize = 0;
	int					iSendSize = 0;

	// 큐에서 데이터를 꺼내어 송신
	while(true)
	{
		dwRet = ::WaitForSingleObject( m_eventExit.GetEvent(), m_dwThreadWait );
		if( dwRet  == WAIT_OBJECT_0 ) //이벤트 발생시
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
					WLOG("snd data error(err:%d)\n", ex.GetErrorCode());
					WLOG("src(%s:%02d) --> dest(%s:%02d) size(%d) idx(%d/%d) data snd fail(socket close)\n"
						, SndPacketData->stPacketHeader.SrcProc.szProcName
						, SndPacketData->stPacketHeader.SrcProc.usCopyNo
						, SndPacketData->stPacketHeader.DestProc.szProcName
						, SndPacketData->stPacketHeader.DestProc.usCopyNo
						, SndPacketData->stPacketHeader.usPktLength
						, SndPacketData->stPacketHeader.usPktIdx
						, SndPacketData->stPacketHeader.usTotPktCnt);
					break; // 여기를 어떻게 처리해야 하나요????
				}

				if(SndPacketData->stPacketHeader.usPktIdx == 1)
				{
					SndData = (wemsDataPacket_st*)((char*)pData->get_buffer() + WEMSTCP_HEADER_SZ);
					iMsgCode = SndData->stDataHeader.usRequestFc;
				}

				if(iSentSize == iSendSize)
				{
					// 메시지 전송 성공(TCP)
 					WLOG("src(%s:%02d) --> dest(%s:%02d) code(0x%0x) size(%d) idx(%d/%d) data snd suc\n"
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
				else
				{
					// 메시지 전송 실패(TCP)
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

	if(!IsCreated()) // 소켓이 끊어지지 않았다면.
	{
		Close();
	}

//	char			szNodeName[WEMS_MAX_NODE_NAME_STR_SZ];
	WLOG("proc(%s:%d) snd thread run exit\n"
		, stSrcProc_.stProcName.szProcName
		, stSrcProc_.stProcName.iCopy);

//	sock_SetConnct(false);
	return 0;
}


//
// 데이터 전송 API
int		SecClientSock::SendMsgData(ushort usDestNodeCode, ushort usDestCopyNo, char* szDestProcName, ushort nReqMsg, ushort nResmsg
	, int nCnt, char* pData, int iDataSize)
{
	int nRet = 0;
//	char			szSrcNodeName[WEMS_MAX_NODE_NAME_STR_SZ];
//	char			szDestNodeName[WEMS_MAX_NODE_NAME_STR_SZ];

	if(!IsCreated())
		return WEMS_SOCK_CLOSE;
 
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
		return WEMS_SOCK_CLOSE;

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
	//wemsDataPacket_st*	RcvData;
	int					nRet = 0;

 	while(true)
 	{
 		dwRet = ::WaitForSingleObject( client_sock_.m_eventExit.GetEvent(), m_dwThreadWait );
 		if( dwRet  == WAIT_OBJECT_0 ) //이벤트 발생시
 		{
 			break;
 		}

		if(!client_sock_.IsCreated())
		{
			continue;
		}
			
 		memset(&RcvPacketData, 0x00, sizeof(wemsPacket_st));
 		if(client_sock_.SocketType() == SOCKET_CLIENT) // 클라이언트 소켓이면
 		{
 			nRet = ClientRecvPacket(&RcvPacketData);
 			if(nRet == WEMS_OK)
 			{
 				RcvPacketProc(&RcvPacketData);
 			}
 			else if(nRet < WEMS_OK)
 			{
 				client_sock_.Close(); // 소켓 종료
 			}
 		}
 		else if(client_sock_.SocketType() == SOCKET_SERVER)
 		{
 			nRet = ClientRecvPacket(&RcvPacketData);
 			if(nRet == 0)
 			{
 				RcvPacketProc(&RcvPacketData);
 			}
 			else if(nRet < WEMS_OK)
 			{
 				client_sock_.Close(); // 소켓 종료
 			}
 		}
 	}

 	WLOG("proc(%s:%d) rcv thread run exit\n"
 		, client_sock_.GetSrcProc()->stProcName.szProcName
 		, client_sock_.GetSrcProc()->stProcName.iCopy);

	return 0;
}

int SecClientSock::SecSockReceiver::RcvPacketProc(wemsPacket_st *pRcvPacket )
{
	wemsDataPacket_st*	RcvData;
	RcvData = (wemsDataPacket_st*) pRcvPacket->pData;
	wemsGPN_st	*stProcName = client_sock_.GetSrcProc();
	char			szNodeName[WEMS_MAX_NODE_NAME_STR_SZ] = {0,};

// 	if(*(ushort*)&stProcName->stNodeName == pRcvPacket->stPacketHeader.DestProc.usNodeCode &&
// 		stProcName->stProcName.iCopy == pRcvPacket->stPacketHeader.DestProc.usCopyNo &&
// 		strcmp(stProcName->stProcName.szProcName, pRcvPacket->stPacketHeader.DestProc.szProcName) == 0) // 자신의 것이면
// 	if(NODENAME_EQUAL((wemsNodeName_st*)&stProcName->stNodeName, (wemsNodeName_st*)&pRcvPacket->stPacketHeader.DestProc.usNodeCode) && 
// 		stProcName->stProcName.iCopy == pRcvPacket->stPacketHeader.DestProc.usCopyNo &&
// 		strcmp(stProcName->stProcName.szProcName, pRcvPacket->stPacketHeader.DestProc.szProcName) == 0)
// 	{
// 
// 	}


// 	if(*(ushort*)&stProcName->stNodeName == pRcvPacket->stPacketHeader.DestProc.usNodeCode &&
// 		stProcName->stProcName.iCopy == pRcvPacket->stPacketHeader.DestProc.usCopyNo &&
// 		strcmp(stProcName->stProcName.szProcName, pRcvPacket->stPacketHeader.DestProc.szProcName) == 0) // 자신의 것이면
	if(NODENAME_EQUAL((wemsNodeName_st*)&stProcName->stNodeName, (wemsNodeName_st*)&pRcvPacket->stPacketHeader.DestProc.usNodeCode) && 
		stProcName->stProcName.iCopy == pRcvPacket->stPacketHeader.DestProc.usCopyNo &&
		strcmp(stProcName->stProcName.szProcName, pRcvPacket->stPacketHeader.DestProc.szProcName) == 0)
	{
		if(RcvData->stDataHeader.usRequestFc == FC_PROC_REG_REQS)
		{
			// 1. 송신한 클라이언트가 즉 SRC가 자신의 노드 또는 BACKUP이면 허용
			// 2. 
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
				client_sock_.Close(); // 소켓 종료
			}

			free(pRcvPacket->pData); 
		}
		else
		{
			// 처리 쓰레드에 전송
			//SecClientSock::SecSockWorker* pThread = client_sock_.GetDataProcThread();
			CubeBoxThread* workThread = client_sock_.GetDataProcThread();
			if(workThread != NULL)
			{
				// 처리쓰레드에서 메모리 해제
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
	}
	else // 자신의 노드가 아니면
	{
		// 처리 안함(버림) -- 나오면 안됨.
		//WLOG("rcv data not proc (own proc <> dest proc)\n");
		// 처리 쓰레드에 전송
		//SecClientSock::SecSockWorker* pThread = client_sock_.GetDataProcThread();
		CubeBoxThread* workThread = client_sock_.GetDataProcThread();
		if(workThread != NULL)
		{
			// 처리쓰레드에서 메모리 해제
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

//	char			szSrcNodeName[WEMS_MAX_NODE_NAME_STR_SZ];
//	char			szDestNodeName[WEMS_MAX_NODE_NAME_STR_SZ];

	int					iMsgCode = 0;
	try
	{
		if(!client_sock_.IsCreated())		// 소켓이 ...
			return WEMS_SOCK_CLOSE;

		int nReceivedSize = 0;
		nReceivedSize = client_sock_.sock_RecvMsg((char*)&pRcvPacket->stPacketHeader, WEMSTCP_HEADER_SZ, WEMSTCP_RCV_TIMEOUT); // 최초 수신시 헤더파일을 읽는다.
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
						// 문제있음.
						WLOG("rcv socket recevied size error");
						return WEMS_SOCK_RCV_SIZE_ERR;
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

 					WLOG("src(%s:%02d) --> dest(%s:%02d) code(0x%0x) size(%d) idx(%d/%d) data rcv suc\n"
 						, pRcvPacket->stPacketHeader.SrcProc.szProcName
 						, pRcvPacket->stPacketHeader.SrcProc.usCopyNo
 						, pRcvPacket->stPacketHeader.DestProc.szProcName
 						, pRcvPacket->stPacketHeader.DestProc.usCopyNo
 						, iMsgCode
 						, pRcvPacket->stPacketHeader.usPktLength
 						, pRcvPacket->stPacketHeader.usPktIdx
 						, pRcvPacket->stPacketHeader.usTotPktCnt);
				}
				else
				{
					// 문제있음.
					WLOG("socket recevied size error");
					return WEMS_SOCK_RCV_SIZE_ERR;
				}
				usPacketIndex++;
			} while (pRcvPacket->stPacketHeader.usPktIdx < usPacketCount);

			pRcvPacket->stPacketHeader.usPktLength = usPacketDataSize;
		}
		else
		{
			// 문제있음.
			WLOG("socket recevied size error");
			return WEMS_SOCK_RCV_SIZE_ERR;
		}
	}
	catch(CWSocketException& ex)
	{
		if(ex.GetErrorCode() != WSAETIMEDOUT)
		{
			//프로그램 종료
			WLOG("proc(%s:%02d) rcv socket closed event(err:%d)\n"
				, client_sock_.GetSrcProc()->stProcName.szProcName
				, client_sock_.GetSrcProc()->stProcName.iCopy
				, ex.GetErrorCode());
			nRet = WEMS_SOCK_CLOSE; // 소켓 클로즈 에러
		}
		else
		{
			nRet = WEMS_SOCK_TIMEOUT; // 소켓 타임아웃 에러 (정상)
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
		if( dwRet  == WAIT_OBJECT_0 ) //이벤트 발생시
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
					if(iRet == WEMS_OK)
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

	WLOG("proc(%s:%d) proc thread run exit\n"
		, client_sock_.GetSrcProc()->stProcName.szProcName
		, client_sock_.GetSrcProc()->stProcName.iCopy);

	return 0;
}