#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
class SmartHandle
{
public:
	SmartHandle(HANDLE handle);
	~SmartHandle();
	operator bool();
	operator HANDLE();
	HANDLE handle();
private:
	HANDLE _handle = NULL;
};

