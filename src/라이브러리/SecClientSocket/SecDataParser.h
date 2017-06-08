#pragma once

#include "thread/CubeData.h"

class SecDataParser
{
public:
	SecDataParser(void);
	~SecDataParser(void);

	static int		GetSecDataList(CubeData::CubeDataVec& pWemsDataList
		, unsigned short usSrcNodeCode, unsigned short usSrcCopyNo, char* szSrcProcName
		, unsigned short usDestNodeCode, unsigned short usDestCopyNo, char* szDestProcName
		, unsigned short nReqMsg
		, unsigned short nResmsg
		, int nCnt
		, char* pData
		, int iDataSize);

	static int		MakeSecDataList(CubeData::CubeDataVec& pWemsDataList
		, unsigned short usSrcNodeCode, unsigned short usSrcCopyNo, char* szSrcProcName
		, unsigned short usDestNodeCode, unsigned short usDestCopyNo, char* szDestProcName
		, char* pData, int iDataSize);

};

