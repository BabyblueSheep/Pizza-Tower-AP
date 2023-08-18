#include "Utils.hpp"

YYRValue Utils::CallBuiltinWrapper(CInstance* Instance, const char* Name, const std::vector<YYRValue>& rvArgs)
{
    YYRValue Result;
    CallBuiltin(Result, Name, Instance, Instance, rvArgs);
    return Result;
}