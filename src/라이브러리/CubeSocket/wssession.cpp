
#include <winsock2.h>
#include <Windows.h>
#include "socket/wssession.h"
#include "sec/SecErrorCode.h"
//#include "common/wscommtypes.h"
#include <time.h>

wssession::wssession(void)// : m_bConnected(false)
{
}


wssession::~wssession(void)
{
	sock_Uninit();
}


int		wssession::sock_Init(char* lpszHostAddress, UINT nHostPort, DWORD dwTimeout)
{
	if(IsCreated())
		return SEC_OK;

	try
	{
		Create();		// 소켓 생성
		Connect(lpszHostAddress, nHostPort, dwTimeout);
	}
	catch(CWSocketException& ex)
	{
		return ex.GetErrorCode();
	}
	catch(std::exception& ex)
	{
		return SEC_SOCK_EXCEPTION;
	}

	//m_bConnected = true;
	return  SEC_OK;
}

void	wssession::sock_Uninit()
{
	Close();
	//m_bConnected = false;
}

int		wssession::sock_SendMsg(const void* pBuffer, int nBufLen, DWORD dwTimeout)
{
	int nSentSize = 0;

	do 
	{
		try
		{
			if(IsCreated())
			{
				nSentSize += Send(static_cast<const BYTE*>(pBuffer) + nSentSize, nBufLen - nSentSize);
			}
			else
				throw CWSocketException(SESSION_INVALID_SOCKET);
		}
		catch(CWSocketException& ex)
		{
			int nError = ex.GetErrorCode();
			//WLOG(LOG_LEVEL_INFO, "SEND ERROR : %d\n", nError);
			if(nError == WSAEWOULDBLOCK)
			{
				int nEvent = ::WSAEventSelect(*this, m_WSABlockEvent.GetEvent(), FD_WRITE);

				if(nEvent != 0)
					throw CWSocketException(::WSAGetLastError());

				DWORD dwWait = WaitForSingleObject(m_WSABlockEvent.GetEvent(), dwTimeout);

				WSAEventSelect(*this, m_WSABlockEvent.GetEvent(), 0);

				if (dwWait != WAIT_OBJECT_0) // 타임아웃
					throw CWSocketException(::WSAGetLastError());
			}
			else
			{
				if(ex.GetErrorCode() == SESSION_INVALID_SOCKET)
					throw CWSocketException(SESSION_INVALID_SOCKET);
				else
					throw CWSocketException(::WSAGetLastError());
			}
		}
	} while (nSentSize < nBufLen);


	return  nSentSize;
}

void	wssession::sock_Flushing()
{
	return;
	char	*buff;
	u_long	rlen = 0; // u_long -> DWORD

	try
	{
		IOCtl(FIONREAD, &rlen);
	}
	catch(CWSocketException& ex)
	{
		return;
	}

	if(rlen <= 0)
		return;

	buff = (char *)malloc( rlen );
	if( buff == NULL )
		return;

	try
	{
		sock_RecvMsg(buff, rlen, 1);
		free(buff);
	}
	catch(CWSocketException& ex)
	{
		
	}
}

#if 0
int		wssession::sock_SendMsg(BYTE bNodeCode, BYTE bActCode, BYTE bCompCode, ushort nReqMsg, ushort nResmsg, int nCnt, char* pData, int iDataSize)
{
	int                 iRetVal;
	int					nPacketSize = WEMSDATA_HEADER_SZ+iDataSize;
	char				*pSendData = (char*) malloc(nPacketSize);

	wemsDataHeader_st   stPacketData;
	memset(&stPacketData, 0x00, sizeof(wemsDataHeader_st));
	setDataHeader(&stPacketData, nReqMsg, nResmsg, nCnt);

	memset(pSendData, 0x00, nPacketSize);
	memcpy(pSendData, &stPacketData, WEMSDATA_HEADER_SZ);
	memcpy(pSendData+WEMSDATA_HEADER_SZ, pData, iDataSize);

	iRetVal = SendPacket(bNodeCode, bActCode, bCompCode, pSendData, nPacketSize);
	if(iRetVal != 0)
	{
		free(pSendData);
		return -1;
	}

	free(pSendData);
	return 0;
}

