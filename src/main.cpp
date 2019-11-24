#include "skse64/PluginAPI.h"  // PluginHandle, SKSEMessagingInterface, SKSEInterface, PluginInfo
#include "skse64_common/skse_version.h"  // RUNTIME_VERSION

#include <ShlObj.h>  // CSIDL_MYDOCUMENTS

#include "version.h"  // ENBHELPERSE_VERSION_VERSTRING

#include "SKSE/API.h"

#include "RE/CommandTable.h"
#include "RE/ConsoleManager.h"
#include "RE/PlayerCamera.h"
#include "RE/PlayerCharacter.h"
#include "RE/Sky.h"
#include "RE/TESWeather.h"
#include "RE/TESWorldSpace.h"
#include "RE/TESObjectCELL.h"

#include "skse64/NiNodes.h"

static bool isLoaded = false;

extern "C" {
bool IsLoaded()
{
    return isLoaded;
}

bool GetCurrentWeather(uint32_t& currentWeatherFormID)
{
    const auto skyPtr = RE::Sky::GetSingleton();
    if (skyPtr && skyPtr->currentWeather)
    {
        currentWeatherFormID = skyPtr->currentWeather->formID;
        return true;
    }
    return false;
}

bool GetOutgoingWeather(uint32_t& outgoingWeatherFormID)
{
    const auto skyPtr = RE::Sky::GetSingleton();
    if (skyPtr && skyPtr->outgoingWeather)
    {
        outgoingWeatherFormID = skyPtr->outgoingWeather->formID;
        return true;
    }
    return false;
}

bool GetWeatherTransition(float& weatherTransition)
{
    const auto skyPtr = RE::Sky::GetSingleton();
    if (skyPtr)
    {
        weatherTransition = skyPtr->transition;
        return true;
    }
    return false;
}

bool GetSkyMode(uint32_t& skyMode)
{
    const auto skyPtr = RE::Sky::GetSingleton();
    if (skyPtr)
    {
        skyMode = skyPtr->skyMode;
        return true;
    }
    return false;
}

bool GetTime(float& time)
{
    const auto skyPtr = RE::Sky::GetSingleton();
    if (skyPtr)
    {
        time = skyPtr->timeOfDay;
        return true;
    }
    return false;
}

bool GetCurrentLocationID(uint32_t& locationFormID)
{
    const auto playerPtr = RE::PlayerCharacter::GetSingleton();
    if (playerPtr && playerPtr->currentLocation)
    {
        locationFormID = reinterpret_cast<BGSLocation*>(playerPtr->currentLocation)->formID;
        return true;
    }
    return false;
}

bool GetWorldSpaceID(uint32_t& worldSpaceFormID)
{
    const auto playerPtr = RE::PlayerCharacter::GetSingleton();
    if (playerPtr && playerPtr->parentCell && (playerPtr->parentCell->flags & RE::TESObjectCELL::Flag::kIsInteriorCell)
        != RE::TESObjectCELL::Flag::kNone)
    {
        worldSpaceFormID = 0x0;
        return true;
    }
    if (playerPtr && playerPtr->currentWorldSpace)
    {
        worldSpaceFormID = playerPtr->currentWorldSpace->formID;
        return true;
    }
    return false;
}

// Papyrus Weather.GetClassification 
int32_t GetClassification(RE::TESWeather* weather)
{
    typedef RE::TESWeather::Data::Flag Flags;

    const auto flags = weather->data.flags;

    if ((flags & Flags::kWeather_Pleasant) != Flags::kNone)
        return 0;
    if ((flags & Flags::kWeather_Cloudy) != Flags::kNone)
        return 1;
    if ((flags & Flags::kWeather_Rainy) != Flags::kNone)
        return 2;
    if ((flags & Flags::kWeather_Snowy) != Flags::kNone)
        return 3;

    return 0xFFFFFFFF;
}

bool GetCurrentWeatherClassification(int32_t& classification)
{
    const auto skyPtr = RE::Sky::GetSingleton();
    if (skyPtr && skyPtr->currentWeather)
    {
        classification = GetClassification(skyPtr->currentWeather);
        return true;
    }
    return false;
}

bool GetOutgoingWeatherClassification(int32_t& classification)
{
    const auto skyPtr = RE::Sky::GetSingleton();
    if (skyPtr && skyPtr->outgoingWeather)
    {
        classification = GetClassification(skyPtr->outgoingWeather);
        return true;
    }
    return false;
}

bool GetPlayerCameraTransformMatrices(NiTransform& m_Local, NiTransform& m_World, NiTransform& m_OldWorld)
{
    const auto playerCamera = RE::PlayerCamera::GetSingleton();
    if (playerCamera && playerCamera->cameraNode)
    {
        const auto cameraNode = playerCamera->cameraNode;

        memcpy(&m_Local, &(cameraNode->localTransform), sizeof(NiTransform));
        memcpy(&m_World, &(cameraNode->worldTransform), sizeof(NiTransform));
        memcpy(&m_OldWorld, &(cameraNode->oldWorldTransform), sizeof(NiTransform));

        return true;
    }
    return false;
}

bool Cmd_TestENBHelperSE_Execute(const RE::SCRIPT_PARAMETER* a_paramInfo, RE::CommandInfo::ScriptData* a_scriptData,
                                 RE::TESObjectREFR* a_thisObj, RE::TESObjectREFR* a_containingObj,
                                 RE::Script* a_scriptObj, RE::ScriptLocals* a_locals, double& a_result,
                                 UInt32& a_opcodeOffsetPtr)
{
    const auto console = RE::ConsoleManager::GetSingleton();

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
            console->Print(
                "> [ENBHelperSE] Player Camera Local NiTransform: rot %f %f %f, %f %f %f, %f %f %f pos %f %f %f scale %f",
                m_local.rot.arr[0], m_local.rot.arr[1], m_local.rot.arr[2], m_local.rot.arr[3], m_local.rot.arr[4],
                m_local.rot.arr[5], m_local.rot.arr[6], m_local.rot.arr[7], m_local.rot.arr[8], m_local.pos.x,
                m_local.pos.y, m_local.pos.z, m_local.scale);
            console->Print(
                "> [ENBHelperSE] Player Camera World NiTransform: rot %f %f %f, %f %f %f, %f %f %f pos %f %f %f scale %f",
                m_world.rot.arr[0], m_world.rot.arr[1], m_world.rot.arr[2], m_world.rot.arr[3], m_world.rot.arr[4],
                m_world.rot.arr[5], m_world.rot.arr[6], m_world.rot.arr[7], m_world.rot.arr[8], m_world.pos.x,
                m_world.pos.y, m_world.pos.z, m_world.scale);
            console->Print(
                "> [ENBHelperSE] Player Camera Old World NiTransform: rot %f %f %f, %f %f %f, %f %f %f pos %f %f %f scale %f",
                m_world.rot.arr[0], m_world.rot.arr[1], m_world.rot.arr[2], m_world.rot.arr[3], m_world.rot.arr[4],
                m_world.rot.arr[5], m_world.rot.arr[6], m_world.rot.arr[7], m_world.rot.arr[8], m_world.pos.x,
                m_world.pos.y, m_world.pos.z, m_world.scale);
        }
        else
            console->Print("> [ENBHelperSE] GetPlayerCameraTransformMatrices failed");
    }
    return true;
}

