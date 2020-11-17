#include "ValkyrieEngine/ValkyrieEngine.hpp"

using namespace vlk;

namespace
{
	bool isRunning = false;
}

void Application::Start(const ApplicationArgs& args)
{
	isRunning = true;

	//TODO: No compiler currently supports <format>, so maybe hold off on that one, chief...
	//Log(std::format("{} Ver. {}.{}.{}.{}"), args.applicationName, args.verMajor, args.verMinor, args.verPatch, args.verPatch);
	Log(args.applicationName + " Ver. " + 
			std::to_string(args.verMajor) + "." + 
			std::to_string(args.verMinor) + "." + 
			std::to_string(args.verRevis) + "." + 
			std::to_string(args.verPatch), __FILE__, __LINE__);

	Log(args.developerName);
	Log("Starting...", __FILE__, __LINE__);

	SendEvent(ApplicationStartEvent{});

	while (isRunning)
	{
		Log<LogLevel::Trace>("Starting Update cycle", __FILE__, __LINE__); 
		SendEvent(PreUpdateEvent {});
		SendEvent(EarlyUpdateEvent {});
		SendEvent(UpdateEvent {});
		SendEvent(LateUpdateEvent {});
		SendEvent(PostUpdateEvent {});
	}
	
	Log("Exiting...", __FILE__, __LINE__);
	SendEvent(ApplicationExitEvent {});
	Log("Goodbye.", __FILE__, __LINE__);
}

void Application::Stop()
{
	isRunning = false;
}
