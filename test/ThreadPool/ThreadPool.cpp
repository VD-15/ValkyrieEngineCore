#include "SampleFunctions.hpp"
#include <iostream>

TEST_CASE("ThreadPool distributes functions properly")
{
	for (vlk::UInt i = 0; i < std::thread::hardware_concurrency() * 2; i++)
	{
		vlk::ThreadPool::Enqueue(Tracker::DoThing);
	}

	vlk::ThreadPool::Wait();
	std::unique_lock ulock(Tracker::mtx);
	REQUIRE(Tracker::max_counter == std::thread::hardware_concurrency());
	REQUIRE(Tracker::total_counter == std::thread::hardware_concurrency() * 2);
	ulock.unlock();

	for (vlk::UInt i = 0; i < std::thread::hardware_concurrency() * 5; i++)
	{
		vlk::ThreadPool::Enqueue(Tracker::DoThing);
	}

	vlk::ThreadPool::Wait();
	ulock.lock();
	REQUIRE(Tracker::max_counter == std::thread::hardware_concurrency());
	REQUIRE(Tracker::total_counter == std::thread::hardware_concurrency() * 7);
}
