// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
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

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
