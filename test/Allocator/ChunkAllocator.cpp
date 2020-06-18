#include "ValkyrieEngine/ChunkAllocator.hpp"
#include "SampleData.hpp"
#include "catch.hpp"

using namespace vlk;

using Allocator = ChunkAllocator<SampleData, 10>;

TEST_CASE("ChunkAllocator meets the requirements for Allocator")
{
	REQUIRE(HasAllocateFunc<ChunkAllocator<SampleData, 10>>::Value);
	REQUIRE(HasDeallocateFunc<ChunkAllocator<SampleData, 10>>::Value);
	REQUIRE(HasForEachFunc<ChunkAllocator<SampleData, 10>, SampleData>::Value);
	REQUIRE(HasFilterForEachFunc<ChunkAllocator<SampleData, 10>, SampleData>::Value);
	REQUIRE(HasCountFunc<ChunkAllocator<SampleData, 10>>::Value);
	REQUIRE(HasFilterCountFunc<ChunkAllocator<SampleData, 10>, SampleData>::Value);
	REQUIRE(IsAllocator<ChunkAllocator<SampleData, 10>, SampleData>::Value);
}

TEST_CASE("ChunkAllocators allocate and deallocate correctly")
{
	REQUIRE(Allocator::Count() == 0);
	REQUIRE(Allocator::ChunkCount() == 0);

	void* ptr = Allocator::Allocate();

	REQUIRE(ptr != nullptr);
	REQUIRE(Allocator::Count() == 1);
	REQUIRE(Allocator::ChunkCount() == 1);

	Allocator::Deallocate(ptr);

	REQUIRE(Allocator::Count() == 0);
	REQUIRE(Allocator::ChunkCount() == 0);
}

TEST_CASE("ChunkAllocator creates new chunk when full")
{
	REQUIRE(Allocator::Count() == 0);
	REQUIRE(Allocator::ChunkCount() == 0);

	std::vector<void*> allocs;
	allocs.reserve(11);

	for (vlk::Int i = 0; i < 10; i++)
	{
		void* ptr = Allocator::Allocate();
		allocs.push_back(ptr);
	}

	REQUIRE(Allocator::Count() == 10);
	REQUIRE(Allocator::ChunkCount() == 1);

	allocs.push_back(Allocator::Allocate());

	REQUIRE(Allocator::Count() == 11);
	REQUIRE(Allocator::ChunkCount() == 2);

	for (auto it = allocs.begin(); it != allocs.end(); it++)
	{
		Allocator::Deallocate(*it);
	}

	REQUIRE(Allocator::Count() == 0);
	REQUIRE(Allocator::ChunkCount() == 0);
}

TEST_CASE("ChunkAllocator ForEach works")
{
	REQUIRE(Allocator::Count() == 0);

	std::vector<SampleData*> samples;
	samples.reserve(17);

	for (Int i = 0; i < 17; i++)
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

TEST_CASE("ChunkAllocator filter ForEach works")
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

TEST_CASE("ChunkAllocator filter Count works")
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
