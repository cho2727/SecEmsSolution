#include "StdAfx.h"
#include "SecServer.h"
#include "SecDataProc.h"
#include "sec/SecMessageCode.h"



int __stdcall PacketDataProc(wemsGPN_st* stSrcProc, wemsGPN_st* stDestProc, wemsDataPacket_st* pRcvData)
{
	SecDataProc::GetInstance()->PacketDataProc(stSrcProc, stDestProc, pRcvData);
	return 0;
}


SecServer::SecServer(void)
	: real_sock_(NULL)
{
	service_handle_  = ::CreateEvent( 0, TRUE, FALSE, 0 );
	memset(&ownProc_, 0x00, sizeof(wemsGPN_st));
}


SecServer::~SecServer(void)
{
}

SecServer*	SecServer::GetInstance( void )  
{
	static SecServer	_Instance;
	return &_Instance;
}


int			SecServer::ServiceInit(int nCopyNo, char* szConfigFile)
{
	int iRet = 0;
	WLOG("[SVR] %s Program Start\n", OWNPROCNAME);
	SetOwnProc(0, nCopyNo, OWNPROCNAME);

	return iRet;
}

int			SecServer::ServiceRun()
{
	int iRet = 0;
	DWORD dwWait;
	SocketInit sec_sock;
	while( (dwWait=::WaitForSingleObject(service_handle_, /*INFINITE*/MainThreadWait)) != WAIT_OBJECT_0 ) 
	{
		if(dwWait == WAIT_TIMEOUT)
		{
			// 실제 구현부
			if(real_sock_ == NULL)
			{
				real_sock_ = new SecClientSock();
				iRet = real_sock_->ClientInit(*(ushort*)&this->ownProc_.stNodeName
					, this->ownProc_.stProcName.iCopy
					, this->ownProc_.stProcName.szProcName
					, "122.129.249.142", 39701, 1000, PacketDataProc);

				if(iRet != 0)
				{
					if(real_sock_)
					{
						delete real_sock_;
						real_sock_ = NULL;
						WLOG("[SVR] Server not connect(ret:%d)\n", iRet);
						continue;
					}
				}
				else
				{
					iRet = real_sock_->SendMsgData(*(ushort*)&this->ownProc_.stNodeName, SEC_ONLY_COPYNUM, "SecRealServer", FC_PROC_REG_REQS, FC_PROC_REG_REQS, 0, NULL, 0);
					if(iRet == 0)
					{
						WLOG("[SVR] Connect Succ\n");
					}
					else
					{
						WLOG("[SVR] Connect Fail\n");
					}
					real_sock_->Activate();
				}
			}
			else
			{
				WLOG("[SVR] Socket is connect(%d)\n", real_sock_->IsCreated());
				if(!real_sock_->IsCreated())
				{
					real_sock_->Abort();
					delete real_sock_;
					real_sock_ = NULL;
				}
				else
				{
					iRet = real_sock_->SendMsgData(*(ushort*)&this->ownProc_.stNodeName, this->ownProc_.stProcName.iCopy, this->ownProc_.stProcName.szProcName, FC_PROC_TEST_REQS, FC_PROC_TEST_REQS, 1, (char*)&ownProc_, sizeof(ownProc_));
				}
			}
		}
	}

	return iRet;
}
void		SecServer::ServiceDestroy()
{
	::CloseHandle(service_handle_);
}

void		SecServer::SetEvent()
{
	::SetEvent(service_handle_);
}


void SecServer::SetOwnProc(ushort usNodeCode, ushort usCopyNo, char* szProcName)
{
	this->ownProc_.stNodeName = *(wemsNodeName_st*)&usNodeCode;
	this->ownProc_.stProcName.iCopy = usCopyNo;
	strcpy(this->ownProc_.stProcName.szProcName, szProcName);
}
