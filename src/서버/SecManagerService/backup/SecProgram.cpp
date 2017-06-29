#include "StdAfx.h"
#include "SecProgram.h"


SecProgram::SecProgram(char* proc_name, bool use_yn)
	: use_yn_(use_yn)
{
	strcpy_s(proc_name_, proc_name);
}


SecProgram::~SecProgram(void)
{
}

void		SecProgram::IncRestartCount()
{
	restart_count_++;
}

void		SecProgram::ProcState(byte state)
{
	proc_state_ = state;
}

void		SecProgram::UseYN(bool flag)
{
	use_yn_ = flag;
}