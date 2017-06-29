#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#include "common/CubeBaseString.h"
#include "NTProcessInfo.h"
#include "process/SecProcess.h"
#include "sec/SecErrorCode.h"

#ifdef _DEBUG
#pragma comment(lib, "CubeLogd.lib")
#pragma comment(lib, "CubeBaseStringd.lib")
#else
#pragma comment(lib, "CubeLog.lib")
#pragma comment(lib, "CubeBaseString.lib")
#endif

namespace SecProcMana
{
	SecProcess::SecProcess(const char* proc_name, int copy_no, bool auto_run, const char* config_file/* = NULL*/)
		: copy_no_(copy_no), auto_run_(auto_run)
	{
		memset(proc_name_, 0x00, sizeof(proc_name_));
		memset(config_file_, 0x00, sizeof(config_file_));

		if(proc_name)
			strcpy_s(proc_name_, proc_name);

		if(config_file)
			strcpy_s(config_file_, config_file);
	}


	SecProcess::~SecProcess(void)
	{
	}

	void		SecProcess::IncRestartCount()
	{
		restart_count_++;
	}

	void		SecProcess::ProcState(byte state)
	{
		proc_state_ = state;
	}

	void		SecProcess::AutoRun(bool flag)
	{
		auto_run_ = flag;
	}


	//////////////////////////////////////////////////////////////////////////////////

#include "NTProcessInfo.h"
#include <tlhelp32.h>
#include <psapi.h>  

#pragma comment(lib, "psapi")  
	DWORD  GetProcessIdByHandle(HANDLE h_proc)  
	{  
		DWORD ret = 0;  
		TCHAR sz_name[MAX_PATH];  

		if (!GetModuleBaseName(h_proc, NULL, sz_name, sizeof(sz_name)))  
			return ret;  

		HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);  
		if (h_snap == INVALID_HANDLE_VALUE)  
			return ret;  

		PROCESSENTRY32 pe32;  

		pe32.dwSize = sizeof(pe32);  
		if (!Process32First(h_snap, &pe32))  
			goto $end;  

		do {  
			if (!_tcsicmp(sz_name, pe32.szExeFile)) {  
				ret = pe32.th32ProcessID;  
				break;  
			}  
		} while (Process32Next(h_snap, &pe32));  

$end:  
		CloseHandle(h_snap);  

		return ret;  
	}


