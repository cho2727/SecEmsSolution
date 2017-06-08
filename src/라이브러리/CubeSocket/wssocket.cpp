#include <winsock2.h>
#include <Windows.h>
#include "socket/wssocket.h"
#include <assert.h>     /* assert */
//#include <stdexcept>
#include <atlconv.h>

#pragma comment(lib, "ws2_32.lib")

CWSocketException::CWSocketException(int nError) : m_nError(nError)
{
}


int CWSocketException::GetErrorCode()
{
	return m_nError;
}




wssocket::wssocket(void) : m_hSocket(INVALID_SOCKET)
{
}


wssocket::~wssocket(void)
{
	if (m_hSocket != INVALID_SOCKET)
		Close();
}
// 
// void wssocket::ThrowWSocketException(int nError)
// {
// 	if (nError == 0)
// 		nError = ::WSAGetLastError();
// 
// 	CWSocketException* pException = new CWSocketException(nError);
// 
// 	TRACE(_T("Warning: throwing CWSocketException for error %d\n"), nError);
// 	THROW(pException);
// }


void    wssocket::Attach(SOCKET hSocket)
{
	if (m_hSocket != INVALID_SOCKET)
		Close();

	m_hSocket = hSocket;
}

SOCKET  wssocket::Detach()
{
	SOCKET socket = m_hSocket;
	m_hSocket = INVALID_SOCKET;
	return socket;
}

void    wssocket::GetPeerName(char* sPeerAddress, UINT& nPeerPort)
{
	if(IsCreated())
	{
		SOCKADDR_IN sockAddr;
		memset(&sockAddr, 0, sizeof(sockAddr));

		int nSockAddrLen = sizeof(sockAddr);
		GetPeerName(reinterpret_cast<SOCKADDR*>(&sockAddr), &nSockAddrLen);
		nPeerPort = ntohs(sockAddr.sin_port);
		sPeerAddress = inet_ntoa(sockAddr.sin_addr);
	}
}

void    wssocket::GetPeerName(SOCKADDR* lpSockAddr, int* lpSockAddrLen)
{
	//Validate our parameters
	assert(IsCreated()); //Must have been created first

	if (getpeername(m_hSocket, lpSockAddr, lpSockAddrLen) == SOCKET_ERROR)
		throw CWSocketException(::WSAGetLastError());
}

void    wssocket::GetSockName(char* sSocketAddress, UINT& nSocketPort)
{
	//Validate our parameters
	assert(IsCreated()); //must have been created first

	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	GetSockName(reinterpret_cast<SOCKADDR*>(&sockAddr), &nSockAddrLen);
	nSocketPort = ntohs(sockAddr.sin_port);
	strcpy(sSocketAddress, inet_ntoa(sockAddr.sin_addr));
	//sSocketAddress = inet_ntoa(sockAddr.sin_addr);
}
void    wssocket::GetSockName(SOCKADDR* lpSockAddr, int* lpSockAddrLen)
{
	//Validate our parameters
	assert(IsCreated()); //Must have been created first

	if (getsockname(m_hSocket, lpSockAddr, lpSockAddrLen) == SOCKET_ERROR)
		throw CWSocketException(::WSAGetLastError());
}
void    wssocket::SetSockOpt(int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel/* = SOL_SOCKET*/)
{
	//Validate our parameters
	assert(IsCreated()); //Must have been created first

	if(setsockopt(m_hSocket, nLevel, nOptionName, static_cast<LPCSTR>(lpOptionValue), nOptionLen) == SOCKET_ERROR)
		throw CWSocketException(::WSAGetLastError());
}
void    wssocket::GetSockOpt(int nOptionName, void* lpOptionValue, int* lpOptionLen, int nLevel/* = SOL_SOCKET*/)
{
	//Validate our parameters
	assert(IsCreated()); //Must have been created first

	if (getsockopt(m_hSocket, nLevel, nOptionName, static_cast<LPSTR>(lpOptionValue), lpOptionLen) == SOCKET_ERROR)
		throw CWSocketException(::WSAGetLastError());
}

BOOL wssocket::IsCreated() const
{
	return (m_hSocket != INVALID_SOCKET);
}

BOOL    wssocket::IsReadible(DWORD dwTimeout)
{
	assert(IsCreated()); //Must have been created first
	timeval timeout;
	timeout.tv_sec = dwTimeout/1000;
	timeout.tv_usec = (dwTimeout%1000)*1000;
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(m_hSocket, &fds);
	int nStatus = select(0, &fds, NULL, NULL, &timeout);
	if (nStatus == SOCKET_ERROR)
		throw CWSocketException(::WSAGetLastError());

	return !(nStatus == 0);
}
BOOL    wssocket::IsWritable(DWORD dwTimeout)
{
	assert(IsCreated()); //must have been created first

	timeval timeout;
	timeout.tv_sec = dwTimeout/1000;
	timeout.tv_usec = (dwTimeout%1000)*1000;
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(m_hSocket, &fds);
	int nStatus = select(0, NULL, &fds, NULL, &timeout);
	if (nStatus == SOCKET_ERROR)
		throw CWSocketException(::WSAGetLastError());

	return !(nStatus == 0);
}

