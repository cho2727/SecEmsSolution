#pragma once
#include "SEvent.h"

#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif

class WELLS_Export CManualEvent : public CSEvent  
{
public :

	explicit CManualEvent(
		bool initialState = false);

	explicit CManualEvent(
		const LPCTSTR &name, 
		bool initialState = false);

private :

	// No copies do not implement
	CManualEvent(const CManualEvent &rhs);
	CManualEvent &operator=(const CManualEvent &rhs);
};
