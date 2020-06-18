#include "SampleData.hpp"
#include "ValkyrieEngine/SingletonAllocator.hpp"
#include "catch.hpp"

using namespace vlk;

using Allocator = SingletonAllocator<SampleData>;

TEST_CASE("SingletonAllocator meets the requirements for Allocator")
{
	REQUIRE(HasAllocateFunc<SingletonAllocator<SampleData>>::Value);
	REQUIRE(HasDeallocateFunc<SingletonAllocator<SampleData>>::Value);
	REQUIRE(HasForEachFunc<SingletonAllocator<SampleData>, SampleData>::Value);
	REQUIRE(HasFilterForEachFunc<SingletonAllocator<SampleData>, SampleData>::Value);
	REQUIRE(HasCountFunc<SingletonAllocator<SampleData>>::Value);
	REQUIRE(HasFilterCountFunc<SingletonAllocator<SampleData>, SampleData>::Value);
	REQUIRE(IsAllocator<SingletonAllocator<SampleData>, SampleData>::Value);
}

TEST_CASE("SingletonAllocators allocate and deallocate correctly")
{
	void* ptr = Allocator::Allocate();

	REQUIRE(ptr != nullptr);
	REQUIRE(Allocator::Count() == 1);

	Allocator::Deallocate(ptr);
	REQUIRE(Allocator::Count() == 0);
}

TEST_CASE("SingletonAllocator throws when full")
{
	REQUIRE(Allocator::Count() == 0);

	void* ptr = Allocator::Allocate();

	REQUIRE_THROWS_AS(
		Allocator::Allocate(),
		std::runtime_error
	);

	REQUIRE(Allocator::Count() == 1);

	Allocator::Deallocate(ptr);

	REQUIRE(Allocator::Count() == 0);
}

TEST_CASE("SingletonAllocator ForEach works")
{
	REQUIRE(Allocator::Count() == 0);

	void* ptr = Allocator::Allocate();
	SampleData* sample = new(ptr) SampleData(20);

	Allocator::ForEach([](SampleData* s) -> void
	{
		s->data = 10;
	});

	REQUIRE(sample->data == 10);

	Allocator::Deallocate(ptr);

	REQUIRE(Allocator::Count() == 0);
}

TEST_CASE("SingletonAllocator filter ForEach works")
{
	REQUIRE(Allocator::Count() == 0);

	void* ptr = Allocator::Allocate();
	SampleData* sample = new(ptr) SampleData(20);

	Allocator::ForEach([](SampleData* s) -> void
	{
		s->data = 10;
	},
	[](const SampleData* s) -> bool
	{
		return s->data == 15;
	});

	REQUIRE(sample->data == 20);

	Allocator::ForEach([](SampleData* s) -> void
	{
		s->data = 10;
	},
	[](const SampleData* s) -> bool
	{
		return s->data == 20;
	});

	REQUIRE(sample->data == 10);

	Allocator::Deallocate(ptr);

	REQUIRE(Allocator::Count() == 0);
}

TEST_CASE("SingletonAllocator filter Count works")
{
	REQUIRE(Allocator::Count() == 0);

	void* ptr = Allocator::Allocate();
	SampleData* sample = new(ptr) SampleData(20);

	Size count = Allocator::Count([](const SampleData* s) -> bool
	{
		return s->data == 10;
	});

	REQUIRE(count == 0);

	count = Allocator::Count([](const SampleData* s) -> bool
	{
		return s->data == 20;
	});

	REQUIRE(count == 1);

	Allocator::Deallocate(sample);
}
