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
#include <map>
#include <iostream>
#include <list>
#include <algorithm>
using namespace std;

#include "sec/SecSocketInit.h"
#include "log/CubeLog.h"
#include "sec/SecErrorCode.h"
#include "sec/SecCommTypes.h"
#include "sec/SecMessageCode.h"

#include "shm/SecShardMem.h"

#include "SecConfig.h"

#define		MainThreadWait			100
#define		OWNPROCNAME			SEC_REAL_PROC

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
