#include "ValkyrieEngine/AllocChunk.hpp"
#include "catch2/catch.hpp"

#include <set>

using namespace vlk;

Int numAllocated = 0;

class AllocData
{
	public:
	ULong data[4];

	AllocData()
	{
		numAllocated++;
	}

	~AllocData()
	{
		numAllocated--;
	}
};

typedef AllocChunk<AllocData, 32> AllocType;

TEST_CASE("AllocChunk Constructor")
{
	AllocType alloc;
	REQUIRE(alloc.Empty());
	REQUIRE(!alloc.Full());
	REQUIRE(alloc.Count() == 0);
	REQUIRE(numAllocated == 0);
}

TEST_CASE("AllocChunk allocates unique values")
{
	std::set<typename AllocType::PointerType> pointers;
	AllocType alloc;
	
	for (Size s = 0; s < AllocType::ChunkSize; s++)
	{
		auto p = alloc.Allocate();
		pointers.insert(p);
		new (p) AllocType::ValueType();

		for (int i = 0; i < 4; i++)
		{
			p->data[i] = (i + 1) * s;
		}
	}

	REQUIRE(pointers.size() == AllocType::ChunkSize);
	REQUIRE(numAllocated == AllocType::ChunkSize);

	for (auto it = pointers.begin(); it != pointers.end(); it++)
	{
		AllocType::PointerType p = *it;
		p->AllocType::ValueType::~ValueType();
		alloc.Deallocate(*it);
	}

	REQUIRE(numAllocated == 0);
}

TEST_CASE("Allocator occupation works properly")
{
	AllocType alloc;
	for (Size s = 0; s < AllocType::ChunkSize; s++)
	{
		REQUIRE(!alloc.IsOccupied(s));

		auto p = alloc.Allocate();
		new (p) AllocType::ValueType();

		for (int i = 0; i < 4; i++)
		{
			p->data[i] = (i + 1) * s;
		}

		REQUIRE(alloc.IsOccupied(s));
	}
	for (Size s = 0; s < AllocType::ChunkSize; s++)
	{
		auto p = alloc.At(s);

		for (int i = 0; i < 4; i++)
		{
			// Allocator assigns linearly so this should be okay
			REQUIRE(p->data[i] == (i + 1) * s);
		}

		alloc.Deallocate(p);
		REQUIRE(!alloc.IsOccupied(s));
	}
}

TEST_CASE("AllocChunk throws when full")
{
	AllocType alloc;

	for (Size s = 0; s < AllocType::ChunkSize; s++)
	{
		AllocType::PointerType p = alloc.Allocate();
		(void)p;
	}

	AllocType::PointerType p;

	REQUIRE(alloc.Full());
	REQUIRE(!alloc.Empty());
	REQUIRE(alloc.Count() == AllocType::ChunkSize);
	REQUIRE_THROWS_AS(p = alloc.Allocate(), std::bad_alloc);
}

TEST_CASE("Allocator owns its pointers")
{
	AllocType alloc;

	for (Size s = 0; s < AllocType::ChunkSize; s++)
	{
		REQUIRE(alloc.OwnsPointer(alloc.At(s)));
	}

	Byte* b = reinterpret_cast<Byte*>(alloc.At(0));
	REQUIRE(!alloc.OwnsPointer(reinterpret_cast<AllocType::PointerType>(b - 1)));

	b = reinterpret_cast<Byte*>(alloc.At(AllocType::ChunkSize - 1));
	REQUIRE(!alloc.OwnsPointer(reinterpret_cast<AllocType::PointerType>(b + 1)));
}
