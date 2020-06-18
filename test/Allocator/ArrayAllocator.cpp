#include "SampleData.hpp"
#include "ValkyrieEngine/ArrayAllocator.hpp"
#include "catch.hpp"

using namespace vlk;

using Allocator = ArrayAllocator<SampleData, 10>;

TEST_CASE("ArrayAllocator meets the requirements for Allocator")
{
	REQUIRE(HasAllocateFunc<ArrayAllocator<SampleData, 10>>::Value);
	REQUIRE(HasDeallocateFunc<ArrayAllocator<SampleData, 10>>::Value);
	REQUIRE(HasForEachFunc<ArrayAllocator<SampleData, 10>, SampleData>::Value);
	REQUIRE(HasFilterForEachFunc<ArrayAllocator<SampleData, 10>, SampleData>::Value);
	REQUIRE(HasCountFunc<ArrayAllocator<SampleData, 10>>::Value);
	REQUIRE(HasFilterCountFunc<ArrayAllocator<SampleData, 10>, SampleData>::Value);
	REQUIRE(IsAllocator<ArrayAllocator<SampleData, 10>, SampleData>::Value);
}

TEST_CASE("ArrayAllocators allocate and deallocate correctly")
{
	void* ptr = Allocator::Allocate();

	REQUIRE(ptr != nullptr);
	REQUIRE(Allocator::Count() == 1);

	Allocator::Deallocate(ptr);
	REQUIRE(Allocator::Count() == 0);
}

TEST_CASE("ArrayAllocator throws when full")
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

	REQUIRE_THROWS_AS(
	Allocator::Allocate(),
	std::bad_alloc
	);

	for (auto it = allocs.begin(); it != allocs.end(); it++)
	{
		Allocator::Deallocate(*it);
	}
}

TEST_CASE("ArrayAllocator ForEach works")
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

TEST_CASE("ArrayAllocator filter ForEach works")
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

TEST_CASE("ArrayAllocator filter Count works")
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