//Methods
void    wssocket::Create(BOOL bUDP/* = FALSE*/)
{
	//Validate our parameters
	assert(!IsCreated()); //must not have been already created

	if (bUDP)
		Create(SOCK_DGRAM, 0, PF_INET);
	else
		Create(SOCK_STREAM, 0, PF_INET);
}

void    wssocket::Create(int nSocketType, int nProtocolType, int nAddressFormat)
{
	//Validate our parameters
	assert(!IsCreated()); //must not have been already created

	m_hSocket = socket(nAddressFormat, nSocketType, nProtocolType);
	if (m_hSocket == INVALID_SOCKET)
		throw CWSocketException(::WSAGetLastError());
}

void    wssocket::CreateEx()
{
	if ((m_hSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED))	== INVALID_SOCKET) {
			throw CWSocketException(::WSAGetLastError());
	}
}

void    wssocket::Accept(wssocket& connectedSocket, sockaddr_in& clientAddress)
{
	assert(IsCreated());               //must have been created first
	assert(!connectedSocket.IsCreated()); //Must be an unitialized socket

	//Call the SDK accept function  
	int nSize = sizeof(sockaddr_in);
	SOCKET socket = accept(m_hSocket, reinterpret_cast<sockaddr*>(&clientAddress), &nSize);
	if (socket == INVALID_SOCKET)
		throw CWSocketException(::WSAGetLastError());

	//Wrap the return value up into a C++ instance
	connectedSocket.Attach(socket);
}

void    wssocket::Accept(SOCKET& connectedSocket, sockaddr_in& clientAddress)
{
	assert(IsCreated());               //must have been created first

	//Call the SDK accept function  
	int nSize = sizeof(sockaddr_in);
	connectedSocket = accept(m_hSocket, reinterpret_cast<sockaddr*>(&clientAddress), &nSize);
	if (connectedSocket == INVALID_SOCKET)
		throw CWSocketException(::WSAGetLastError());
}

void    wssocket::Bind(UINT nSocketPort, LPSTR lpszSocketAddress/* = NULL*/)
{
	//USES_CONVERSION;

	//Setup the structure used in sdk "bind" calls
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(static_cast<u_short>(nSocketPort));

	//Do we need to bind to a specific IP address?
	if (lpszSocketAddress)
	{
		//Convert to an ASCII string
		//LPSTR lpszAscii = T2A(const_cast<LPTSTR>(lpszSocketAddress));
		sockAddr.sin_addr.s_addr = inet_addr(lpszSocketAddress);

		//If the address is not dotted notation, then do a DNS 
		//lookup of it.
		if (sockAddr.sin_addr.s_addr == INADDR_NONE)
		{
			LPHOSTENT lphost;
			lphost = gethostbyname(lpszSocketAddress);
			if (lphost != NULL)
				sockAddr.sin_addr.s_addr = (reinterpret_cast<LPIN_ADDR>(lphost->h_addr))->s_addr;
			else
			{
				throw CWSocketException(::WSAGetLastError());
				return;
			}
		}
	}
	else
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY); //Bind to any IP address;

	Bind(reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(sockAddr));
}

void    wssocket::Bind(const SOCKADDR* lpSockAddr, int nSockAddrLen)
{
	//Validate our parameters
	assert(IsCreated()); //Must have been created first

	if (bind(m_hSocket, lpSockAddr, nSockAddrLen) == SOCKET_ERROR)
		throw CWSocketException(::WSAGetLastError());

}

void wssocket::Close()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
}
void    wssocket::Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen)
{
	//Validate our parameters
	assert(IsCreated()); //must have been created first

	if (connect(m_hSocket, lpSockAddr, nSockAddrLen) == SOCKET_ERROR)
		throw CWSocketException(::WSAGetLastError());
}

void    wssocket::Connect(LPSTR lpszHostAddress, UINT nHostPort)
{

	//USES_CONVERSION;

	//Validate our parameters
	assert(IsCreated());             //must have been created first
	assert(lpszHostAddress);          //Must have a valid host

	//Work out the IP address of the machine we want to connect to
	//LPSTR lpszAscii = T2A(const_cast<LPTSTR>(lpszHostAddress));

	//Determine if the address is in dotted notation
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(static_cast<u_short>(nHostPort));
	sockAddr.sin_addr.s_addr = inet_addr(lpszHostAddress);

	//If the address is not dotted notation, then do a DNS 
	//lookup of it.
	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost = gethostbyname(lpszHostAddress);
		if (lphost != NULL)
			sockAddr.sin_addr.s_addr = (reinterpret_cast<LPIN_ADDR>(lphost->h_addr))->s_addr;
		else
			throw CWSocketException(::WSAGetLastError());
	}

	//Call the other version of Connect which does the actual work
	Connect(reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(sockAddr));
}

