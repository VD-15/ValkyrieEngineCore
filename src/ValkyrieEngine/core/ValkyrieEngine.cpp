#include "ValkyrieEngine.h"
#include "Module.hpp"
#include "Event.hpp"

#include <sstream>
#include <iomanip>
#include <map>

using namespace vlk;

namespace
{
	Boolean VLK_INITIALIZED = false;

	std::vector<IModuleQueue*> modules;
}

void vlk::RegisterModuleQueue(IModuleQueue* m)
{
	if (!VLK_INITIALIZED)
	{
		modules.push_back(m);
	}
	else
	{
		throw std::exception("Modules canntot be registered once VLKInit() has been called.");
	}
}

void vlk::Init()
{
	if (!VLK_INITIALIZED)
	{
		for (auto it = modules.begin(); it != modules.end(); it++)
		{
			(*it)->Init();
		}

		/*
		if (InitFlagRaised(MODULE_RANDOM)) VLKRandom::Init();
		if (InitFlagRaised(MODULE_WINDOW)) Window::Init();
		if (InitFlagRaised(MODULE_KEYBOARD)) Keyboard::Init();
		if (InitFlagRaised(MODULE_MOUSE)) Mouse::Init();
		if (InitFlagRaised(MODULE_OPENGL)) GLRenderer::Init();*/

		VLK_INITIALIZED = true;
	}
}

void vlk::Destroy()
{
	if (VLK_INITIALIZED)
	{
		for (auto it = modules.begin(); it != modules.end(); it++)
		{
			(*it)->Destroy();
		}

		/*
		GLRenderer::Destroy();
		Mouse::Destroy();
		Keyboard::Destroy();
		Window::Destroy();
		VLKRandom::Destroy();
		VLKTime::Destroy();*/

		VLK_INITIALIZED = false;
	}
}
/*
//TODO: inline these
void vlk::LogTrace(const std::string& source, const std::string& message)
{
	EventBus<LogEvent>::Send(LogEvent { source, vlk::LOG_LEVEL_TRACE, message });
}

void vlk::LogVerbose(const std::string& source, const std::string& message)
{
	EventBus<LogEvent>::Send(LogEvent { source, vlk::LOG_LEVEL_VERBOSE, message });
}

void vlk::LogInfo(const std::string& source, const std::string& message)
{
	EventBus<LogEvent>::Send(LogEvent { source, vlk::LOG_LEVEL_INFO, message });
}

void vlk::LogWarning(const std::string& source, const std::string& message)
{
	EventBus<LogEvent>::Send(LogEvent { source, vlk::LOG_LEVEL_WARNING, message });
}

void vlk::LogError(const std::string& source, const std::string& message)
{
	EventBus<LogEvent>::Send(LogEvent { source, vlk::LOG_LEVEL_ERROR, message });
}

void vlk::LogFatal(const std::string& source, const std::string& message)
{
	EventBus<LogEvent>::Send(LogEvent { source, vlk::LOG_LEVEL_FATAL, message });
}

void vlk::LogAnnounce(const std::string& source, const std::string& message)
{
	EventBus<LogEvent>::Send(LogEvent { source, vlk::LOG_LEVEL_ANNOUNCE, message });
}*/

std::string vlk::PointerToString(const void* ptr)
{
	std::stringstream ss;
	ss << "0x" << std::hex << ptr;
	return ss.str();
}

std::string vlk::TypeInfoToString(const std::type_info& t)
{
	std::string s(t.name());
	std::stringstream ss(s.substr(6, std::string::npos));
	std::string item;
	while (std::getline(ss, item, ':')) {}
	return item;
}

Boolean vlk::StringEndsWith(const std::string& str, const std::string& end)
{
	if (end.size() > str.size()) return false;
	return std::equal(end.crbegin(), end.crend(), str.crbegin());
}

Boolean vlk::StringBeginsWith(const std::string& str, const std::string& beg)
{
	if (beg.size() > str.size()) return false;
	return std::equal(beg.cbegin(), beg.cend(), str.cbegin());
}