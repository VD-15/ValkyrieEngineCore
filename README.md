# Valkyrie Engine

| | |
|-|-|
|Release|Probably| |
|Build Status|Currently on fire|
|Coverage|Not a whole lot|
|Read the Issues|Please|

Valkyrie Engine is a game engine written in modern C++ that is designed to be as modular as possible and as lightweight as you want.

Valkyrie Engine's Core module is designed to be small and easy to implement with no dependencies outside of the C++ standard library. Most of the functionality that you would expect from a game engine is defined across Valkyrie Engine's various modules. These modules embody the Unix philosophy of performing a single, well defined task as part of a larger program/system.

## 1 Manifesto

The supply of video games in the current market is increasing at a rate that is far outpacing the demand. Consumers are beginning to raise their overall standards for quality as various niches are quickly becoming saturated. As games become more complex and their specifications more refined, it becomes increasingly difficult to produce a single tool that adequately meets this wide range of requirements and yet most publicly-available engines are trying to do exactly that. 

This is where Valkyrie Engine comes in. Instead of giving developers one general-purpose system, Valkyrie Engine is designed to give them a choice between several specialised systems so they can use the best option for their game. Valkyrie Engine lets the developer decide how it's implemented and in what capacity. You can mix-and-match existing modules and use Valkyrie Engine as a fully-featured game engine or you can strip all the modules out and elect to make your own.

## 2 Documentation

> See issue #7

## 3 Examples

### 3.1 Initialization

Valkyrie Engine doesn't require much in terms of initialization, currently it only requires a call to `vlk::Init()` and `vlk::Destroy()` either side of your program. `vlk::Application::Run()` is the function that starts the main event loop. This loop will run continuously until `vlk::Application::Destroy()` is called, in which case it will exit once the current update loop is completed.

```cpp
#include "ValkyrieEngine/core/Application.h"

int main()
{
    vlk::Init();

    vlk::Application::Run();

    vlk::Destroy();
}
```

### 3.2 Events

In Valkyrie Engine, events are the primary way that inter-module communication occurs. Instead of calling a function from another module directly, send an event. Events help keep Valkyrie Engine modular and your code maintainable, both by yourself and others.

#### 3.2.1 Creating Events

In Valkyrie Engine, any object can be an event, though usually they are just POD structs. 

```cpp
struct MyEvent
{
    const vlk::Int foo;
    const vlk::Float bar;
};
```

#### 3.2.2 Pre-Defined Events

Valkyrie Engine Core comes with a few events that help form the main update loop of the engine. All of these events are simply empty structs with no data but their usage varies significantly:

* `ApplicationStartEvent` is sent once, just before the first event loop of the application runs. This is used primarily for second-stage module initialization and to set up the initial state of your game.
* `ApplicationExitEvent` is sent once, after the event loop exits and the application is due to terminate. This is used to perform cleanup on any entities or systems that need it.
* `PreUpdateEvent` is sent at the beginning of every update loop but should not be considered part of it. It is mostly used to update modules and should not be used to change the state of your game.
* `EarlyUpdateEvent` is sent after `PreUpdateEvent` and is mostly used to read the state of other entities before `UpdateEvent` is called to help prevent race conditions.
* `UpdateEvent` is sent after `EarlyUpdateEvent` and is used to update the state of entities present in the game.
* `LateUpdateEvent` is sent after `UpdateEvent` and is mostly used to react to the changes that occured during the `UpdateEvent`, again, this is to help eliminate race conditions.
* `PostUpdateEvent` is sent after `LateUpdateEvent` and is similar to the `PreUpdateEvent` in that it is mostly used to update modules and should not be used to alter the game state in any way.

> See issue

#### 3.2.3 Sending Events

Sending events in Valkyrie Engine is done through the `EventBus` class. This will raise the event handler for every listener of your event.

```cpp
vlk::EventBus<MyEvent>::Send( {26, 49.0f} );
```

#### 3.2.4 Recieving Events

In order to recieve an event, you need to define a class that inherits from the `EventListener` interface and construct an instance of it.

```cpp
class MyEventListener : public EventListener<MyEvent>
{
    /* ... */

    void OnEvent(const MyEvent& ev) final override
    {
        /* ... */
    }
};
```

#### 3.2.5 Putting it all together

