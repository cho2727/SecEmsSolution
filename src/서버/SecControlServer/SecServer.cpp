#include "StdAfx.h"
#include "SecServer.h"
#include "SecDataProc.h"


int __stdcall ClientDataProc(wemsGPN_st* stSrcProc, wemsGPN_st* stDestProc, wemsDataPacket_st* pRcvData)
{
	SecDataProc::GetInstance()->PacketDataProc(stSrcProc, stDestProc, pRcvData);
	return 0;
}

int __stdcall SocketAcceptProc(SOCKET client_socket)
{
	wemsGPN_st* ownProc = SECSERVER->GetOwnProc();
	SecClientSock* client = new SecClientSock(client_socket, *(ushort*)&ownProc->stNodeName, ownProc->stProcName.iCopy, ownProc->stProcName.szProcName);
	client->ClientInit(ClientDataProc);
	client->Activate();

	// 클라이언트 감시 추가
	SECSERVER->AddClient(client);
	return 0;
}

SecServer::SecServer(void)
	: server_sock_()
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
	server_sock_.Init(39702, SocketAcceptProc);

	server_sock_.Activate();
	while( (dwWait=::WaitForSingleObject(service_handle_, /*INFINITE*/MainThreadWait)) != WAIT_OBJECT_0 ) 
	{
		if(dwWait == WAIT_TIMEOUT)
		{
			// 실제 구현부
			CheckClient();
		}
	}

	server_sock_.Abort();
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
	strcpy_s(this->ownProc_.stProcName.szProcName, szProcName);
}

void SecServer::AddClient(SecClientSock* client)
{
	client_list_.push_back(client);
}


void SecServer::DelClient()
{
	ClientList::iterator it = client_list_.begin();

	while(it != client_list_.end())
	{
		(*it)->Abort();
		delete *it;
		it = client_list_.erase(it); // 확인 필요
	}
}

SecClientSock*	SecServer::GetClient(ushort usNodeCode, ushort usCopyNo, const char* szProcName)
{
	SecClientSock* clientSock = NULL;
	ClientList::iterator it;
	for(it = client_list_.begin(); it != client_list_.end(); it++)
	{
		wemsGPN_st	*stProcName = (*it)->GetDestProc();

		if(NODENAME_EQUAL(&stProcName->stNodeName, (wemsNodeName_st*)&usNodeCode) &&
			stProcName->stProcName.iCopy == usCopyNo &&
			strcmp(stProcName->stProcName.szProcName, szProcName) == 0)
		{
			clientSock = (*it);
			return clientSock;
		}

		WLOG("NODE NAME : %s:%02d\n", stProcName->stProcName.szProcName, stProcName->stProcName.iCopy);
	}

	WLOG("FIND NODE NAME : %s:%02d Not Found(cnt:%d)\n", szProcName, usCopyNo, client_list_.size());

	return NULL;
}


void SecServer::CheckClient()
{
	ClientList::iterator it = client_list_.begin();
	
	try
	{
		while(it != client_list_.end())
		{
			if((*it) == NULL)
				continue;

			wemsGPN_st	*stProcName = (*it)->GetDestProc();
			if(stProcName == NULL)
				continue;


			if(!(*it)->IsCreated())
			{
				(*it)->Abort();
				WLOG("proc(%s:%d) client socket close\n"
					, stProcName->stProcName.szProcName
					, stProcName->stProcName.iCopy);

				delete *it;

				it = client_list_.erase(it);
			}
			else
				it++;
		}
	}
	catch (...)
	{
		WLOG("msgmd client check exception\n");
	}
}