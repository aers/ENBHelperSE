bool isLoaded = false;

extern "C" DLLEXPORT bool IsLoaded()
{
	return isLoaded;
}

extern "C" DLLEXPORT bool GetCurrentWeather(std::uint32_t& currentWeatherFormID)
{
	const auto skyPtr = RE::Sky::GetSingleton();
	if (skyPtr && skyPtr->currentWeather) {
		currentWeatherFormID = skyPtr->currentWeather->formID;
		return true;
	}
	return false;
}

extern "C" DLLEXPORT bool GetOutgoingWeather(std::uint32_t& outgoingWeatherFormID)
{
	const auto skyPtr = RE::Sky::GetSingleton();
	if (skyPtr && skyPtr->lastWeather) {
		outgoingWeatherFormID = skyPtr->lastWeather->formID;
		return true;
	}
	return false;
}

extern "C" DLLEXPORT bool GetWeatherTransition(float& weatherTransition)
{
	const auto skyPtr = RE::Sky::GetSingleton();
	if (skyPtr) {
		weatherTransition = skyPtr->currentWeatherPct;
		return true;
	}
	return false;
}

extern "C" DLLEXPORT bool GetSkyMode(std::uint32_t& skyMode)
{
	const auto skyPtr = RE::Sky::GetSingleton();
	if (skyPtr) {
		skyMode = skyPtr->mode.underlying();
		return true;
	}
	return false;
}

extern "C" DLLEXPORT bool GetTime(float& time)
{
	const auto skyPtr = RE::Sky::GetSingleton();
	if (skyPtr) {
		time = skyPtr->currentGameHour;
		return true;
	}
	return false;
}

extern "C" DLLEXPORT bool GetCurrentLocationID(std::uint32_t& locationFormID)
{
	const auto playerPtr = RE::PlayerCharacter::GetSingleton();
	if (playerPtr && playerPtr->currentLocation) {
		locationFormID = playerPtr->currentLocation->formID;
		return true;
	}
	return false;
}

extern "C" DLLEXPORT bool GetWorldSpaceID(std::uint32_t& worldSpaceFormID)
{
	const auto playerPtr = RE::PlayerCharacter::GetSingleton();
	if (playerPtr && playerPtr->parentCell && (playerPtr->parentCell->cellFlags & RE::TESObjectCELL::Flag::kIsInteriorCell) != RE::TESObjectCELL::Flag::kNone) {
		worldSpaceFormID = 0x0;
		return true;
	}
	if (playerPtr && playerPtr->GetWorldspace()) {
		worldSpaceFormID = playerPtr->GetWorldspace()->formID;
		return true;
	}
	return false;
}

// Papyrus Weather.GetClassification
extern "C" DLLEXPORT int32_t GetClassification(RE::TESWeather* weather)
{
	typedef RE::TESWeather::WeatherDataFlag Flags;

	const auto flags = weather->data.flags;

	if ((flags & Flags::kPleasant) != Flags::kNone)
		return 0;
	if ((flags & Flags::kCloudy) != Flags::kNone)
		return 1;
	if ((flags & Flags::kRainy) != Flags::kNone)
		return 2;
	if ((flags & Flags::kSnow) != Flags::kNone)
		return 3;

	return 0xFFFFFFFF;
}

extern "C" DLLEXPORT bool GetCurrentWeatherClassification(int32_t& classification)
{
	const auto skyPtr = RE::Sky::GetSingleton();
	if (skyPtr && skyPtr->currentWeather) {
		classification = GetClassification(skyPtr->currentWeather);
		return true;
	}
	return false;
}

extern "C" DLLEXPORT bool GetOutgoingWeatherClassification(int32_t& classification)
{
	const auto skyPtr = RE::Sky::GetSingleton();
	if (skyPtr && skyPtr->lastWeather) {
		classification = GetClassification(skyPtr->lastWeather);
		return true;
	}
	return false;
}

extern "C" DLLEXPORT bool GetPlayerCameraTransformMatrices(RE::NiTransform& m_Local, RE::NiTransform& m_World, RE::NiTransform& m_OldWorld)
{
	const auto playerCamera = RE::PlayerCamera::GetSingleton();
	if (playerCamera && playerCamera->cameraRoot) {
		const auto cameraNode = playerCamera->cameraRoot;

		memcpy(&m_Local, &(cameraNode->local), sizeof(RE::NiTransform));
		memcpy(&m_World, &(cameraNode->world), sizeof(RE::NiTransform));
		memcpy(&m_OldWorld, &(cameraNode->previousWorld), sizeof(RE::NiTransform));

		return true;
	}
	return false;
}
