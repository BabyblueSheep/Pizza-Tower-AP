#pragma once
#define YYSDK_PLUGIN	
#include "SDK/SDK.hpp"
#include "apclient.hpp"

#include "Features/Utils.hpp"
#include "Features/AP.hpp"

DllExport YYTKStatus PluginEntry(YYTKPlugin* PluginObject);
YYTKStatus PluginUnload();
YYTKStatus FrameCallback(YYTKEventBase* pEvent, void* OptionalArgument);