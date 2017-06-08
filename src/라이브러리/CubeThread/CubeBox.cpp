#include "thread/CubeBox.h"
#include "thread/CubeData.h"
#include <iostream>


CubeBox::CubeBox(int capacity)
	:queue_count_(capacity), 
	queue_(NULL), 
	enqueue_index_(0),
	dequeue_index_(0),
	cycle_value_(0)
{
	queue_ = new CubeData*[capacity];
	memset(queue_, 0x00, sizeof(CubeData*)*capacity);
	InitializeCriticalSection(&cs_); 
}


CubeBox::~CubeBox(void)
{
	release();
	DeleteCriticalSection(&cs_); //임계영역을 정리한다.
}


int CubeBox::count()
{
	int iCount = 0;
	if(cycle_value_ == 0)
		iCount = enqueue_index_ - dequeue_index_;
	else
		iCount = enqueue_index_ + (queue_count_ - dequeue_index_);

	return iCount;
}

bool CubeBox::is_empty (void) const
{
	if(dequeue_index_ == enqueue_index_ && cycle_value_ == 0)
	{
		return true;
	}

	return false;
}


bool CubeBox::is_full (void) const
{
	if(dequeue_index_ == enqueue_index_ && cycle_value_ == 1/*queue_[enqueue_index_] != NULL*/)
	{
		return true;
	}

	return false;
}


#if 1
int CubeBox::enqueue (CubeData::CubeDataVec& new_data)
{
	int nRet = -1;
	EnterCriticalSection(&cs_);
	try
	{
		if(!is_full())
		{
			CubeData::CubeDataVec::iterator it = new_data.begin();
			while(it != new_data.end())
			{
				queue_[enqueue_index_] = (CubeData *)(*it);

				enqueue_index_++;
				if(enqueue_index_ >= queue_count_)
				{
					enqueue_index_ = 0;
					cycle_value_++;
				}

				it = new_data.erase(it); // 확인 필요
			}
			nRet = 0;
		}
	}
	catch (...)
	{

	}
	LeaveCriticalSection(&cs_);
	return nRet;
}
#endif

int  CubeBox::enqueue (const CubeData *new_item)
{
	int nRet = -1;
	EnterCriticalSection(&cs_);
	try
	{
		if(!is_full())
		{
			queue_[enqueue_index_] = (CubeData *)(new_item);

			enqueue_index_++;
			if(enqueue_index_ >= queue_count_)
			{
				enqueue_index_ = 0;
				cycle_value_++;
			}

			nRet = 0;
		}
	}
	catch (...)
	{

	}
	LeaveCriticalSection(&cs_);
	return nRet;
}


int  CubeBox::dequeue (CubeData *&item)
{
	int nRet = -1;
	EnterCriticalSection(&cs_);
	try
	{
		if(!is_empty())
		{
			item = (CubeData *)queue_[dequeue_index_];

			queue_[dequeue_index_] = NULL;
			dequeue_index_++;
			if(dequeue_index_ >= queue_count_)
			{
				dequeue_index_ = 0;
				cycle_value_--;
			}
			nRet = 0;
		}
	}
	catch (...)
	{

	}

	LeaveCriticalSection(&cs_);
	return nRet;
}


int  CubeBox::release ()
{
	release_message ();

	delete queue_;
	queue_ = NULL;
	return 0;
}



int  CubeBox::release_message ()
{
	while(!(dequeue_index_ == enqueue_index_ && cycle_value_ == 0))
	{
		CubeData *item = queue_[dequeue_index_++];
		if(dequeue_index_ >= queue_count_)
		{
			dequeue_index_ = 0;
			cycle_value_--;
		}

		//allocator_->free(item);
		delete item;
	}

	return 0;
}
