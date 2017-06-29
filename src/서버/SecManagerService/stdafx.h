// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

//#include <winsock2.h>
#include <Windows.h>

#include <iostream>
#include <list>
using namespace std;

#include "sec/SecSocketInit.h"
#include "log/CubeLog.h"
#include "sec/SecErrorCode.h"
#include "sec/SecCommTypes.h"
#include "sec/SecMessageCode.h"
#include "SecConfig.h"

#define		MainThreadWait			100
#define		ProcThreadWait			10
#define		OWNPROCNAME				"SecManagerSvc"

#define SERVICE_SHOW_NAME		_T("Sec Manager Service")
#define SERVICE_NAME			_T("SecService")

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
