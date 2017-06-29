#pragma once

// typedef enum _ProcState_e
// {
// 	PS_STOP,
// 	PS_RUN
// }ProcState_e;

class SecProgram
{
public:
	SecProgram(char* proc_name, bool use_yn);
	~SecProgram(void);

	char*		ProcName() { return proc_name_; }
	bool		UseYN() { return use_yn_; }
	byte		ProcState() { return proc_state_; }
	int			RestartCount() { return restart_count_; }

	void		IncRestartCount();
	void		ProcState(byte state);
	void		UseYN(bool flag);

private:
	char		proc_name_[MAX_PROCNAME_SZ];
	bool		use_yn_;			// 프로세스 감시 여부
	byte		proc_state_;		// 0:none, 1:실행, 2:중지
	
	int			restart_count_;		// 재시작 COUNT;
};

