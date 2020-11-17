#include "SampleEntity.hpp"
#include "ValkyrieEngine/Component.hpp"
#include "catch2/catch.hpp"

#include <thread>
#include <chrono>

using namespace vlk;

void GenEntities()
{
	for (int i = 0; i < 10000; i++)
	{
		EntityID eId = Entity::Create();
	}
}

template <>
VLK_CXX14_CONSTEXPR inline ComponentHints vlk::GetComponentHints<OtherComponent>()
{
	return ComponentHints { 10, false };
}

TEST_CASE("Entity Counter Works Correctly")
{
	REQUIRE(Entity::Create() == static_cast<EntityID>(1));

	std::thread t1(GenEntities);
	std::thread t2(GenEntities);

	t1.join();
	t2.join();

	REQUIRE(Entity::Create() == static_cast<EntityID>(20002));
}

TEST_CASE("Component Allocator works as intended")
{
	REQUIRE(Component<SampleComponent>::Count() == 0);
	REQUIRE(Component<SampleComponent>::ChunkCount() == 0);

	std::vector<Component<SampleComponent>*> components;
	components.reserve(65);

	for (int i = 0; i < 65; i++)
	{
		components.emplace_back(Component<SampleComponent>::Create(Entity::Create()));
	}

	REQUIRE(Component<SampleComponent>::Count() == 65);
	REQUIRE(Component<SampleComponent>::ChunkCount() == 2);

	for (auto it = components.begin(); it != components.end(); it++)
	{
		(*it)->Delete();
	}

	REQUIRE(Component<SampleComponent>::Count() == 0);
	REQUIRE(Component<SampleComponent>::ChunkCount() == 0);
}

TEST_CASE("Component Hints work as intended")
{
	REQUIRE(Component<OtherComponent>::Count() == 0);
	REQUIRE(Component<OtherComponent>::ChunkCount() == 0);

	std::vector<Component<OtherComponent>*> components;
	components.reserve(10);

	for (int i = 0; i < 10; i++)
	{
		components.push_back(Component<OtherComponent>::Create(Entity::Create(), i));
	}

	REQUIRE(Component<OtherComponent>::Count() == 10);
	REQUIRE(Component<OtherComponent>::ChunkCount() == 1);

	REQUIRE_THROWS_AS(Component<OtherComponent>::Create(Entity::Create(), 20), std::range_error);

	for (auto it = components.begin(); it != components.end(); it++)
	{
		(*it)->Delete();
	}

	REQUIRE(Component<OtherComponent>::Count() == 0);
	REQUIRE(Component<OtherComponent>::ChunkCount() == 0);
}

TEST_CASE("Attaching Components works as intended")
{
	REQUIRE(Component<SampleComponent>::Count() == 0);
	REQUIRE(Component<SampleComponent>::ChunkCount() == 0);
	REQUIRE(Component<OtherComponent>::Count() == 0);
	REQUIRE(Component<OtherComponent>::ChunkCount() == 0);

	EntityID e1 = Entity::Create();
	EntityID e2 = Entity::Create();

	REQUIRE(e1 != e2);

	auto cs = Component<SampleComponent>::Create(e1);
	auto co = Component<OtherComponent>::Create(e2, 10);

	REQUIRE(cs->GetEntity() == e1);
	REQUIRE(co->GetEntity() == e2);

	REQUIRE(Component<SampleComponent>::FindOne(e1) == cs);
	REQUIRE(Component<SampleComponent>::FindOne(e2) == nullptr);
	REQUIRE(Component<OtherComponent>::FindOne(e2) == co);
	REQUIRE(Component<OtherComponent>::FindOne(e1) == nullptr);

	REQUIRE(Component<SampleComponent>::Count() == 1);
	REQUIRE(Component<SampleComponent>::ChunkCount() == 1);
	REQUIRE(Component<OtherComponent>::Count() == 1);
	REQUIRE(Component<OtherComponent>::ChunkCount() == 1);

	cs->Attach(e2);

	REQUIRE(cs->GetEntity() == e2);
	REQUIRE(co->GetEntity() == e2);

	REQUIRE(Component<SampleComponent>::FindOne(e1) == nullptr);
	REQUIRE(Component<SampleComponent>::FindOne(e2) == cs);
	REQUIRE(Component<OtherComponent>::FindOne(e2) == co);
	REQUIRE(Component<OtherComponent>::FindOne(e1) == nullptr);

	REQUIRE(Component<SampleComponent>::Count() == 1);
	REQUIRE(Component<SampleComponent>::ChunkCount() == 1);
	REQUIRE(Component<OtherComponent>::Count() == 1);
	REQUIRE(Component<OtherComponent>::ChunkCount() == 1);

	co->Attach(e1);

	REQUIRE(cs->GetEntity() == e2);
	REQUIRE(co->GetEntity() == e1);

	REQUIRE(Component<SampleComponent>::FindOne(e1) == nullptr);
	REQUIRE(Component<SampleComponent>::FindOne(e2) == cs);
	REQUIRE(Component<OtherComponent>::FindOne(e2) == nullptr);
	REQUIRE(Component<OtherComponent>::FindOne(e1) == co);

	REQUIRE(Component<SampleComponent>::Count() == 1);
	REQUIRE(Component<SampleComponent>::ChunkCount() == 1);
	REQUIRE(Component<OtherComponent>::Count() == 1);
	REQUIRE(Component<OtherComponent>::ChunkCount() == 1);

	Entity::Delete(e2);

	REQUIRE(Component<SampleComponent>::Count() == 0);
	REQUIRE(Component<SampleComponent>::ChunkCount() == 0);

	Entity::Delete(e1);

	REQUIRE(Component<OtherComponent>::Count() == 0);
	REQUIRE(Component<OtherComponent>::ChunkCount() == 0);
}

