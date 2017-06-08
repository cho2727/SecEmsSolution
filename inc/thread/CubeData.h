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
	void					set_swell_buffer  ( const void* pBuf, unsigned int nSize );
	size_t					buffer_size ( );

	void SetProcInfo(unsigned short usNodeCode, unsigned short usCopyNo, char* szProcName);

	unsigned short			NodeCode(){ return usNodeCode_; }
	unsigned short			CopyNumber(){ return usCopyNo_; }
	const char*		ProcName(){ return szProcName_; }

private:
	int						allocate_size( size_t nSize );

private:
	void*				buffer_	; //!<	실제 데이터를 저장하는 버퍼
	unsigned int		bufsize_; //!<  bufsize

	unsigned short	usNodeCode_;
	unsigned short	usCopyNo_;
	char	szProcName_[16];
};

