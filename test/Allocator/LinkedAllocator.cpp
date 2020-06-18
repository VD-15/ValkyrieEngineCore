#include "SampleData.hpp"
#include "ValkyrieEngine/LinkedAllocator.hpp"
#include "catch.hpp"

using namespace vlk;

using Allocator = LinkedAllocator<SampleData>;

TEST_CASE("LinkedAllocator meets the requirements for Allocator")
{
	REQUIRE(HasAllocateFunc<LinkedAllocator<SampleData>>::Value);
	REQUIRE(HasDeallocateFunc<LinkedAllocator<SampleData>>::Value);
	REQUIRE(HasForEachFunc<LinkedAllocator<SampleData>, SampleData>::Value);
	REQUIRE(HasFilterForEachFunc<LinkedAllocator<SampleData>, SampleData>::Value);
	REQUIRE(HasCountFunc<LinkedAllocator<SampleData>>::Value);
	REQUIRE(HasFilterCountFunc<LinkedAllocator<SampleData>, SampleData>::Value);
	REQUIRE(IsAllocator<LinkedAllocator<SampleData>, SampleData>::Value);
}

TEST_CASE("LinkedAllocators allocate and deallocate correctly")
{
	void* ptr = Allocator::Allocate();

	REQUIRE(ptr != nullptr);
	REQUIRE(Allocator::Count() == 1);

	Allocator::Deallocate(ptr);
	REQUIRE(Allocator::Count() == 0);
}

TEST_CASE("LinkedAllocator can allocate multiple instances")
{
	REQUIRE(Allocator::Count() == 0);

	std::vector<void*> allocs;
	allocs.reserve(10);

	for (vlk::Int i = 0; i < 10; i++)
	{
		void* ptr = Allocator::Allocate();
		allocs.push_back(ptr);
	}

	REQUIRE(Allocator::Count() == 10);

	for (Int i = 0; i < 5; i++)
	{
		Allocator::Deallocate(allocs.front());
		allocs.erase(allocs.begin());
	}

	REQUIRE(Allocator::Count() == 5);

	for (Int i = 0; i < 5; i++)
	{
		Allocator::Deallocate(allocs.front());
		allocs.erase(allocs.begin());
	}

	REQUIRE(Allocator::Count() == 0);
}

TEST_CASE("LinkedAllocator ForEach works")
{
	REQUIRE(Allocator::Count() == 0);

	std::vector<SampleData*> samples;
	samples.reserve(7);

	for (Int i = 0; i < 7; i++)
	{
		void* ptr = Allocator::Allocate();
		samples.push_back(new(ptr) SampleData(20));
	}

	Allocator::ForEach([](SampleData* s) -> void
	{
		s->data = 10;
	});

	for (auto it = samples.begin(); it != samples.end(); it++)
	{
		REQUIRE((*it)->data == 10);
		Allocator::Deallocate(reinterpret_cast<void*>(*it));
	}
}

TEST_CASE("LinkedAllocator filter ForEach works")
{
	REQUIRE(Allocator::Count() == 0);

	std::vector<SampleData*> samples;
	samples.reserve(7);

	for (Int i = 0; i < 7; i++)
	{
		void* ptr = Allocator::Allocate();
		samples.push_back(new(ptr) SampleData(i));
	}

	Allocator::ForEach(
	[](SampleData* s) -> void
	{
		s->data = -50;
	},
	[](const SampleData* s) -> bool
	{
		return (s->data % 2) == 0;
	});

	for (Int i = 0; i < 7; i++)
	{
		if (i % 2 == 0)
		{
			REQUIRE(samples[i]->data == -50);
		}
		else
		{
			REQUIRE(samples[i]->data == i);
		}

		Allocator::Deallocate(reinterpret_cast<void*>(samples[i]));
	}
}

TEST_CASE("LinkedAllocator filter Count works")
{
	REQUIRE(Allocator::Count() == 0);

	std::vector<SampleData*> samples;
	samples.reserve(10);

	for (int i = 0; i < 10; i++)
	{
		void* ptr = Allocator::Allocate();
		samples.push_back(new(ptr) SampleData((i < 5)? 10 : 20));
	}

	Size count = Allocator::Count([](const SampleData* s) -> bool
	{
		return s->data == 10;
	});

	REQUIRE(count == 5);

	for (int i = 0; i < 10; i++)
	{
		Allocator::Deallocate(samples[i]);
	}
}