TEST_CASE("ForEach Works as intended")
{
	REQUIRE(Component<SampleComponent>::Count() == 0);
	REQUIRE(Component<SampleComponent>::ChunkCount() == 0);

	EntityID eId = Entity::Create();

	int t1 = 0;
	int t2 = 0;

	for (int i = 0; i < 200; i++)
	{
		auto c = Component<SampleComponent>::Create(eId);
		c->i = i;
		t1 += i;
	}

	Component<SampleComponent>::CForEach([&t2](const Component<SampleComponent>* c)
	{
		t2 += c->i;
	});

	REQUIRE(t1 == t2);

	Component<SampleComponent>::ForEach([](Component<SampleComponent>* c)
	{
		c->i = 10;
	});

	t2 = 0;

	Component<SampleComponent>::CForEach([&t2](const Component<SampleComponent>* c)
	{
		t2 += c->i;
	});

	REQUIRE(t2 == 2000);
	
	std::vector<Component<SampleComponent>*> components;
	components.reserve(200);

	Component<SampleComponent>::ForEach([&components](Component<SampleComponent>* c)
	{
		components.emplace_back(c);
	});

	REQUIRE(components.size() == 200);

	for (auto it = components.begin(); it != components.end(); it++)
	{
		if (std::distance(components.begin(), it) % 5 == 0)
		{
			(*it)->Delete();
		}
	}

	t2 = 0;

	Component<SampleComponent>::CForEach([&t2](const Component<SampleComponent>* c)
	{
		t2 += c->i;
	});

	REQUIRE(t2 == 1600);

	components.clear();

	Entity::Delete(eId);

	REQUIRE(Component<SampleComponent>::Count() == 0);
	REQUIRE(Component<SampleComponent>::ChunkCount() == 0);
}

TEST_CASE("Components are destructed properly")
{
	REQUIRE(Component<Counter>::Count() == 0);
	REQUIRE(Component<Counter>::ChunkCount() == 0);

	std::vector<Component<Counter>*> components;
	components.reserve(500);

	REQUIRE(Component<Counter>::GetNum() == 0);

	EntityID eId = Entity::Create();

	for (int i = 0; i < 500; i++)
	{
		components.push_back(Component<Counter>::Create(eId));
	}

	REQUIRE(Component<Counter>::GetNum() == 500);

	for (int i = 0; i < 100; i++)
	{
		components.at(i)->Delete();
	}

	REQUIRE(Component<Counter>::GetNum() == 400);

	Entity::Delete(eId);

	REQUIRE(Component<Counter>::GetNum() == 0);
	REQUIRE(Component<Counter>::Count() == 0);
	REQUIRE(Component<Counter>::ChunkCount() == 0);
}
