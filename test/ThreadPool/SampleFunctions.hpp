#include "ValkyrieEngine/ValkyrieEngine.hpp"
#include "catch.hpp"

class Tracker
{
	public:
	static vlk::UInt present_counter;
	static vlk::UInt max_counter;
	static vlk::UInt total_counter;
	static std::mutex mtx;
	
	static void DoThing()
	{
		std::unique_lock ulock(mtx);

		present_counter++;
		total_counter++;
		max_counter = std::max(max_counter, present_counter);
		
		ulock.unlock();

		std::this_thread::sleep_for(std::chrono::seconds(1));

		ulock.lock();
		present_counter--;
	}
};

vlk::UInt Tracker::present_counter = 0;
vlk::UInt Tracker::max_counter = 0;
vlk::UInt Tracker::total_counter = 0;
std::mutex Tracker::mtx;
