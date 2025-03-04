#pragma once

#include "common/AkType.h"
#include "Enum.h"

struct EventHandle_t
{
	EVENT_TYPE eType = {};
	void* pObj = nullptr;
	AkU64 u64Param0 = 0;
	AkU64 u64Param1 = 0;
	AkU64 u64Param2 = 0;
};