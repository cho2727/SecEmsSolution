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
			int* pData = (int*)&pRcvData->pData;
			for(int i=0; i<pRcvData->stDataHeader.usCount; i++)
				WLOG("PROC(%s:%d) RCV DATA(%d) RCV\n", stSrcProc->stProcName.szProcName, stSrcProc->stProcName.iCopy, *pData);
		}
		break;

	case FC_PROC_STOP_REQS:
		break;

	case FC_PROC_LOG_CHANGE:
		break;

		//------	ȯ�� ���� ��ε�
	case FC_PROC_RELOAD_CONFIG	:
		break;

	case FC_NODE_RANK_CHANGE:
		break;

	default:
		break;
	}

	return 0;
}
