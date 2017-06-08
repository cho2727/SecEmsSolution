#include "thread/SEvent.h"
#include "thread/AutoEvent.h"


CAutoEvent::CAutoEvent(
	bool initialState /* = false */)
	:  CSEvent(0, false, initialState)
{

}

CAutoEvent::CAutoEvent(
	const LPCTSTR &name, 
	bool initialState /* = false */)
	:  CSEvent(0, false, initialState, name)
{

}
