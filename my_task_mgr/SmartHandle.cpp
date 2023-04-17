#include "SmartHandle.h"

SmartHandle::SmartHandle(HANDLE handle)
{
	_handle = handle;
}

SmartHandle::~SmartHandle()
{
	if (_handle) {
		CloseHandle(_handle);
	}
}

SmartHandle::operator bool()
{
	return _handle != NULL;
}

SmartHandle::operator HANDLE()
{
	return _handle;
}

HANDLE SmartHandle::handle()
{
	return _handle;
}
