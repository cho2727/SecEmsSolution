#pragma once
#include "thread/CubeThread.h"
#include "thread/CubeBoxT.h"

template <typename T>
class CubeBoxThreadT : public CubeThread
{
public:
	CubeBoxThreadT(int count):msg_box_(count){}
	~CubeBoxThreadT(void){}

	int MsgSend(T* pData){return msg_box_.enqueue(pData);}
	int MsgRecv(T*& pData){return msg_box_.dequeue(pData);}

	int MsgCount(){return msg_box_.count();}
private:
	/////////////////////////////
	CubeBoxT<T> msg_box_;
};
