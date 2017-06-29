#pragma once

#include "sec/SecCommTypes.h"

#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif

namespace SecProcMana
{

	typedef enum _ProcState_e
	{
		PS_STOP,
		PS_RUN
	}ProcState_e;

 	int		WELLS_Export ProcStart(const char* proc_name, int copy_no, const char* config_file = NULL);
 	int		WELLS_Export ProcCreate(const char* proc_name, int copy_no, const char* config_file = NULL);
 	int		WELLS_Export ProcStop(const char* proc_name, int copy_no);
 	bool	WELLS_Export ProcAlive(const char* proc_name, int copy_no, int& pid);


	class WELLS_Export SecProcess
	{
	public:
		SecProcess(const char* proc_name, int copy_no, bool auto_run, const char* config_file = NULL);
		~SecProcess(void);

		const char*		ProcName() { return proc_name_; }
		bool			AutoRun() { return auto_run_; }
		ushort			CopyNo() { return copy_no_; }
		byte			ProcState() { return proc_state_; }
		int				RestartCount() { return restart_count_; }
		const char*		ConfigFileName() { return config_file_; }

		void		IncRestartCount();
		void		ProcState(byte state);
		void		AutoRun(bool flag);

	private:
		char		proc_name_[MAX_PROCNAME_SZ];
		ushort		copy_no_;
		char		config_file_[PROC_CONFIG_SZ];
		bool		auto_run_;			// 프로세스 감시 여부
		byte		proc_state_;		// 0:none, 1:실행, 2:중지

		int			restart_count_;		// 재시작 COUNT;
	};
}

