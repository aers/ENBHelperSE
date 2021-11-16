#include "functions.h"

extern "C" DLLEXPORT constexpr SKSE::PluginVersionData SKSEPlugin_Version = {
	.dataVersion = SKSE::PluginVersionData::kVersion,
	.pluginVersion = 2,
	.pluginName = "ENBHelperSE",
	.author = "aers",
	.supportEmail = "aers00@gmail.com",
	.compatibleVersions = { SKSE::RUNTIME_1_6_318.pack(), 0 },
	.xseMinimum = 0
};

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		return false;
	}

	*path /= Version::PROJECT;
	*path += ".log"sv;
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef NDEBUG
	log->set_level(spdlog::level::trace);
#else
	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::warn);
#endif

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);

	logger::info(FMT_STRING("{} v{}"), Version::PROJECT, Version::NAME);
	
	SKSE::Init(a_skse);

	isLoaded = true;
	
	logger::info("loaded");

	return true;
}
