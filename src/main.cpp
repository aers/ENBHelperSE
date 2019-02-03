#include "skse64/PluginAPI.h"  // PluginHandle, SKSEMessagingInterface, SKSEInterface, PluginInfo
#include "skse64_common/skse_version.h"  // RUNTIME_VERSION

#include <ShlObj.h>  // CSIDL_MYDOCUMENTS

#include "version.h"  // ENBHELPERSE_VERSION_VERSTRING

#include "RE/Sky.h" // Sky
#include "RE/CommandTable.h"
#include "RE/ConsoleManager.h"

static PluginHandle	g_pluginHandle = kPluginHandle_Invalid;
static bool isLoaded = false;

extern "C" {
    bool IsLoaded()
    {
        return isLoaded;
    }

    bool GetCurrentWeather(uint32_t &currentWeatherFormID)
    {
        const auto sky = RE::Sky::GetSingleton();
        if (sky && sky->currentWeather)
        {
            currentWeatherFormID = sky->currentWeather->formID;
            return true;
        }
        return false;
    }

    bool GetOutgoingWeather(uint32_t &outgoingWeatherFormID)
    {
        const auto sky = RE::Sky::GetSingleton();
        if (sky && sky->outgoingWeather)
        {
            outgoingWeatherFormID = sky->outgoingWeather->formID;
            return true;
        }
        return false;
    }

    bool GetWeatherTransition(float &weatherTransition)
    {
        const auto sky = RE::Sky::GetSingleton();
        if (sky)
        {
            weatherTransition = sky->transition;
            return true;
        }
        return false;
    }

    bool GetTime(float &time)
    {
        const auto sky = RE::Sky::GetSingleton();
        if (sky)
        {
            time = sky->timeOfDay;
            return true;
        }
        return false;
    }

    bool Cmd_TestENBHelperSE_Execute(const RE::SCRIPT_PARAMETER* a_paramInfo, RE::CommandInfo::ScriptData* a_scriptData, RE::TESObjectREFR* a_thisObj, RE::TESObjectREFR* a_containingObj, RE::Script* a_scriptObj, ScriptLocals* a_locals, double& a_result, UInt32& a_opcodeOffsetPtr)
    {
        RE::ConsoleManager* console = RE::ConsoleManager::GetSingleton();

        if (console && RE::ConsoleManager::IsConsoleMode())
        {
            uint32_t currentWeatherFormId;
            uint32_t outgoingWeatherFormId;
            float weatherTransition;
            float time;

            console->Print("> [ENBHelperSE] Testing functions...");

            if (GetCurrentWeather(currentWeatherFormId))
                console->Print("> [ENBHelperSE] Current Weather FormID: %08X", currentWeatherFormId);
            else
                console->Print("> [ENBHelperSE] GetCurrentWeather failed");

            if (GetOutgoingWeather(outgoingWeatherFormId))
                console->Print("> [ENBHelperSE] Outgoing Weather FormID: %08X", outgoingWeatherFormId);
            else
                console->Print("> [ENBHelperSE] GetOutgoingWeather failed (will fail if no transition happening)");

            if (GetWeatherTransition(weatherTransition))
                console->Print("> [ENBHelperSE] Weather Transition: %f", weatherTransition);
            else
                console->Print("> [ENBHelperSE] GetWeatherTransition failed");

            if (GetTime(time))
                console->Print("> [ENBHelperSE] Time: %f", time);
            else
                console->Print("> [ENBHelperSE] GetTime failed");
        }
        return true;
    }

	bool SKSEPlugin_Query(const SKSEInterface* a_skse, PluginInfo* a_info)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\ENBHelperSE.log");
		gLog.SetPrintLevel(IDebugLog::kLevel_DebugMessage);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		_MESSAGE("ENBHelperSE v%s", ENBHELPERSE_VERSION_VERSTRING);

		a_info->infoVersion = PluginInfo::kInfoVersion;
		a_info->name = "ENBHelperSE";
		a_info->version = 1;

		g_pluginHandle = a_skse->GetPluginHandle();

		if (a_skse->isEditor) {
			_FATALERROR("[FATAL ERROR] Loaded in editor, marking as incompatible!\n");
			return false;
		}

		if (a_skse->runtimeVersion != RUNTIME_VERSION_1_5_62) {
			_FATALERROR("[FATAL ERROR] Unsupported runtime version %08X!\n", a_skse->runtimeVersion);
			return false;
		}

		return true;
	}

	bool SKSEPlugin_Load(const SKSEInterface* a_skse)
	{
        isLoaded = true;

        _MESSAGE("Registering test console command");

        typedef RE::SCRIPT_PARAMETER::Type Type;

        RE::CommandInfo* info = RE::CommandInfo::Locate("TestLocalMap");  // Unused
        if (info) {
            static RE::SCRIPT_PARAMETER params[] = {
                { "Name", Type::kString, 1 }
            };
            info->longName = "TestENBHelperSE";
            info->shortName = "tehs";
            info->helpText = "Test ENBHelperSE";
            info->isRefRequired = false;
            info->SetParameters(params);
            info->execute = &Cmd_TestENBHelperSE_Execute;
            info->eval = 0;

            _MESSAGE("[DEBUG] Registered console command: %s (%s)", info->longName, info->shortName);
        }
        else {
            _ERROR("[ERROR] Failed to register console command!\n");
        }

        _MESSAGE("Done");

		return true;
	}
};
