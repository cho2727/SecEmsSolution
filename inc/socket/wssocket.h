#pragma once
#include <iostream>
//#include <exception>
#include <stdexcept>


#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif


class WELLS_Export CWSocketException : public std::exception
{
public:
	CWSocketException(int nError);

	int GetErrorCode();

// 	virtual BOOL GetErrorMessage(LPTSTR lpstrError, UINT nMaxError,	PUINT pnHelpContext = NULL);
// 	CString GetErrorMessage();

	int m_nError;
};



class WELLS_Export wssocket
{
public:
	wssocket(void);
	~wssocket(void);

	void    Attach(SOCKET hSocket);
	SOCKET  Detach(); 

	void    GetPeerName(char* sPeerAddress, UINT& nPeerPort);
	void    GetPeerName(SOCKADDR* lpSockAddr, int* lpSockAddrLen);

	void    GetSockName(char* sSocketAddress, UINT& nSocketPort);
	void    GetSockName(SOCKADDR* lpSockAddr, int* lpSockAddrLen);
	void    SetSockOpt(int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel = SOL_SOCKET);
	void    GetSockOpt(int nOptionName, void* lpOptionValue, int* lpOptionLen, int nLevel = SOL_SOCKET);
	BOOL    IsCreated() const; 
	BOOL    IsReadible(DWORD dwTimeout);
	BOOL    IsWritable(DWORD dwTimeout);

	//Methods
	void    Create(BOOL bUDP = FALSE);
	void    Create(int nSocketType, int nProtocolType, int nAddressFormat);
	void    CreateEx();
	void    Accept(wssocket& connectedSocket, sockaddr_in& clientAddress);
	void    Accept(SOCKET& connectedSocket, sockaddr_in& clientAddress);
	void    Bind(UINT nSocketPort, LPSTR lpszSocketAddress = NULL);
	void    Bind(const SOCKADDR* lpSockAddr, int nSockAddrLen);
	void    Close();
	void    Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen);
	void    Connect(LPSTR lpszHostAddress, UINT nHostPort);
	//Connect methods which have a timeout parameter are only provided if we are using WinSock2
	//because we are making use of Winsock2 functionality such as WSAEventSelect
	void    Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen, DWORD dwConnectionTimeout, BOOL bResetToBlockingMode = TRUE);
	void    Connect(LPSTR lpszHostAddress, UINT nHostPort, DWORD dwConnectionTimeout, BOOL bResetToBlockingMode = TRUE);

	void    IOCtl(long lCommand, DWORD* lpArgument);
	void    Listen(int nConnectionBacklog = SOMAXCONN);
	int     Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	int     ReceiveFrom(void* lpBuf, int nBufLen, SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags = 0);
	int     ReceiveFrom(void* lpBuf, int nBufLen, char* sSocketAddress, UINT& nSocketPort, int nFlags = 0);
	int     Send(const void* pBuffer, int nBufLen, int nFlags = 0);
	int     SendTo(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags = 0);
	int     SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPSTR lpszHostAddress = NULL, int nFlags = 0);
	enum { receives = 0, sends = 1, both = 2 };
	void    ShutDown(int nHow = sends);

	SOCKET GetSocket(){return m_hSocket;}
	//Operators
	operator SOCKET();

protected:

	SOCKET m_hSocket;
};