bool SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	SKSE::Logger::OpenRelative(FOLDERID_Documents, R"(\My Games\Skyrim Special Edition\SKSE\ENBHelperSE.log)");
#ifdef _DEBUG
	SKSE::Logger::SetPrintLevel(SKSE::Logger::Level::kDebugMessage);
	SKSE::Logger::SetFlushLevel(SKSE::Logger::Level::kDebugMessage);
#else
	SKSE::Logger::SetPrintLevel(SKSE::Logger::Level::kMessage);
	SKSE::Logger::SetFlushLevel(SKSE::Logger::Level::kMessage);
#endif	
	SKSE::Logger::UseLogStamp(true);

    _MESSAGE("ENBHelperSE v%s", ENBHELPERSE_VERSION_VERSTRING);

    a_info->infoVersion = PluginInfo::kInfoVersion;
    a_info->name = "ENBHelperSE";
    a_info->version = ENBHELPERSE_VERSION_MAJOR;

    if (a_skse->IsEditor())
    {
        _FATALERROR("[FATAL ERROR] Loaded in editor, marking as incompatible!\n");
        return false;
    }

    if (a_skse->RuntimeVersion() != RUNTIME_VERSION_1_5_97)
    {
        _FATALERROR("[FATAL ERROR] Unsupported runtime version %08X!\n", a_skse->RuntimeVersion());
        return false;
    }

    return true;
}

bool SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
    
    _MESSAGE("Registering test console command");

    typedef RE::SCRIPT_PARAMETER::Type Type;

    RE::CommandInfo* info = RE::CommandInfo::LocateConsoleCommand("TestLocalMap"); // Unused
    if (info)
    {
        static RE::SCRIPT_PARAMETER params[] = {
            {"Name", Type::kString, 1}
        };
        info->longName = "TestENBHelperSE";
        info->shortName = "tehs";
        info->helpText = "Test ENBHelperSE";
        info->isRefRequired = false;
        info->SetParameters(params);
        info->execute = &Cmd_TestENBHelperSE_Execute;
        info->eval = nullptr;

        _MESSAGE("[DEBUG] Registered console command: %s (%s)", info->longName, info->shortName);
    }
    else
    {
        _ERROR("[ERROR] Failed to register console command!\n");
    }

    _MESSAGE("Done");
    
    isLoaded = true;

    return true;
}
};