//Connect methods which have a timeout parameter are only provided if we are using WinSock2
//because we are making use of Winsock2 functionality such as WSAEventSelect
void    wssocket::Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen, DWORD dwConnectionTimeout, BOOL bResetToBlockingMode/* = TRUE*/)
{
	//Create an event to wait on
	WSAEVENT hConnectedEvent = WSACreateEvent();
	if (hConnectedEvent == WSA_INVALID_EVENT)
		throw CWSocketException(::WSAGetLastError());

	//Setup event selection on the socket
	if (WSAEventSelect(m_hSocket, hConnectedEvent, FD_CONNECT) == SOCKET_ERROR)
	{
		//Hive away the last error
		DWORD dwLastError = GetLastError();

		//Close the event before we return
		WSACloseEvent(hConnectedEvent);

		//Throw the exception that we could not setup event selection
		throw CWSocketException(::WSAGetLastError());
	}

	//Call the SDK "connect" function
	int nConnected = connect(m_hSocket, lpSockAddr, nSockAddrLen);
	if (nConnected == SOCKET_ERROR)
	{
		//Check to see if the call should be completed by waiting for the event to be signalled
		DWORD dwLastError = GetLastError();
		if (dwLastError == WSAEWOULDBLOCK)
		{
			DWORD dwWait = WaitForSingleObject(hConnectedEvent, dwConnectionTimeout);
			if (dwWait == WAIT_OBJECT_0)
			{
				//Get the error value returned using WSAEnumNetworkEvents
				WSANETWORKEVENTS networkEvents;
				int nEvents = WSAEnumNetworkEvents(m_hSocket, hConnectedEvent, &networkEvents);
				if (nEvents == SOCKET_ERROR)
				{
					//Hive away the last error
					DWORD dwLastError = GetLastError();

					//Close the event before we return
					WSACloseEvent(hConnectedEvent);

					//Throw the exception that we could not call WSAEnumNetworkEvents
					throw CWSocketException(::WSAGetLastError());
				}
				else
				{
					assert(networkEvents.lNetworkEvents & FD_CONNECT);

					//Has an error occured in the connect call
					if (networkEvents.iErrorCode[FD_CONNECT_BIT] != ERROR_SUCCESS)
					{
						//Close the event before we return
						WSACloseEvent(hConnectedEvent);

						//Throw the exception that an error has occurred in calling connect
						throw CWSocketException(::WSAGetLastError());
					}
				}
			}
			else
			{
				//Close the event before we return
				WSACloseEvent(hConnectedEvent);

				//Throw the exception that we could not connect in a timely fashion
				throw CWSocketException(::WSAGetLastError());
			}
		}
		else
		{
			//Close the event before we return
			WSACloseEvent(hConnectedEvent);

			//Throw the exception that the connect call failed unexpectedly
			throw CWSocketException(::WSAGetLastError());
		}
	}

	//Remove the event notification on the socket
	WSAEventSelect(m_hSocket, hConnectedEvent, 0);

	//Destroy the event now that we are finished with it
	WSACloseEvent(hConnectedEvent);

	//Reset the socket to blocking mode if required
	if (bResetToBlockingMode)
	{
		DWORD dwNonBlocking = 0;
		if (ioctlsocket(m_hSocket, FIONBIO, &dwNonBlocking) == SOCKET_ERROR)
		{
			//Throw the exception that we could not reset the socket to blocking mode
			throw CWSocketException(::WSAGetLastError());
		}
	}    
}

void    wssocket::Connect(LPSTR lpszHostAddress, UINT nHostPort, DWORD dwConnectionTimeout, BOOL bResetToBlockingMode/* = TRUE*/)
{
	//USES_CONVERSION;

	//Validate our parameters
	assert(IsCreated());             //must have been created first
	assert(lpszHostAddress);          //Must have a valid host

	//Work out the IP address of the machine we want to connect to
	//LPSTR lpszAscii = T2A(const_cast<LPTSTR>(lpszHostAddress));

	//Determine if the address is in dotted notation
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(static_cast<u_short>(nHostPort));
	sockAddr.sin_addr.s_addr = inet_addr(lpszHostAddress);

	//If the address is not dotted notation, then do a DNS 
	//lookup of it.
	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost = gethostbyname(lpszHostAddress);
		if (lphost != NULL)
			sockAddr.sin_addr.s_addr = (reinterpret_cast<LPIN_ADDR>(lphost->h_addr))->s_addr;
		else
			throw CWSocketException(::WSAGetLastError());
	}

	//Call the other version of Connect which does the actual work
	Connect(reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(sockAddr), dwConnectionTimeout, bResetToBlockingMode);
}


