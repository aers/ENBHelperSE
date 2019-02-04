#include "skse64/PluginAPI.h"  // PluginHandle, SKSEMessagingInterface, SKSEInterface, PluginInfo
#include "skse64_common/skse_version.h"  // RUNTIME_VERSION

#include <ShlObj.h>  // CSIDL_MYDOCUMENTS

#include "version.h"  // ENBHELPERSE_VERSION_VERSTRING

#include "RE/Sky.h" // Sky
#include "RE/CommandTable.h"
#include "RE/ConsoleManager.h"

#include "skse64/GameAPI.h"
#include "skse64/GameReferences.h"
#include "skse64/GameCamera.h"
#include "skse64/NiNodes.h"


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

    bool GetSkyMode(uint32_t &skyMode)
    {
        const auto sky = RE::Sky::GetSingleton();
        if (sky)
        {
            skyMode = sky->skyMode;
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

    bool GetCurrentLocationID(uint32_t &locationFormID)
    {
        const auto player = *g_thePlayer;
        if (player && player->locationAC8)
        {
            locationFormID = player->locationAC8->formID;
            return true;
        }
        return false;
    }

    bool GetWorldSpaceID(uint32_t &worldSpaceFormID)
    {
        const auto player = *g_thePlayer;
        if (player && player->currentWorldSpace)
        {
            worldSpaceFormID = player->currentWorldSpace->formID;
            return true;
        }
        return false;
    }

    // Papyrus Weather.GetClassification 
    int32_t GetClassification(TESWeather * weather)
    {
        const auto flags = *((byte *)weather + 0x66F);

        if (flags & 1)
            return 0;
        if (flags & 2)
            return 1;
        if (flags & 4)
            return 2;
        if (flags & 8)
            return 3;

        return 0xFFFFFFFF;
    }

    bool GetCurrentWeatherClassification(int32_t &classification)
    {
        const auto sky = RE::Sky::GetSingleton();
        if (sky && sky->currentWeather)
        {
            classification = GetClassification(sky->currentWeather);
            return true;
        }
        return false;
    }

    bool GetOutgoingWeatherClassification(int32_t &classification)
    {
        const auto sky = RE::Sky::GetSingleton();
        if (sky && sky->outgoingWeather)
        {
            classification = GetClassification(sky->outgoingWeather);
            return true;
        }
        return false;
    }

    bool GetPlayerCameraTransformMatrices(NiTransform &m_Local, NiTransform &m_World, NiTransform &m_OldWorld)
    {
        const auto playerCamera = PlayerCamera::GetSingleton();
        if (playerCamera && playerCamera->cameraNode)
        {
            const auto cameraNode = playerCamera->cameraNode;
            
            memcpy(&m_Local, &(cameraNode->m_localTransform), sizeof(NiTransform));
            memcpy(&m_World, &(cameraNode->m_worldTransform), sizeof(NiTransform));
            memcpy(&m_OldWorld, &(cameraNode->m_oldWorldTransform), sizeof(NiTransform));

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
            int32_t currentWeatherClassification;
            uint32_t outgoingWeatherFormId;
            int32_t outgoingWeatherClassification;
            float weatherTransition;
            uint32_t skyMode;
            float time;
            uint32_t currentLocationFormID;
            uint32_t worldSpaceFormID;
            NiTransform m_local;
            NiTransform m_world;
            NiTransform m_oldWorld;

            console->Print("> [ENBHelperSE] Testing functions...");

            if (GetCurrentWeather(currentWeatherFormId))
                console->Print("> [ENBHelperSE] Current Weather FormID: %08X", currentWeatherFormId);
            else
                console->Print("> [ENBHelperSE] GetCurrentWeather failed");

            if (GetCurrentWeatherClassification(currentWeatherClassification))
                console->Print("> [ENBHelperSE] Current Weather Classification: %d", currentWeatherClassification);
            else
                console->Print("> [ENBHelperSE] GetCurrentWeatherClassification failed");

            if (GetOutgoingWeather(outgoingWeatherFormId))
                console->Print("> [ENBHelperSE] Outgoing Weather FormID: %08X", outgoingWeatherFormId);
            else
                console->Print("> [ENBHelperSE] GetOutgoingWeather failed (will fail if no transition happening)");

            if (GetOutgoingWeatherClassification(outgoingWeatherClassification))
                console->Print("> [ENBHelperSE] Outgoing Weather Classification: %d", outgoingWeatherClassification);
            else
                console->Print("> [ENBHelperSE] GetOutgoingWeatherClassification failed");

            if (GetWeatherTransition(weatherTransition))
                console->Print("> [ENBHelperSE] Weather Transition: %f", weatherTransition);
            else
                console->Print("> [ENBHelperSE] GetWeatherTransition failed");

            if (GetSkyMode(skyMode))
                console->Print("> [ENBHelperSE] Sky Mode: %d", skyMode);
            else
                console->Print("> [ENBHelperSE] GetSkyMode failed");

            if (GetTime(time))
                console->Print("> [ENBHelperSE] Time: %f", time);
            else
                console->Print("> [ENBHelperSE] GetTime failed");

            if (GetCurrentLocationID(currentLocationFormID))
                console->Print("> [ENBHelperSE] Current Location FormID: %08X", currentLocationFormID);
            else
                console->Print("> [ENBHelperSE] GetCurrentLocationID failed");

            if (GetWorldSpaceID(worldSpaceFormID))
                console->Print("> [ENBHelperSE] WorldSpace FormID: %08X", worldSpaceFormID);
            else
                console->Print("> [ENBHelperSE] GetWorldSpaceID failed");

            if (GetPlayerCameraTransformMatrices(m_local, m_world, m_oldWorld))
            {
                console->Print("> [ENBHelperSE] Player Camera Local NiTransform: rot %f %f %f, %f %f %f, %f %f %f pos %f %f %f scale %f", m_local.rot.arr[0], m_local.rot.arr[1], m_local.rot.arr[2], m_local.rot.arr[3], m_local.rot.arr[4], m_local.rot.arr[5], m_local.rot.arr[6], m_local.rot.arr[7], m_local.rot.arr[8], m_local.pos.x, m_local.pos.y, m_local.pos.z, m_local.scale);
                console->Print("> [ENBHelperSE] Player Camera World NiTransform: rot %f %f %f, %f %f %f, %f %f %f pos %f %f %f scale %f", m_world.rot.arr[0], m_world.rot.arr[1], m_world.rot.arr[2], m_world.rot.arr[3], m_world.rot.arr[4], m_world.rot.arr[5], m_world.rot.arr[6], m_world.rot.arr[7], m_world.rot.arr[8], m_world.pos.x, m_world.pos.y, m_world.pos.z, m_world.scale);
                console->Print("> [ENBHelperSE] Player Camera Old World NiTransform: rot %f %f %f, %f %f %f, %f %f %f pos %f %f %f scale %f", m_world.rot.arr[0], m_world.rot.arr[1], m_world.rot.arr[2], m_world.rot.arr[3], m_world.rot.arr[4], m_world.rot.arr[5], m_world.rot.arr[6], m_world.rot.arr[7], m_world.rot.arr[8], m_world.pos.x, m_world.pos.y, m_world.pos.z, m_world.scale);

            }
            else
                console->Print("> [ENBHelperSE] GetPlayerCameraTransformMatrices failed");
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
