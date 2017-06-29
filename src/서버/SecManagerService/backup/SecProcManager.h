#pragma once
#include "process/SecProcess.h"

class SecProcManager
{
public:
	typedef list<SecProcMana::SecProcess*> ProcList;
private:
	SecProcManager(void);
public:
	~SecProcManager(void);

	static SecProcManager* GetInstance();

	int				AddProgram(const char* proc_name, int copy_no, bool auto_run, char* config_file/* = NULL*/);
	SecProcMana::SecProcess*		GetProgram(const char* proc_name, ushort copy_no);
	void			DelProgram();
	int				GetProgramCount();

	int				ProcStart();
	int				ProcStop();
	virtual int				ProcCheck();

private:
	ProcList			proc_list_;
};