void    wssocket::IOCtl(long lCommand, DWORD* lpArgument)
{
	//Validate our parameters
	assert(IsCreated()); //must have been created first

	if (ioctlsocket(m_hSocket, lCommand, lpArgument) == SOCKET_ERROR)
		throw CWSocketException(::WSAGetLastError());
}

void    wssocket::Listen(int nConnectionBacklog/* = SOMAXCONN*/)
{
	//Validate our parameters
	assert(IsCreated()); //must have been created first

	if (listen(m_hSocket, nConnectionBacklog) == SOCKET_ERROR)
		throw CWSocketException(::WSAGetLastError());
}

int     wssocket::Receive(void* lpBuf, int nBufLen, int nFlags/* = 0*/)
{
	//Validate our parameters
	assert(IsCreated()); //must have been created first

	int nReceived = recv(m_hSocket, static_cast<LPSTR>(lpBuf), nBufLen, nFlags); 
	if (nReceived == SOCKET_ERROR)
		throw CWSocketException(::WSAGetLastError());
	
	return nReceived;
}

int     wssocket::ReceiveFrom(void* lpBuf, int nBufLen, SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags/* = 0*/)
{
	//Validate our parameters
	assert(IsCreated()); //must have been created first

	int nReceived = recvfrom(m_hSocket, static_cast<LPSTR>(lpBuf), nBufLen, nFlags, lpSockAddr, lpSockAddrLen);
	if (nReceived == SOCKET_ERROR)
		throw CWSocketException(::WSAGetLastError());

	return nReceived;
}

int     wssocket::ReceiveFrom(void* lpBuf, int nBufLen, char* sSocketAddress, UINT& nSocketPort, int nFlags/* = 0*/)
{
	//Validate our parameters
	assert(IsCreated()); //must have been created first

	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	int nResult = ReceiveFrom(lpBuf, nBufLen, reinterpret_cast<SOCKADDR*>(&sockAddr), &nSockAddrLen, nFlags);
	nSocketPort = ntohs(sockAddr.sin_port);
	sSocketAddress = inet_ntoa(sockAddr.sin_addr);
	return nResult;
}

int     wssocket::Send(const void* pBuffer, int nBufLen, int nFlags/* = 0*/)
{
	assert(IsCreated()); //must have been created first

	int nSent = send(m_hSocket, static_cast<const char*>(pBuffer), nBufLen, nFlags);
	if (nSent == SOCKET_ERROR)
		throw CWSocketException(::WSAGetLastError());

	return nSent;
}

int     wssocket::SendTo(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags/* = 0*/)
{
	//Validate our parameters
	assert(IsCreated()); //must have been created first

	int nSent = sendto(m_hSocket, static_cast<const char*>(lpBuf), nBufLen, nFlags, lpSockAddr, nSockAddrLen);
	if (nSent == SOCKET_ERROR)
		throw CWSocketException(::WSAGetLastError());

	return nSent; 
}

int     wssocket::SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPSTR lpszHostAddress/* = NULL*/, int nFlags/* = 0*/)
{
	//Validate our parameters
	assert(IsCreated()); //must have been created first

	//USES_CONVERSION;

	//Determine if the address is in dotted notation
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons((u_short)nHostPort);

	if (lpszHostAddress == NULL)
		sockAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	else
	{
		//If the address is not dotted notation, then do a DNS 
		//lookup of it.
		//LPSTR lpszAscii = T2A(const_cast<LPTSTR>(lpszHostAddress));
		sockAddr.sin_addr.s_addr = inet_addr(lpszHostAddress);
		if (sockAddr.sin_addr.s_addr == INADDR_NONE)
		{
			LPHOSTENT lphost = gethostbyname(lpszHostAddress);
			if (lphost != NULL)
				sockAddr.sin_addr.s_addr = (reinterpret_cast<LPIN_ADDR>(lphost->h_addr))->s_addr;
			else
				throw CWSocketException(::WSAGetLastError());
		}
	}

	return SendTo(lpBuf, nBufLen, reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(sockAddr), nFlags);
}

void    wssocket::ShutDown(int nHow/* = sends*/)
{
	//Validate our parameters
	assert(IsCreated()); //must have been created first

	if (shutdown(m_hSocket, nHow) == SOCKET_ERROR)
		throw CWSocketException(::WSAGetLastError());
}

wssocket::operator SOCKET()
{
	return m_hSocket;
}