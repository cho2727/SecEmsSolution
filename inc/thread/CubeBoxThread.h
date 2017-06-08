#pragma once
#include "thread/CubeThread.h"
#include "thread/CubeBox.h"
#include "thread/CubeData.h"

#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif

class WELLS_Export CubeBoxThread : public CubeThread
{
public:
	CubeBoxThread(void);
	virtual ~CubeBoxThread(void);

	int MsgSend(CubeData* pData);
	int MsgRecv(CubeData*& pData);
#if 1
	int MsgSend(CubeData::CubeDataVec& vData);
#endif
	int MsgCount();
private:
	/////////////////////////////
	CubeBox msg_box_;
};

