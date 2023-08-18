#pragma once
#define YYSDK_PLUGIN
#include "../SDK/SDK.hpp"

namespace Utils
{
	YYRValue CallBuiltinWrapper(CInstance* Instance, const char* Name, const std::vector<YYRValue>& rvArgs);
}