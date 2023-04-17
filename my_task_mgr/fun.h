#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <time.h>
#include <string>
#include <thread>

#include "SmartHandle.h"
#include "CPUusage.h"

#ifdef UNICODE
#define tcout std :: wcout
#elif
#define tcout std :: cout
#endif 

using namespace std;

struct ProcessInfo {
	PROCESSENTRY32 pe;
	vector<THREADENTRY32> threads;
};

vector<ProcessInfo> print_process();
void takt_process();
bool get_id(DWORD ID);
bool term(vector<ProcessInfo> processInfos);

void load_proc(vector<ProcessInfo> processInfos);
void get_id_threads(vector<ProcessInfo> processInfos);

static float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks);
static unsigned long long FileTimeToInt64(const FILETIME& ft);
float GetCPULoad();

//tests

char test_choice_enter();