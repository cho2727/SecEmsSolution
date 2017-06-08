// SEvent.h: interface for the CSEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEVENT_H__463961FE_E4BC_40FC_8E32_74B7750ECF16__INCLUDED_)
#define AFX_SEVENT_H__463961FE_E4BC_40FC_8E32_74B7750ECF16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


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

#endif // !defined(AFX_SEVENT_H__463961FE_E4BC_40FC_8E32_74B7750ECF16__INCLUDED_)
