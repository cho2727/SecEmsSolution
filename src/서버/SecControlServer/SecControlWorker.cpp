#include "StdAfx.h"
#include "SecControlWorker.h"
#include "SecDataProc.h"


SecControlWorker::SecControlWorker(void)
{
}


SecControlWorker::~SecControlWorker(void)
{
}

void SecControlWorker::Activate()
{
	EventExit_.Reset();
	Start();
}

void SecControlWorker::Abort()
{
	EventExit_.Set();
	Wait();
}

int SecControlWorker::Run()
{
	DWORD				dwRet = 0x00;
	int					iRet = 0;

	while(true)
	{
		dwRet = ::WaitForSingleObject( EventExit_.GetEvent(), ProcThreadWait );
		if( dwRet  == WAIT_OBJECT_0 ) // 이벤트 발생시
		{
			break;
		}	

		try
		{
			CubeData* pData = NULL;
			if(this->MsgRecv(pData) == SEC_OK)	// 패킷 수신함.
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

				int iRet = SecDataProc::GetInstance()->PacketDataProc(&GpnSrcProc, &GpnDestProc, pDataPacket);
				if(iRet == SEC_OK)
				{
					// 성공
				}
				else
				{
					// 실패
				}

				if(pData)
				{
					delete pData;
					pData = NULL;
				}
			}
		}
		catch(...)
		{
			::Sleep(100);
		}
	}

	return iRet;
}
