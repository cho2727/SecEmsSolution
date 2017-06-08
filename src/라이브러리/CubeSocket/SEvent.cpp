// SEvent.cpp: implementation of the CSEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "SEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Static helper methods
///////////////////////////////////////////////////////////////////////////////

static HANDLE Create(
   LPSECURITY_ATTRIBUTES lpEventAttributes, 
   bool bManualReset, 
   bool bInitialState, 
   LPCTSTR lpName);

///////////////////////////////////////////////////////////////////////////////
// CEvent
///////////////////////////////////////////////////////////////////////////////

CSEvent::CSEvent(
   LPSECURITY_ATTRIBUTES lpEventAttributes, 
   bool bManualReset, 
   bool bInitialState)
   :  m_hEvent(Create(lpEventAttributes, bManualReset, bInitialState, 0))
{

}

 CSEvent::CSEvent(
    LPSECURITY_ATTRIBUTES lpEventAttributes, 
    bool bManualReset, 
    bool bInitialState, 
    const LPCTSTR &name)
    :  m_hEvent(Create(lpEventAttributes, bManualReset, bInitialState, name))
 {
 
 }

CSEvent::~CSEvent()
{
   ::CloseHandle(m_hEvent);
}

HANDLE CSEvent::GetEvent() const
{
   return m_hEvent;
}

void CSEvent::Wait() const
{
	Wait(INFINITE);
}

bool CSEvent::Wait(DWORD timeoutMillis) const
{
   bool ok;

   DWORD result = ::WaitForSingleObject(m_hEvent, timeoutMillis);

   if (result == WAIT_TIMEOUT)
   {
      ok = false;
   }
   else if (result == WAIT_OBJECT_0)
   {
      ok = true;
   }
   return ok;
}

void CSEvent::Reset()
{
	::ResetEvent(m_hEvent);
}

void CSEvent::Set()
{
   ::SetEvent(m_hEvent);
}

void CSEvent::Pulse()
{
   ::PulseEvent(m_hEvent);
}

///////////////////////////////////////////////////////////////////////////////
// Static helper methods
///////////////////////////////////////////////////////////////////////////////

static HANDLE Create(
   LPSECURITY_ATTRIBUTES lpEventAttributes, 
   bool bManualReset, 
   bool bInitialState, 
   LPCTSTR lpName)
{
   HANDLE hEvent = ::CreateEvent(lpEventAttributes, bManualReset, bInitialState, lpName);
   return hEvent;
}
