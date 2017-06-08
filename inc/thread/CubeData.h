#pragma once

#include <iostream>
#include <vector>

#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif

using namespace std;

class WELLS_Export CubeData
{
public:
	typedef vector<CubeData*> CubeDataVec;

public:
	CubeData(void);
	CubeData(int buf_size);
	// 	CubeData(unsigned int nSize);
	// 	CubeData(const void* pBuf, unsigned int nSize);

	~CubeData(void);

	void*					get_buffer  ( ) const ;
	void					set_buffer  ( const void* pBuf, unsigned int nSize );
	void					set_swell_buffer ( const void* pBuf, unsigned int nSize );
	size_t					buffer_size ( );

	void SetSrcProcInfo(unsigned short usNodeCode, unsigned short usCopyNo, char* szProcName);
	void SetDestProcInfo(unsigned short usNodeCode, unsigned short usCopyNo, char* szProcName);

	unsigned short			SrcNodeCode(){ return usSrcNodeCode_; }
	unsigned short			SrcCopyNumber(){ return usSrcCopyNo_; }
	const char*				SrcProcName(){ return szSrcProcName_; }

	unsigned short			DestNodeCode(){ return usDestNodeCode_; }
	unsigned short			DestCopyNumber(){ return usDestCopyNo_; }
	const char*				DestProcName(){ return szDestProcName_; }

private:
	int						allocate_size( size_t nSize );

private:
	void*				buffer_	; //!<	���� �����͸� �����ϴ� ����
	unsigned int		bufsize_; //!<  bufsize

	unsigned short		usSrcNodeCode_;
	unsigned short		usSrcCopyNo_;
	char				szSrcProcName_[16];

	unsigned short		usDestNodeCode_;
	unsigned short		usDestCopyNo_;
	char				szDestProcName_[16];
};