#define MAX_PI				1024	// Max processes to get for demo
	bool GetProcInfo(int pid, smPROCESSINFO* proc_info)
	{
		bool	RetVal = false;
		DWORD dwPIDs[MAX_PI] = {0};
		DWORD dwArraySize	 = MAX_PI * sizeof(DWORD);
		DWORD dwSizeNeeded	 = 0;
		DWORD dwPIDCount	 = 0;

		//== only to have better chance to read processes =====
		if(!sm_EnableTokenPrivilege(SE_DEBUG_NAME)) {
			//return RetVal;
		}

		// Get a list of Process IDs of current running processes
		if(EnumProcesses((DWORD*)&dwPIDs, dwArraySize, &dwSizeNeeded))
		{
			HMODULE hNtDll = sm_LoadNTDLLFunctions();

			if(hNtDll)
			{
				// Get detail info on each process
				dwPIDCount = dwSizeNeeded / sizeof(DWORD);
				for(DWORD p = 0; p < MAX_PI && p < dwPIDCount; p++)
				{
					if(sm_GetNtProcessInfo(dwPIDs[p], /*&lpi[p]*/proc_info))
					{
						// Do something else upon success
						if(proc_info->dwPID == pid)
						{
							RetVal = true;
							break;
						}
					}
				}

				sm_FreeNTDLLFunctions(hNtDll);
			}
		}

		// Return either PID count or MAX_PI whichever is smaller
		//return (DWORD)(dwPIDCount > MAX_PI) ? MAX_PI : dwPIDCount;
		return RetVal;
	}


	int GetCopyNo(char* szCmdLine)
	{
		int		nSize;
		bool	bStartNote = false; // 쌍따음표로 시작하는지 채크
		int		nCommandEnd = 0;
		int		nStartPoint = 0;	// szCopyNo를 저장시 사용하는 포인트

		char	szCopyNo[4] = {0,};


		if(szCmdLine == NULL)
			return -1;

		nSize = strlen(szCmdLine);

		for(int i=0; i<nSize; i++)
		{
			if(i == 0)
			{
				if(szCmdLine[i] == '\"')
					bStartNote = true;

				continue;
			}

			if(nCommandEnd == 1)
			{
				if(isspace((int)szCmdLine[i]))
				{
					if(nStartPoint != 0)
						break;
				}
				else
				{
					if(isdigit((int)szCmdLine[i]))
						szCopyNo[nStartPoint++] = szCmdLine[i];
					else
						break;
				}
				continue;
			}

			if(bStartNote) // 초기 시작이 쌍따음표인 경우
			{
				if(szCmdLine[i] == '\"')
				{
					// 명령의 마지막임
					nCommandEnd = 1;
				}
			}
			else // 초기 시작이 쌍따음표가 아닌경우
			{
				if(isspace((int)szCmdLine[i]))
				{
					// 명령의 마지막임
					nCommandEnd = 1;
				}
			}
		}

		if(nStartPoint > 0)
			return atoi(szCopyNo);

		return -1;
	}



	int				ProcStart(const char* proc_name, int copy_no, char* config_file/* = NULL*/)
	{
		int proc_id = -1;

		char		szModulePath[MAX_PATH] = {0,};
		char		szFullFileName[MAX_PATH] = {0,};

		try
		{
			char param_value[128];
			if(ProcAlive(proc_name, copy_no, proc_id))
			{
				return proc_id;
			}
			else
			{
				CubeBase::GetModulePath(szModulePath);
				sprintf_s(szFullFileName, "%s%s.exe", szModulePath, proc_name);
				if(config_file)
				{
					sprintf_s(param_value, "%d %s", copy_no, config_file);
				}
				else
				{
					sprintf_s(param_value, "%d config.xml", copy_no);
				}

				SHELLEXECUTEINFOA si;
				ZeroMemory(&si, sizeof(SHELLEXECUTEINFOA));

				si.cbSize = sizeof(SHELLEXECUTEINFOA); 
				si.lpVerb = "open";                 //mode : print, write 
				si.lpFile = szFullFileName;           //실행파일 
				si.lpParameters = param_value; //실행파일 인자값. 
				// shellIP.lpDirectory =		//실핼파일 디렉토리. 
				si.nShow = SW_HIDE;                    //SW_HIDE 
				si.fMask = SEE_MASK_NOCLOSEPROCESS|SEE_MASK_FLAG_DDEWAIT; 
				BOOL bRet = ShellExecuteExA(&si);
				//CREATE_NEW_PROCESS_GROUP
				if(bRet == TRUE)
				{
					::Sleep(1);
					WaitForInputIdle(si.hProcess, INFINITE); 
					proc_id = GetProcessIdByHandle(si.hProcess);
					return proc_id;
				}
			}		
		}
		catch (...)
		{

		}

		return proc_id;
	}

	int				ProcCreate(const char* proc_name, int copy_no, const char* config_file/* = NULL*/)
	{
		int proc_id = -1;
		char		szModulePath[MAX_PATH] = {0,};
		char		szCommandLide[MAX_PATH] = {0,};
		BOOL		state;

		try
		{
			if(ProcAlive(proc_name, copy_no, proc_id))
			{
				return proc_id;
			}
			else
			{
				PROCESS_INFORMATION pi; 
				STARTUPINFOA si = {sizeof (STARTUPINFOA)};
				CubeBase::GetModulePath(szModulePath);

				if(config_file)
				{
					sprintf_s(szCommandLide, "%s%s.exe %d %s", szModulePath, proc_name, copy_no, config_file);
				}
				else
				{
					sprintf_s(szCommandLide, "%s%s.exe %d config.xml", szModulePath, proc_name, copy_no);
				}

				if (CreateProcessA (0, szCommandLide, 
					0,//lpProcessAttributes, 
					0,//lpThreadAttributes, 
					FALSE,//bInheritHandles, 
					CREATE_NEW_PROCESS_GROUP,//dwCreationFlag, 
					0,//lpEnvironment, 
					0,//lpCurrentDirectory, 
					&si,//lpStatupInfo, 
					&pi))
				{
					CloseHandle (pi.hProcess); 
					CloseHandle (pi.hThread);
					proc_id = pi.dwProcessId;
				}
			}
		}
		catch (...)
		{

		}

		return proc_id;
	}

	int				ProcStop(const char* proc_name, int copy_no)
	{
		int			iRet = SEC_ERR;
		HANDLE		hProcess = NULL;
		int			procID = 0;

		if(ProcAlive(proc_name, copy_no, procID))
		{
			if ( hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, procID)) 
			{
				BOOL bRet = GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, procID);
				if(bRet)
				{
					iRet = SEC_OK;
				}
				else
				{
					iRet = SEC_ERR;
				}
				// 			TerminateProcess ( hProcess, 0 );
				// 			CloseHandle ( hProcess );
			}
		}

		return iRet;
	}

	bool			ProcAlive(const char* proc_name, int copy_no, int& pid)
	{
		bool bVal = false;
		HANDLE   hProcess = NULL;
		PROCESSENTRY32 pe32 = {0};
		hProcess = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
		pe32.dwSize = sizeof( PROCESSENTRY32 );
		int nCount = 0;

		pid = 0;
		if( Process32First( hProcess, &pe32 ) )
		{
			char procFullName[128] = {0,};
			char procExeName[128] = {0,};
			char procCommand[32] = {0,};
			sprintf(procFullName, "%s.exe", proc_name);

			do 
			{
				if(stricmp(procFullName, CubeBase::ConvertT2A(pe32.szExeFile, procExeName)) == 0)	//  프로세스를 찾음.
				{
					smPROCESSINFO	proc_info;
					if(GetProcInfo(pe32.th32ProcessID, &proc_info))
					{
						int find_copy_no = GetCopyNo(CubeBase::ConvertT2A(proc_info.szCmdLine, procCommand));
						if(copy_no == find_copy_no)
						{
							pid = pe32.th32ProcessID;
							bVal = true;
							break;
						}
					}
				}
			} while ( Process32Next( hProcess, &pe32 ) );
		}

		CloseHandle (hProcess);
		return bVal;
	}
}
