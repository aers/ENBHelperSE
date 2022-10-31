#include "functions.h"

using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;


/**
    * Setup logging.
    *
    * <p>
    * Logging is important to track issues. CommonLibSSE bundles functionality for spdlog, a common C++ logging
    * framework. Here we initialize it, using values from the configuration file. This includes support for a debug
    * logger that shows output in your IDE when it has a debugger attached to Skyrim, as well as a file logger which
    * writes data to the standard SKSE logging directory at <code>Documents/My Games/Skyrim Special Edition/SKSE</code>
    * (or <code>Skyrim VR</code> if you are using VR).
    * </p>
    */
void InitializeLogging()
{
	auto path = log_directory();
	if (!path) {
		report_and_fail("Unable to lookup SKSE logs directory.");
	}
	*path /= PluginDeclaration::GetSingleton()->GetName();
	*path += L".log";

	std::shared_ptr<spdlog::logger> log;
	if (IsDebuggerPresent()) {
		log = std::make_shared<spdlog::logger>(
			"Global", std::make_shared<spdlog::sinks::msvc_sink_mt>());
	} else {
		log = std::make_shared<spdlog::logger>(
			"Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));
	}

	log->set_level({ spdlog::level::level_enum::info });
	log->flush_on({ spdlog::level::level_enum::trace });

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v");
}

SKSEPluginLoad(const LoadInterface* skse)
{
	InitializeLogging();

	auto* plugin = PluginDeclaration::GetSingleton();
	auto version = plugin->GetVersion();
	log::info("{} {} is loading...", plugin->GetName(), version);


	log::info("{} has finished loading.", plugin->GetName());
	return true;
}
