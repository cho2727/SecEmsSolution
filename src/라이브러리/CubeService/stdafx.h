// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.


#pragma once


#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <process.h>

#include <iostream>
using namespace std;

#ifdef UNICODE
#define _tcout wcout
#define _tostream wostream
#else
#define _tcout cout
#define _tostream ostream
#endif

#include <TCHAR.h>