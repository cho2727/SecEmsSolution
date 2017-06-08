#include "StdAfx.h"
#include "SecDataProc.h"


SecDataProc::SecDataProc(void)
{
}


SecDataProc::~SecDataProc(void)
{
}



SecDataProc* SecDataProc::GetInstance( void )  
{
	static SecDataProc	_Instance;
	return &_Instance;
}

int SecDataProc::PacketDataProc(wemsGPN_st* stSrcProc, wemsGPN_st* stDestProc, wemsDataPacket_st* pRcvData)
{	
	switch(pRcvData->stDataHeader.usRequestFc)
	{
	case FC_PROC_TEST_REQS:
		{
			WLOG("[SVR] FC_PROC_TEST_REQS Receive\n");
			wemsGPN_st* pData = (wemsGPN_st*)&pRcvData->pData;
			WLOG("DATA------------------%s:%02d\n", pData->stProcName.szProcName, pData->stProcName.iCopy);
		}
		break;

	case FC_PROC_STOP_REQS:
		break;

	case FC_PROC_LOG_CHANGE:
		break;

		//------	환경 파일 재로딩
	case FC_PROC_RELOAD_CONFIG	:
		break;

	case FC_NODE_RANK_CHANGE:
		break;

	default:
		break;
	}

	return 0;
}
