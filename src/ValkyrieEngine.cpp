#include "ValkyrieEngine/ValkyrieEngine.hpp"

using namespace vlk;

void Application::Start(const ApplicationArgs& args)
{
	Application::isRunning = true;

	//TODO: No compiler currently supports <format>, so maybe hold off on that one, chief...
	//Log(std::format("{} Ver. {}.{}.{}.{}"), args.applicationName, args.verMajor, args.verMinor, args.verPatch, args.verPatch);
	Log(args.applicationName + " Ver. " + 
			std::to_string(args.verMajor) + "." + 
			std::to_string(args.verMinor) + "." + 
			std::to_string(args.verRevis) + "." + 
			std::to_string(args.verPatch));

	Log(args.developerName);
	Log("Starting...");

	SendEvent(ApplicationStartEvent{});

	while (isRunning)
	{
		SendEvent(PreUpdateEvent {});
		SendEvent(EarlyUpdateEvent {});
		SendEvent(UpdateEvent {});
		SendEvent(LateUpdateEvent {});
		SendEvent(PostUpdateEvent {});
	}
	
	Log<>("Exiting...");
	SendEvent(ApplicationExitEvent {});
	Log<>("Goodbye.");
}

void Application::Stop()
{
	Application::isRunning = false;
}
