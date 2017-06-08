#include "thread/SEvent.h"
#include "thread/ManualEvent.h"


CManualEvent::CManualEvent(
	bool initialState /* = false */)
	:  CSEvent(0, true, initialState)
{

}

CManualEvent::CManualEvent(
	const LPCTSTR &name, 
	bool initialState /* = false */)
	:  CSEvent(0, true, initialState, name)
{

}
