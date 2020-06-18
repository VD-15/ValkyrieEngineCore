#include "SampleEntity.hpp"
#include "catch2/catch.hpp"

using namespace vlk;

typedef SampleComponent::Allocator CAlloc;
typedef SampleEntity::Allocator EAlloc;

TEST_CASE("Entities are constructed and destroyed correctly")
{
	REQUIRE(CAlloc::Count() == 0);
	REQUIRE(EAlloc::Count() == 0);

	SampleEntity* e = new SampleEntity();

	REQUIRE(CAlloc::Count() == 1);
	REQUIRE(EAlloc::Count() == 1);

	delete e;

	REQUIRE(CAlloc::Count() == 0);
	REQUIRE(EAlloc::Count() == 0);
}

TEST_CASE("Components are added/removed properly")
{
	REQUIRE(CAlloc::Count() == 0);
	REQUIRE(EAlloc::Count() == 0);

	SampleEntity* e = new SampleEntity();

	REQUIRE(CAlloc::Count() == 1);
	REQUIRE(EAlloc::Count() == 1);

	SampleComponent* c = e->AddComponent<SampleComponent>();

	REQUIRE(CAlloc::Count() == 2);
	REQUIRE(EAlloc::Count() == 1);

	delete c;

	REQUIRE(CAlloc::Count() == 1);
	REQUIRE(EAlloc::Count() == 1);

	delete e;

	REQUIRE(CAlloc::Count() == 0);
	REQUIRE(EAlloc::Count() == 0);
}

TEST_CASE("Signals are sent properly")
{
	REQUIRE(CAlloc::Count() == 0);
	REQUIRE(EAlloc::Count() == 0);

	SampleEntity* e = new SampleEntity();

	REQUIRE(CAlloc::Count() == 1);
	REQUIRE(EAlloc::Count() == 1);
	REQUIRE(e->sampleComponent->i == 0);

	Signal testSignal("SIGNAL_TEST");
	testSignal.PutArg<int>("Value", 5);

	e->SendSignal(testSignal);

	REQUIRE(e->sampleComponent->i == 5);

	delete e;

	REQUIRE(CAlloc::Count() == 0);
	REQUIRE(EAlloc::Count() == 0);
}

TEST_CASE("Signals are sent properly to dynamically added components")
{
	REQUIRE(CAlloc::Count() == 0);
	REQUIRE(EAlloc::Count() == 0);

	SampleEntity* e = new SampleEntity();
	SampleComponent* c = e->AddComponent<SampleComponent>();

	REQUIRE(CAlloc::Count() == 2);
	REQUIRE(EAlloc::Count() == 1);
	REQUIRE(e->sampleComponent->i == 0);
	REQUIRE(c->i == 0);

	Signal testSignal("SIGNAL_TEST");
	testSignal.PutArg<int>("Value", 5);

	e->SendSignal(testSignal);

	REQUIRE(e->sampleComponent->i == 5);
	REQUIRE(c->i == 5);

	delete c;
	delete e;

	REQUIRE(CAlloc::Count() == 0);
	REQUIRE(EAlloc::Count() == 0);
}

TEST_CASE("Signals return empty with wrong type")
{
	REQUIRE(CAlloc::Count() == 0);
	REQUIRE(EAlloc::Count() == 0);

	SampleEntity* e = new SampleEntity();

	REQUIRE(CAlloc::Count() == 1);
	REQUIRE(EAlloc::Count() == 1);
	REQUIRE(e->sampleComponent->i == 0);

	Signal testSignal("SIGNAL_TEST");
	testSignal.PutArg<std::string>("Value", "Wrong type");

	e->SendSignal(testSignal);

	REQUIRE(e->sampleComponent->i == -1);

	delete e;

	REQUIRE(CAlloc::Count() == 0);
	REQUIRE(EAlloc::Count() == 0);
}

TEST_CASE("Signals return empty with no value")
{
	REQUIRE(CAlloc::Count() == 0);
	REQUIRE(EAlloc::Count() == 0);

	SampleEntity* e = new SampleEntity();

	REQUIRE(CAlloc::Count() == 1);
	REQUIRE(EAlloc::Count() == 1);
	REQUIRE(e->sampleComponent->i == 0);

	Signal testSignal("SIGNAL_TEST");

	e->SendSignal(testSignal);

	REQUIRE(e->sampleComponent->i == -1);

	delete e;

	REQUIRE(CAlloc::Count() == 0);
	REQUIRE(EAlloc::Count() == 0);
}

//Sync tests
