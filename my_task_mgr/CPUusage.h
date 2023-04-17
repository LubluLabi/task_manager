#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <process.h>
class CPUusage
{
private:
	typedef long long int64_t;
	typedef unsigned long long uint64_t;
	HANDLE _hProcess;
	int _processor;     //number of cpu
	int64_t _last_time;         // last time
	int64_t _last_system_time;


	// �������������� �������
	uint64_t file_time_2_utc(const FILETIME* ftime);

	// �������� ���������� ���� ����������
	int get_processor_number();

	// �-��� ��� �������������
	void init();
	//�������� ������������ ��������
	void clear();
public:
	CPUusage(DWORD ProcessID);
	CPUusage();
	~CPUusage();
	HANDLE setpid(DWORD ProcessID); //������������ �������� � ��� ���������� ��������, ������� ����� ���������� ���������� OpenProcess 
	float get_cpu_usage();
};

