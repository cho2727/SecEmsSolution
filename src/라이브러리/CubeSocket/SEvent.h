// SEvent.h: interface for the CSEvent class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif


#include <Windows.h>

class WELLS_Export CSEvent  
{
  public :
   
      CSEvent(
         LPSECURITY_ATTRIBUTES lpSecurityAttributes,
         bool manualReset,
         bool initialState);
   
       CSEvent(
          LPSECURITY_ATTRIBUTES lpSecurityAttributes,
          bool manualReset,
          bool initialState,
          const LPCTSTR &name);
      
      virtual ~CSEvent();

      HANDLE GetEvent() const;

      void Wait() const;

      bool Wait(
         DWORD timeoutMillis) const;

      void Reset();

      void Set();

      void Pulse();

   private :

      HANDLE m_hEvent;

      // No copies do not implement
      CSEvent(const CSEvent &rhs);
      CSEvent &operator=(const CSEvent &rhs);
};

