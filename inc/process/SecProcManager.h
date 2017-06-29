#pragma once

#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif


#include "process/SecProcess.h"
#include <list>
using namespace std;

namespace SecProcMana
{
	class WELLS_Export SecProcManager
	{
	public:
		typedef list<SecProcMana::SecProcess*> ProcList;
	private:
		SecProcManager(void);
	public:
		~SecProcManager(void);

		static SecProcManager* GetInstance();

		int				AddProgram(const char* proc_name, int copy_no, bool auto_run, const char* config_file/* = NULL*/);
		SecProcess*		GetProgram(const char* proc_name, ushort copy_no);
		void			DelProgram();
		int				GetProgramCount();

		int				ProcStart();
		int				ProcStop();
		virtual int				ProcCheck();

	private:
		ProcList			proc_list_;
	};

}

