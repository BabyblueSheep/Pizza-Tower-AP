#include "dllmain.hpp"
#include <Windows.h>
#include <vector>

#ifdef CLR_DEFAULT
#undef CLR_DEFAULT
#endif

#define apClient AP::Client


static CallbackAttributes_t* FrameCallbackAttributes = nullptr;
static CallbackAttributes_t* CodeHandlerAttributes = nullptr;

static uint32_t FrameNumber = 0;


YYTKStatus CodeHandler(YYTKEventBase* pEvent, void*)
{
	YYTKCodeEvent* pCodeEvent = dynamic_cast<decltype(pCodeEvent)>(pEvent);

	auto& [Self, Other, Code, Res, Flags] = pCodeEvent->Arguments();

	if (!Code->i_pName)
		return YYTK_INVALIDARG;

	if (_stricmp(Code->i_pName, "gml_Object_obj_mainmenu_Alarm_3") == 0)
	{
		AP::APStartConnection();
	}
}


YYTKStatus FrameCallback(YYTKEventBase* PluginEvent, void* OptionalArgument)
{
	FrameNumber++;
	if (FrameNumber == 60) {
		FrameNumber = 0;
		Utils::CallBuiltinWrapper(nullptr, "gc_collect", {});
	}
	if (apClient) apClient->poll();

	return YYTK_OK;
}



YYTKStatus PluginUnload()
{
	Utils::CallBuiltinWrapper(nullptr, "gc_enable", { 1.0 });

	YYTKStatus Removal = PmRemoveCallback(FrameCallbackAttributes);
	if (Removal != YYTK_OK)
	{
		PrintError(__FILE__, __LINE__, "[Pizza Tower AP] PmRemoveCallback failed with 0x%x", Removal);
	}
	Removal = PmRemoveCallback(CodeHandlerAttributes);
	if (Removal != YYTK_OK)
	{
		PrintError(__FILE__, __LINE__, "[Pizza Tower AP] PmRemoveCallback failed with 0x%x", Removal);
	}

	return YYTK_OK;
}



DllExport YYTKStatus PluginEntry(
	YYTKPlugin* PluginObject
)
{
	Utils::CallBuiltinWrapper(nullptr, "gc_enable", { 0.0 });
	PluginObject->PluginUnload = PluginUnload;

	PrintMessage(CLR_DEFAULT, "[Pizza Tower AP] Loaded for YYTK %s!", YYSDK_VERSION);


	PluginAttributes_t* PluginAttributes = nullptr;
	if (YYTKStatus Status = PmGetPluginAttributes(PluginObject, PluginAttributes))
	{
		PrintError(__FILE__, __LINE__, "[Pizza Tower AP] - PmGetPluginAttributes failed with 0x%x", Status);
		return YYTK_FAIL;
	}
	YYTKStatus Status = PmCreateCallback(PluginAttributes, FrameCallbackAttributes, FrameCallback, static_cast<EventType>(EVT_PRESENT | EVT_ENDSCENE), nullptr);
	if (Status)
	{
		PrintError(__FILE__, __LINE__, "[Pizza Tower AP] - PmCreateCallback failed with 0x%x", Status);
		return YYTK_FAIL;
	}
	Status = PmCreateCallback(PluginAttributes, CodeHandlerAttributes, CodeHandler, EVT_CODE_EXECUTE, nullptr);
	if (Status)
	{
		PrintError(__FILE__, __LINE__, "[Pizza Tower AP] - PmCreateCallback failed with 0x%x", Status);
		return YYTK_FAIL;
	}

	return YYTK_OK;
}






BOOL APIENTRY DllMain(
	HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	return 1;
}