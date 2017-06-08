#pragma once
#include <Windows.h>
#include "CubeData.h"

class CubeData;
class CubeBox
{
public:
	CubeBox(int capacity);
	~CubeBox(void);

	bool is_empty (void) const;
	bool is_full (void) const;

	int enqueue (const CubeData *new_item);
#if 1
	int enqueue (CubeData::CubeDataVec& new_data);
#endif
	int dequeue (CubeData *&item);
	int release ();
	int release_message ();
	int count();

private:
	CubeData** queue_;
	int queue_count_;
	int enqueue_index_;
	int dequeue_index_;
	int cycle_value_;


	CRITICAL_SECTION	cs_;
};

