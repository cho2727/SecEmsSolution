#include "thread/CubeBoxThread.h"

#define CUBEBOXSIZE			1000
CubeBoxThread::CubeBoxThread(void) : msg_box_(CUBEBOXSIZE)
{
}

CubeBoxThread::~CubeBoxThread(void)
{
}

int CubeBoxThread::MsgSend(CubeData* pData)
{
	return msg_box_.enqueue(pData);
}

int CubeBoxThread::MsgRecv(CubeData*& pData)
{
	return msg_box_.dequeue(pData);
}

#if 1
int CubeBoxThread::MsgSend(CubeData::CubeDataVec& vData)
{
	return msg_box_.enqueue(vData);
}
#endif

int CubeBoxThread::MsgCount()
{
	return msg_box_.count();
}