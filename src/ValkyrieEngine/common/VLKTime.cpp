#include "VLKTime.h"
#include "../core/Event.hpp"

#include <chrono>

using namespace vlk;

namespace
{
	std::chrono::high_resolution_clock::time_point lastTick;
	std::chrono::high_resolution_clock::time_point thisTick;
	Double deltaTime = 0.0;

	class TimeSystem : 
		public EventListener<ApplicationStartEvent>,
		public EventListener<PreUpdateEvent>
	{
		void OnEvent(const ApplicationStartEvent& ev) final override
		{
			lastTick = std::chrono::high_resolution_clock::now();
			thisTick = lastTick;
		}

		void OnEvent(const PreUpdateEvent& ev) final override
		{
			LogTrace("VLKTime", "Ticking.");
			lastTick = thisTick;
			thisTick = std::chrono::high_resolution_clock::now();
			deltaTime = std::chrono::duration_cast<std::chrono::duration<Double>>(thisTick - lastTick).count();
		}
	};

	TimeSystem* sys;
}

VLKTime::VLKTime(const VLKTimeArgs& a) :
	args(a)
{
	if (args.isFixedTimestep)
	{
		lastTick = std::chrono::high_resolution_clock::time_point::min();
		thisTick = lastTick + std::chrono::nanoseconds(args.fixedTimestep);

		deltaTime = std::chrono::duration_cast<std::chrono::duration<Double>>(thisTick - lastTick).count();

	}
	else
	{
		deltaTime = 0.0;

		sys = new TimeSystem();
	}
}

VLKTime::~VLKTime()
{

}

template<>
Double VLKTime::DeltaTime<Double>()
{
	return deltaTime;
}

template<>
Float VLKTime::DeltaTime<Float>()
{
	return static_cast<Float>(deltaTime);
}