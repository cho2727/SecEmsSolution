// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <Windows.h>

#include <iostream>
#include <list>

#include "sec/SecSocketInit.h"
#include "log/CubeLog.h"
#include "sec/SecErrorCode.h"
#include "sec/SecCommTypes.h"
#include "sec/SecMessageCode.h"

#define		MainThreadWait			1000
#define		OWNPROCNAME			"SecScheduler"
// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