int wssession::MakeResultHeader(wemsPacketHeader_st *SendHeader, 
	BYTE node_code, 
	BYTE action_code, 
	ushort tot_packet_cnt, 
	ushort packet_idx, 
	BYTE sequence, 
	BYTE compress, 
	ushort length )
{
	memset(SendHeader, 0x00, sizeof(wemsPacketHeader_st));

	/* 공통 부분 */
	SendHeader->ucNodeCode = node_code;
	SendHeader->ucActCode = action_code;
	SendHeader->usTotPktCnt = tot_packet_cnt;
	SendHeader->usPktIdx = packet_idx;
	SendHeader->ucSequence = sequence;
	SendHeader->ucCompress = compress;
	SendHeader->usPktLength = length;

	return 0;
}



int wssession::SendPacket(BYTE node_code, BYTE act_code, BYTE compress, char* pData, int iDataSize)
{
	int				iRetVal;	
	wemsPacket_st	stSndPacket;

	int				outValue;
	ushort			usTotalPktCnt;
	ushort			usPktIdx = 1;
	int				iLength;
	char			*pDataOffset = NULL;
	char			stSndData[WEMSTCP_PACKET_SZ];
	char 			*ptr = stSndData;
	int				nNam;

	usTotalPktCnt = iDataSize/WEMSTCP_DATA_SZ;

	nNam = iDataSize%WEMSTCP_DATA_SZ;
	if(nNam > 0)
		usTotalPktCnt++;

	if(usTotalPktCnt == 0)
		usTotalPktCnt = 1;

	memset(&stSndPacket, 0x00, sizeof(wemsPacket_st));

	if ( iDataSize > 0)
		pDataOffset = pData;

	while(usPktIdx <= usTotalPktCnt)
	{
		memset(&stSndData, 0x00, WEMSTCP_DATA_SZ);
		memset(&stSndPacket.stPacketHeader, 0x00, WEMSTCP_HEADER_SZ);

		/* 전송해야될 남은 data size */
		if (iDataSize > 0)
		{
			iLength = (pData + iDataSize) - pDataOffset;
		}
		else
			iLength = iDataSize;

		iLength = (iLength > WEMSTCP_DATA_SZ)? WEMSTCP_DATA_SZ:iLength ;

		MakeResultHeader(&stSndPacket.stPacketHeader
			, node_code
			, act_code
			, usTotalPktCnt
			, usPktIdx
			, 1//GetSequenceId()
			, compress
			, WEMSTCP_HEADER_SZ + iLength);

		memcpy(ptr, &stSndPacket.stPacketHeader, WEMSTCP_HEADER_SZ);

		if(iLength > 0)
		{
			memcpy(ptr + WEMSTCP_HEADER_SZ, pDataOffset, iLength);
		}

		if(sock_SendMsg((const void*)&stSndData, stSndPacket.stPacketHeader.usPktLength, 1000) != 0)
		{
			return -1;
		}

		if(pDataOffset != NULL)
			pDataOffset += iLength;

		usPktIdx++;
	}
	return 0;
}


int wssession::setDataHeader(wemsDataHeader_st* stDataHeader, ushort nReqMsg, ushort nResmsg, int cnt)
{
	stDataHeader->usCount = cnt;
	stDataHeader->uiTime = time(NULL);
	stDataHeader->sRequestFc = nReqMsg;
	stDataHeader->sResqonseFc = nResmsg;
	return 0;
}
#endif



int		wssession::sock_RecvMsg(void* pBuffer, int nBufLen, DWORD dwTimeout)
{
	int		nDataReceived = 0;

	if(pBuffer != NULL)
	{
		do 
		{
			if(!IsReadible(dwTimeout))
			{
				throw CWSocketException(WSAETIMEDOUT);
			}

			try
			{
				nDataReceived += Receive(pBuffer, nBufLen - nDataReceived);
				if(nDataReceived == 0)
				{
					throw CWSocketException(::WSAGetLastError());
				}
			}
			catch(CWSocketException& ex)
			{
				int nError = ex.GetErrorCode();
				//WLOG(LOG_LEVEL_INFO, "RECV ERROR : %d\n", nError);
				if(nError == WSAEWOULDBLOCK)
				{
					int nEvent = ::WSAEventSelect(*this, m_WSABlockEvent.GetEvent(), FD_READ);

					if(nEvent != 0)
						throw CWSocketException(::WSAGetLastError());

					DWORD dwWait = WaitForSingleObject(m_WSABlockEvent.GetEvent(), dwTimeout);

					WSAEventSelect(*this, m_WSABlockEvent.GetEvent(), 0);

					if (dwWait != WAIT_OBJECT_0) // 타임아웃
						throw CWSocketException(WSAETIMEDOUT);
				}
				else
					throw CWSocketException(::WSAGetLastError());
			}
		} while (nDataReceived < nBufLen);
	}

	return  nDataReceived;
}