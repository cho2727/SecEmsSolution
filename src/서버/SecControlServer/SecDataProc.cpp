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
	
	switch(pRcvData->stDataHeader.usRequestFc)
	{
	case FC_PROC_TEST_REQS:
		{
			WLOG("proc(%s:%02d) -> proc(%s:%02d) FC_PROC_TEST_REQS work...\n", stSrcProc->stProcName.szProcName, stSrcProc->stProcName.iCopy
				, stDestProc->stProcName.szProcName, stDestProc->stProcName.iCopy);
			SecClientSock* client_sock = SECSERVER->GetClient(*(ushort*)&stDestProc->stNodeName, stDestProc->stProcName.iCopy, stDestProc->stProcName.szProcName);
			if(client_sock)
			{
				wemsGPN_st* pData = (wemsGPN_st*)&pRcvData->pData;
				WLOG("DATA------------------%s:%02d\n", pData->stProcName.szProcName, pData->stProcName.iCopy);
				client_sock->SendMsgData(*(ushort*)&stDestProc->stNodeName, stDestProc->stProcName.iCopy, stDestProc->stProcName.szProcName, FC_PROC_TEST_REQS, FC_PROC_TEST_REQS, pRcvData->stDataHeader.usCount, (char*)pData, sizeof(wemsGPN_st));
			}
			else
			{
				WLOG("client is null..........\n");
			}
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