```cpp
#include "ValkyrieEngine/core/Event.hpp"
#include <iostream>
#include <string>

struct MyEvent
{
    const vlk::Int foo;
    const std::string bar;
};

class MyEventListener : public vlk::EventListener<MyEvent>
{
    public:
    MyEventListener() = default;

    private:
    void OnEvent(const MyEvent& ev) final override
    {
        std::cout << "Event handler raised: " << std::to_string(ev.foo) << std::endl;
        std::cout << "Cow says: " << ev.bar << std::endl;
    }
};

int main()
{
    vlk::Init();

    MyEventListener listener;

    //Raises MyEventListener::OnEvent(const MyEvent&)
    vlk::EventBus<MyEvent>::Send( { 51, "Moo!" } );

    vlk::Destroy();
}
```

### 3.3 Modules

> See issue #9

### 3.4 Entity-Component System

Valkyrie Engine core defines a basic entity-component system that serves as the underlying framework for any game produced in it.

#### 3.4.1 Components

Components are data types that define a particular behaviour or property for the entity they're attached to. It is important to note that components shouldn't be event listeners. This is mostly to avoid congestion on the event bus as constantly adding and removing event listeners can significantly impact performance.

The constructor for `Component` takes a single pointer to the entity the component belongs to in the form of an `IEntity*`. Any constructors for your component **must** accept an `IEntity*` as their first argument. Components should be deleted through a member function called `Delete()` as oppose to using the standard C++ `delete` operator.

```cpp
#include "ValkyrieEngine/core/GameComponent.hpp"

class MyComponent : public vlk::Component<MyComponent>
{
    public:
    MyComponent(IEntity* e, vlk::UInt _score) :
        Component<MyComponent>(e)
    {
        this->score = _score;
    }

    vlk::UInt score;
};
```

#### 3.4.2 Entities

Entities are simply data types used to composite several components into one structure, they will usually not hold any data aside from components. `Entity` defines several member functions, one of which is the main way of attaching components to an entity. Similar to components, entities should be deleted through the member function `Delete()` as oppose to using the standard C++ `delete` operator. Entities are created with the static `CreateEntity(...)` member function.

```cpp
#include "ValkyrieEngine/core/GameEntity.hpp"

class MyEntity : public vlk::Entity<MyEntity>
{
    public:
    MyEntity()
    {
        //Calls constructor MyComponent::MyComponent(IEntity*, vlk::UInt);
        myComponent = AddComponent<MyComponent>(10);
    }

    ~MyEntity()
    {
        //Calls MyComponent::~MyComponent() and frees allocated memory
        myComponent->Delete();
    }

    private:
    MyComponent* myComponent;
};
```

#### 3.4.3 Systems

Where components define behaviour for entities, systems are where it's defined. Most systems will take the form of event listeners with any necessary data stored in member variables or the like. Since systems perform operations on/with components, they need a way to access every instance of that component at runtime. `Component` has a static member function called `ForEach` that does exactly that. It accepts a const reference to anything that can serve as an std::function, usually lambdas.

```
#include "ValkyrieEngine/core/Event.hpp"

class MySystem : public vlk::EventListener<vlk::UpdateEvent>
{
    public:
    MySystem() = default;

    private:
    void OnEvent(const vlk::UpdateEvent& ev) final override
    {
        MyComponent::ForEach([](MyComponent* c)
        {
            c->score++;
        });
    }
};
```

#### 3.4.4 Putting it all together

```cpp
#include "ValkyrieEngine/core/Application.h"
#include "ValkyrieEngine/core/GameComponent.hpp"
#include "ValkyrieEngine/core/GameEntity.hpp"
#include "ValkyrieEngine/core/Event.hpp"

class MyComponent : public vlk::Component<MyComponent>
{
    public:
    MyComponent(vlk::IEntity* e, vlk::UInt _score) :
        Component<MyComponent>(e)
    {
        this->score = _score;
    }

    vlk::UInt score;
};

class MyEntity : public vlk::Entity<MyEntity>
{
    public:
    MyEntity()
    {
        this->myComponent = AddComponent<MyComponent>(10);
    }

    ~MyEntity()
    {
        this->myComponent->Delete();
    }

    private:
    MyComponent* myComponent;
};

class MySystem : public vlk::EventListener<vlk::UpdateEvent>
{
    public:
    MySystem() = default;

    private:
    void OnEvent(const vlk::UpdateEvent& ev) final override
    {
        MyComponent::ForEach([](MyComponent* c)
        {
            c->score++;
        });
    }
};

int main()
{
    MySystem mySys;

    vlk::Init();

    vlk::IEntity* myEntity = MyEntity::CreateEntity();

    Application::Run();

    myEntity->Delete();

    vlk::Destroy();
}
```

## 4 Contribution

### 4.1 Contributors

### 4.2 How to Contribute

#### 4.2.1 Style
