#include "SecDataParser.h"
#include "sec/SecCommTypes.h"
#include "log/CubeLog.h"
//#include "common/wsstring.h"
#include <time.h>
#include <sys/timeb.h>

tm* GetLocalTime(int &milisec)
{
	struct timeb timebuffer;
	time_t ltime;
	ftime(&timebuffer);
	ltime = timebuffer.time;
	milisec = timebuffer.millitm;
	return localtime(&ltime);
}

int MakePacketHeader(wemsPacketHeader_st *SendHeader, 
	unsigned short usSrcNodeCode, unsigned short usSrcCopyNo, char* szSrcProcName,
	unsigned short usDestNodeCode, unsigned short usDestCopyNo, char* szDestProcName,
	unsigned short tot_packet_cnt, 
	unsigned short packet_idx, 
	unsigned short length )
{
	memset(SendHeader, 0x00, sizeof(wemsPacketHeader_st));

	/* 공통 부분 */
	SendHeader->SrcProc.usNodeCode	= usSrcNodeCode;
	SendHeader->SrcProc.usCopyNo	= usSrcCopyNo;
	SendHeader->DestProc.usNodeCode	= usDestNodeCode;
	SendHeader->DestProc.usCopyNo	= usDestCopyNo;
	strcpy(SendHeader->SrcProc.szProcName, szSrcProcName);
	strcpy(SendHeader->DestProc.szProcName, szDestProcName);
	SendHeader->usTotPktCnt = tot_packet_cnt;
	SendHeader->usPktIdx = packet_idx;
	SendHeader->usPktLength = length;

	return 0;
}

int MakeDataHeader(wemsDataHeader_st* stDataHeader, ushort nReqMsg, ushort nResmsg, int cnt)
{
	int milisec;
	GetLocalTime(milisec);
	stDataHeader->usCount = cnt;
	stDataHeader->uiTime = (uint)time(NULL);
	//stDataHeader->uiMillitm = 111;
	stDataHeader->usRequestFc = nReqMsg;
	stDataHeader->usResqonseFc = nResmsg;
	return 0;
}

SecDataParser::SecDataParser(void)
{
}


SecDataParser::~SecDataParser(void)
{
}


int SecDataParser::GetSecDataList(CubeData::CubeDataVec& pWemsDataList
	, ushort usSrcNodeCode, ushort usSrcCopyNo, char* szSrcProcName
	, ushort usDestNodeCode, ushort usDestCopyNo, char* szDestProcName
	, ushort nReqMsg
	, ushort nResmsg
	, int nCnt
	, char* pData
	, int iDataSize)
{
	int                 iRetVal;
	int					nPacketSize = WEMSDATA_HEADER_SZ+(iDataSize*nCnt);
	char				*pSendData = (char*) malloc(nPacketSize);
	wemsDataHeader_st   stPacketData;

	memset(&stPacketData, 0x00, sizeof(wemsDataHeader_st));
	MakeDataHeader(&stPacketData, nReqMsg, nResmsg, nCnt); // 여기에서 셋팅

	memset(pSendData, 0x00, nPacketSize);
	memcpy(pSendData, &stPacketData, WEMSDATA_HEADER_SZ);
	memcpy(pSendData+WEMSDATA_HEADER_SZ, pData, (iDataSize*nCnt));

	iRetVal = MakeSecDataList(pWemsDataList, usSrcNodeCode, usSrcCopyNo, szSrcProcName, usDestNodeCode, usDestCopyNo, szDestProcName, pSendData, nPacketSize);
	if(iRetVal != 0)
	{
		free(pSendData);
		return -1;
	}

	free(pSendData);

	return 0;

}

int SecDataParser::MakeSecDataList(CubeData::CubeDataVec& pWemsDataList
	, ushort usSrcNodeCode, ushort usSrcCopyNo, char* szSrcProcName
	, ushort usDestNodeCode, ushort usDestCopyNo, char* szDestProcName
	, char* pData, int iDataSize)
{
	int				iRetVal;	
	ushort			usTotalPktCnt = 0;
	ushort			usPktIdx = 1;
	int				iLength;
	char			*pDataOffset = NULL;
	char			stSndData[WEMSTCP_PACKET_SZ];
	char 			*ptr = stSndData;

#if 0
	usTotalPktCnt = iDataSize/WEMSTCP_DATA_SZ;
	usTotalPktCnt++;
#else
	usTotalPktCnt = (iDataSize/WEMSTCP_DATA_SZ) + (((iDataSize % WEMSTCP_DATA_SZ) > 0) ? 1: 0);
#endif

	if ( iDataSize > 0 && pData != NULL)
		pDataOffset = pData;

	while(usPktIdx <= usTotalPktCnt)
	{
		memset(&stSndData, 0x00, WEMSTCP_PACKET_SZ);
		CubeData* pCubeData = new CubeData();
		pCubeData->SetProcInfo(usDestNodeCode, usDestCopyNo, szDestProcName);

		/* 전송해야될 남은 data size */
		if(pDataOffset != NULL && iDataSize > 0)		// iDataSize = 166 
		{
			iLength = iDataSize - (pDataOffset - pData);
		}
		else
			iLength = iDataSize;

		iLength = (iLength > WEMSTCP_DATA_SZ)? WEMSTCP_DATA_SZ:iLength ; //< 현재 IDX에서 보낼 데이터 사이즈

		MakePacketHeader((wemsPacketHeader_st*)ptr
			, usSrcNodeCode, usSrcCopyNo, szSrcProcName, usDestNodeCode, usDestCopyNo, szDestProcName
			, usTotalPktCnt
			, usPktIdx
			, WEMSTCP_HEADER_SZ + iLength);

		if(iLength > 0)
		{
			memcpy(ptr + WEMSTCP_HEADER_SZ, pDataOffset, iLength);
		}

		pCubeData->set_buffer(ptr, WEMSTCP_HEADER_SZ + iLength);
		pWemsDataList.push_back(pCubeData);

		if(pDataOffset != NULL)
			pDataOffset += iLength;

		usPktIdx++;
	}
	return 0;
}