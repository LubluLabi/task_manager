#include "CPUusage.h"

uint64_t CPUusage::file_time_2_utc(const FILETIME* ftime)
{
	LARGE_INTEGER li;
	li.LowPart = ftime->dwLowDateTime;
	li.HighPart = ftime->dwHighDateTime;
	return li.QuadPart;
}

int CPUusage::get_processor_number()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return info.dwNumberOfProcessors; // ���������� ���������� �����������
}

void CPUusage::init()
{
	_last_system_time = 0;
	_last_time = 0;
	_hProcess = 0;
}

void CPUusage::clear()
{
	if (_hProcess) {
		CloseHandle(_hProcess);
		_hProcess = 0;
	}
}

CPUusage::CPUusage(DWORD ProcessID)
{
	init();
	_processor = get_processor_number();
	setpid(ProcessID);
}

CPUusage::CPUusage()
{
	init(); 
	_processor = get_processor_number();
}

CPUusage::~CPUusage()
{
	clear();
}

HANDLE CPUusage::setpid(DWORD ProcessID)
{
	clear();    //  ���� ����� ����������c� ������ �������, ������� ����������� ��� ����������
	init();
	// 1 �������� ���� ��� ���������� ��������; 
	//� ������ ������ ������ ����� ������ -2 (������ ����� ����� ������ -2 ����� ��������� PID � setpid � ��������)
	return _hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, ProcessID);
}

float CPUusage::get_cpu_usage()
{
	FILETIME now;
	FILETIME creation_time;
	FILETIME exit_time;
	FILETIME kernel_time;
	FILETIME user_time;
	int64_t system_time;
	int64_t time;
	int64_t system_time_delta;
	int64_t time_delta;

	DWORD exitcode;

	float cpu = -1;

	if (!_hProcess) return -1;

	GetSystemTimeAsFileTime(&now);

	//���������� �� �������
	GetExitCodeProcess(_hProcess, &exitcode);
	if (exitcode != STILL_ACTIVE) {
		clear();
		return -1;
	}

	//������� ��������� ��
	if (!GetProcessTimes(_hProcess, &creation_time, &exit_time, &kernel_time, &user_time))
	{
		clear();
		return -1;
	}
	system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time)) / _processor;
	time = file_time_2_utc(&now);

	//�������� �� ��� ������ �����������
	if ((_last_system_time == 0) || (_last_time == 0))
	{
		_last_system_time = system_time;
		_last_time = time;
		return -2;
	}

	system_time_delta = system_time - _last_system_time;
	time_delta = time - _last_time;

	if (time_delta == 0) {
		return -1;
	}

	cpu = (float)system_time_delta * 100 / (float)time_delta;
	_last_system_time = system_time;
	_last_time = time;
	return cpu;
}
