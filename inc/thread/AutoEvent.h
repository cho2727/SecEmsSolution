#pragma once
#include "SEvent.h"

#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif

class WELLS_Export CAutoEvent : public CSEvent  
{
public :

	explicit CAutoEvent(
		bool initialState = false);

	explicit CAutoEvent(
		const LPCTSTR &name, 
		bool initialState = false);

private :

	// No copies do not implement
	CAutoEvent(const CAutoEvent &rhs);
	CAutoEvent &operator=(const CAutoEvent &rhs);
};
