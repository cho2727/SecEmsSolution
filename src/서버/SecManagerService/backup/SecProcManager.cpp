#include "StdAfx.h"
#include "SecProcManager.h"
#include "common/CubeBaseString.h"


SecProcManager::SecProcManager(void)
{
}


SecProcManager::~SecProcManager(void)
{
	DelProgram();
}

SecProcManager* SecProcManager::GetInstance()
{
	static SecProcManager _instance;
	return &_instance;
}


int				SecProcManager::AddProgram(const char* proc_name, int copy_no, bool auto_run, char* config_file = NULL)
{
	SecProcMana::SecProcess *proc = GetProgram(proc_name, copy_no);
	if(proc != NULL)
	{
		return SEC_DATA_EXIST;
	}

	proc = new SecProcMana::SecProcess(proc_name, copy_no, auto_run, config_file);
	proc_list_.push_back(proc);

	return SEC_OK;
}

SecProcMana::SecProcess*		SecProcManager::GetProgram(const char* proc_name, ushort copy_no)
{
	ProcList::iterator	it;
	for(it = proc_list_.begin(); it != proc_list_.end(); it++)
	{
		if(stricmp((*it)->ProcName(), proc_name) == 0 && (*it)->CopyNo() == copy_no)
		{
			return (*it);
		}
	}

	return NULL;
}

void			SecProcManager::DelProgram()
{
	ProcList::iterator it = proc_list_.begin();

	while (	it != proc_list_.end())
	{
		delete (*it);
		it = proc_list_.erase(it);
	}
}

int				SecProcManager::GetProgramCount()
{
	return proc_list_.size();
}


/////////////////////////////////////////////////////////////////////////////

int				SecProcManager::ProcStart()
{
	ProcList::iterator	it;
	for(it = proc_list_.begin(); it != proc_list_.end(); it++)
	{
		if(SecProcMana::ProcCreate((*it)->ProcName(), (*it)->CopyNo(), (*it)->ConfigFileName()) > 0)
			(*it)->ProcState(SecProcMana::ProcState_e::PS_RUN);
		else
			(*it)->ProcState(SecProcMana::ProcState_e::PS_STOP);
	}
	return 0;
}

int				SecProcManager::ProcStop()
{
	ProcList::iterator	it;
	for(it = proc_list_.begin(); it != proc_list_.end(); it++)
	{
		SecProcMana::ProcStop((*it)->ProcName(), (*it)->CopyNo());
	}

	return 0;
}

int				SecProcManager::ProcCheck()
{
	int proc_id = -1;

	ProcList::iterator	it;
	for(it = proc_list_.begin(); it != proc_list_.end(); it++)
	{
		if((*it)->AutoRun())
		{
			if(!SecProcMana::ProcAlive((*it)->ProcName(), 0, proc_id))
			{
				if(SecProcMana::ProcCreate((*it)->ProcName(), 0) > 0)
				{
					(*it)->ProcState(SecProcMana::ProcState_e::PS_RUN);
					(*it)->IncRestartCount();
				}
				else
				{
					(*it)->ProcState(SecProcMana::ProcState_e::PS_STOP);
				}

				WLOG("proc(%s:%d) cnt:%d state:%d restart\n"
					, (*it)->ProcName()
					, (*it)->CopyNo()
					, (*it)->RestartCount()
					, (*it)->ProcState());
			}
		}
	}

	return 0;
}
