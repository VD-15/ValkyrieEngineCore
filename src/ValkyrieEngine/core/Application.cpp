#include "Application.h"
#include "Event.hpp"

using namespace vlk;

namespace
{
	Boolean APPLICATION_RUNNING;
}

void Application::Run()
{
	APPLICATION_RUNNING = true;

	EventBus<ApplicationStartEvent>::Send();

	while (APPLICATION_RUNNING)
	{
		EventBus<PreUpdateEvent>::Send();
		EventBus<EarlyUpdateEvent>::Send();
		EventBus<UpdateEvent>::Send();
		EventBus<LateUpdateEvent>::Send();
		EventBus<PostUpdateEvent>::Send();
	}

	EventBus<ApplicationExitEvent>::Send();
}

void Application::Exit()
{
	APPLICATION_RUNNING = false;
}
