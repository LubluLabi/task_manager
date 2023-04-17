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


	// Преобразование времени
	uint64_t file_time_2_utc(const FILETIME* ftime);

	// Получить количество ядер процессора
	int get_processor_number();

	// Ф-ции для инициализации
	void init();
	//Закрытие дескрипторов процесса
	void clear();
public:
	CPUusage(DWORD ProcessID);
	CPUusage();
	~CPUusage();
	HANDLE setpid(DWORD ProcessID); //Возвращаемое значение — это дескриптор процесса, который может определить успешность OpenProcess 
	float get_cpu_usage();
};

