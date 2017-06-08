// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.


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