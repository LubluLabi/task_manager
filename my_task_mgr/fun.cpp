#include "fun.h"

HANDLE h;
//static HWND hCombo, hwndDlg;
//static HWND hPr;

vector<ProcessInfo> print_process()
{
	SmartHandle processSnap(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
	SmartHandle threadSnap(CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0));

	system("cls");

	if (!processSnap || !threadSnap) {
		cout << "\nerror\n";
	}

	vector<ProcessInfo> processInfos;
	vector<THREADENTRY32> threads;

	THREADENTRY32 te{ sizeof(THREADENTRY32) };
	if (Thread32First(threadSnap, &te) == FALSE) {
		cout << "\nerror\n";
	}

	do {
		threads.push_back(te);
	} while (Thread32Next(threadSnap, &te));

	PROCESSENTRY32 pe{ sizeof(PROCESSENTRY32) };
	if (Process32First(processSnap, &pe) == FALSE) {
		cout << "\nerror\n";
	}

	do {
		vector<THREADENTRY32> subThreads;
		for (const auto& thread : threads) {
			if (thread.th32OwnerProcessID == pe.th32ProcessID) {
				subThreads.push_back(thread);
			}
		}
		processInfos.push_back(ProcessInfo{ pe, subThreads });
	} while (Process32Next(processSnap, &pe));

	int i = 0;
	for (const auto& processInfo : processInfos) {
		tcout << i << ". ";
		i++;
		tcout << "\tid[" << processInfo.pe.th32ProcessID << "] \t" << processInfo.pe.szExeFile << " [" <<
			processInfo.pe.cntThreads << "] " << endl;
	}
	tcout << "\n\tCPU: " << GetCPULoad() * 100 << "%\n" << endl;
	return processInfos;
}

void takt_process()
{
	while (1) {

		int ms = 60000;

		clock_t end_timer = clock() + ms;
		long long int set = 0;
		ms = 0;

		while (clock() < end_timer) {
			set++;
		};

		if (set >= 1) {

			SmartHandle processSnap(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
			SmartHandle threadSnap(CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0));

			system("cls");

			if (!processSnap || !threadSnap) {
				//return 1;
			}

			vector<ProcessInfo> processInfos;
			vector<THREADENTRY32> threads;

			THREADENTRY32 te{ sizeof(THREADENTRY32) };
			if (Thread32First(threadSnap, &te) == FALSE) {
				//return 2;
			}

			do {
				threads.push_back(te);
			} while (Thread32Next(threadSnap, &te));

			PROCESSENTRY32 pe{ sizeof(PROCESSENTRY32) };
			if (Process32First(processSnap, &pe) == FALSE) {
				//return 2;
			}

			do {
				vector<THREADENTRY32> subThreads;
				for (const auto& thread : threads) {
					if (thread.th32OwnerProcessID == pe.th32ProcessID) {
						subThreads.push_back(thread);
					}
				}
				processInfos.push_back(ProcessInfo{ pe, subThreads });
			} while (Process32Next(processSnap, &pe));

			int i = 0;
			for (const auto& processInfo : processInfos) {
				tcout << i << ". ";
				i++;
				tcout << "\tid[" << processInfo.pe.th32ProcessID << "] \t" << processInfo.pe.szExeFile << " [" <<
					processInfo.pe.cntThreads << "] " << endl;
			}
			tcout << "\n\tCPU: " << GetCPULoad() * 100 << "%\n" << endl;
		}
	}
}

bool get_id(DWORD ID)
{
	//Получение прав на завершение
	h = OpenProcess(PROCESS_TERMINATE, 0, ID);
	if (h == INVALID_HANDLE_VALUE) {
		cout << "INVALID_HANDLE_VALUE!" << endl;
		return false;
	}
	int i = 0;
	//Завершение
	if (TerminateProcess(h, i) != 0) {
		cout << "process [" << ID << "] terminate\n";
		return true;
	}
	else return false;

	CloseHandle(h);
}

bool term(vector<ProcessInfo> processInfos)
{
	wstring name;
	tcout << "> ";
	wcin >> name;

	for (const auto& processInfo : processInfos) {
		if (name == processInfo.pe.szExeFile) {
			if ((get_id(processInfo.pe.th32ProcessID)) == true) {
				return true;
			}
			else return false;
		}
	}
}

void load_proc(vector<ProcessInfo> processInfos)
{
	int id = 0, flag = 0; CPUusage usg; wstring name;
	tcout << "> "; wcin >> name;

	for (const auto& processInfo : processInfos) {
		if (processInfo.pe.szExeFile == name) {
			id = processInfo.pe.th32ProcessID;
			if (id > 0) {
				flag = 1;
				break;
			}
			break;
		}
	}

	if (flag == 1) {

		if (id < 0) {
			tcout << "Error ID\n";
		}

		usg.setpid(id);
		float sr_n = 0, sr = 0;
		int flag = 0;
		cout << "\nloading ";
		for (int i = 0; i < 10; i++) {
			float cpu = usg.get_cpu_usage();
			if (i > 0 && cpu != 0.00) {
				sr_n = sr_n + cpu;
				flag++;
			}
			//printf(" \tcpu: %.2f%%\n ", cpu);
			printf("*");
			Sleep(500);
		}
		tcout << "\n";
		sr = sr_n / flag;

		if (sr > 0.001) {
			printf("loading end: load process [%d]: %.2f%%\n", id, sr);
			//return false;
		}
		else tcout << "\nload < 0.001%\n";
		sr = 0;
	}
	else cout << "name not found!\n";
}

void get_id_threads(vector<ProcessInfo> processInfos)
{
	wstring name;
	tcout << "> "; wcin >> name;

	for (const auto& processInfo : processInfos) {
		if (processInfo.pe.szExeFile == name) {
			for (const auto& thread : processInfo.threads) {
				tcout << "   " << thread.th32ThreadID << endl;
			}
			break;
		}
	}
}

float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks)
{
	static unsigned long long _previousTotalTicks = 0;
	static unsigned long long _previousIdleTicks = 0;
	
	unsigned long long totalTicksSinceLastTime = totalTicks - _previousTotalTicks; // общие = общ - предыдущий
	unsigned long long idleTicksSinceLastTime = idleTicks - _previousIdleTicks; // соло

	float ret = 1.0f - ((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime) / totalTicksSinceLastTime : 0);

	_previousTotalTicks = totalTicks;
	_previousIdleTicks = idleTicks;
	return ret;
}

unsigned long long FileTimeToInt64(const FILETIME& ft)
{
	return (((unsigned long long)(ft.dwHighDateTime)) << 32) | ((unsigned long long)ft.dwLowDateTime);
}

float GetCPULoad()
{
	FILETIME idleTime, kernelTime, userTime;
	return GetSystemTimes(&idleTime, &kernelTime, &userTime) ? CalculateCPULoad(FileTimeToInt64(idleTime), FileTimeToInt64(kernelTime) + FileTimeToInt64(userTime)) : -1.0f;
}

char test_choice_enter()
{
	char choice = 'l';
	/*string str = "ptlhg"; char choice;
	for (int i = 0; i < str.size(); i++) {
		choice = str[i];
		return choice;
	}*/
	return choice;
}
