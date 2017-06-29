#include "StdAfx.h"
#include "SecDataProc.h"
#include "SecServer.h"



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
	int iRet = 0;
	if(!(NODENAME_EQUAL(&SECSERVER->GetOwnProc()->stNodeName, (wemsNodeName_st*)&stDestProc->stNodeName) &&
		SECSERVER->GetOwnProc()->stProcName.iCopy == stDestProc->stProcName.iCopy &&
		strcmp(SECSERVER->GetOwnProc()->stProcName.szProcName, stDestProc->stProcName.szProcName) == 0))
	{
		SecClientSock* client_sock = SECSERVER->GetClient(*(ushort*)&stDestProc->stNodeName, stDestProc->stProcName.iCopy, stDestProc->stProcName.szProcName);
		if(client_sock)
		{
			int* pData = (int*)&pRcvData->pData;
			for(int i=0; i<pRcvData->stDataHeader.usCount; i++)
				WLOG("DATA(%d) RCV\n", *pData);

			iRet = client_sock->SendMsgData(*(ushort*)&stSrcProc->stNodeName, stSrcProc->stProcName.iCopy, stSrcProc->stProcName.szProcName
				, *(ushort*)&stDestProc->stNodeName, stDestProc->stProcName.iCopy, stDestProc->stProcName.szProcName
				, pRcvData->stDataHeader.usRequestFc, pRcvData->stDataHeader.usRequestFc, pRcvData->stDataHeader.usCount, (char*)pData, sizeof(int));
		}
		else
		{
			WLOG("client is null..........\n");
		}

		return iRet;
	}

	switch(pRcvData->stDataHeader.usRequestFc)
	{
	case FC_PROC_TEST_REQS:
		{
			
		}
		break;

	case FC_PROC_STOP_REQS:
		{

		}
		break;

	case FC_PROC_LOG_CHANGE:
		{

		}
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
