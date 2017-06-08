#pragma once
#include <Windows.h>
#include "CubeData.h"

template <typename T>
class CubeBoxT
{
public:
	CubeBoxT(int capacity)
		:queue_count_(capacity), 
		queue_(NULL), 
		enqueue_index_(0),
		dequeue_index_(0),
		cycle_value_(0)
	{
		queue_ = new T*[capacity];
		memset(queue_, 0x00, sizeof(T*)*capacity);
		InitializeCriticalSection(&cs_); 
	}

	~CubeBoxT(void)
	{
		release();
		DeleteCriticalSection(&cs_); //임계영역을 정리한다.
	}


	bool is_empty (void) const
	{
		if(dequeue_index_ == enqueue_index_ && cycle_value_ == 0)
		{
			return true;
		}

		return false;
	}

	bool is_full (void) const
	{
		if(dequeue_index_ == enqueue_index_ && cycle_value_ == 1/*queue_[enqueue_index_] != NULL*/)
		{
			return true;
		}

		return false;
	}

	int enqueue (const T *new_item)
	{
		int nRet = -1;
		EnterCriticalSection(&cs_);
		try
		{
			if(!is_full())
			{
				queue_[enqueue_index_] = (T *)(new_item);

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


	int dequeue (T *&item)
	{
		int nRet = -1;
		EnterCriticalSection(&cs_);
		try
		{
			if(!is_empty())
			{
				item = (T *)queue_[dequeue_index_];

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
	int release ()
	{
		release_message ();

		delete queue_;
		queue_ = NULL;
		return 0;
	}


	int release_message ()
	{
		while(!(dequeue_index_ == enqueue_index_ && cycle_value_ == 0))
		{
			T *item = queue_[dequeue_index_++];
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

	int count()
	{
		int iCount = 0;
		if(cycle_value_ == 0)
			iCount = enqueue_index_ - dequeue_index_;
		else
			iCount = enqueue_index_ + (queue_count_ - dequeue_index_);

		return iCount;
	}

private:
	T** queue_;
	int queue_count_;
	int enqueue_index_;
	int dequeue_index_;
	int cycle_value_;


	CRITICAL_SECTION	cs_;
};

