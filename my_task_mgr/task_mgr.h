#pragma once
#include "SmartHandle.h"
#include "CPUusage.h"
class task_mgr : public SmartHandle, public CPUusage
{
public:
	~task_mgr();
};

