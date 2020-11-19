/*!
 * \file ValkyrieEngine.hpp
 * \brief Valkyrie Engine's primary header file
 */

#ifndef VLK_ENGINE_H
#define VLK_ENGINE_H

#include "ValkyrieEngine/ValkyrieDebug.hpp"
#include "ValkyrieEngine/Component.hpp"
#include "ValkyrieEngine/EventBus.hpp"

/*!
 * \mainpage ValkyrieEngine Core
 * \tableofcontents
 *
 * <h1>Valkyrie Engine</h1>
 * Valkyrie Engine is a game engine written in modern C++ that is designed to be as modular as possible and as lightweight as you want.
 * Valkyrie Engine's Core module is designed to be small and easy to implement. Most of the functionality that you would expect from a game engine is defined across Valkyrie Engine's various modules. These modules are collections of systems, events and components designed to help accomplish a specific functionality or behaviour within a game. 
 * 
 * \section quick_start Quick Start Guide
 * \subsection init Initialization
 *
 * Valkyrie Engine doesn't require much in terms of initialization, currently it only requires a call to `vlk::Application::Start()` at the beginning of your program. This is the function that starts the main update loop. This loop will run continuously until `vlk::Application::Stop()` is called, in which case it will exit once the current update loop is completed.
 * \code{.cpp}
 * #include "ValkyrieEngine/ValkyrieEngine.hpp"
 *
 * int main()
 * {
 *     vlk::ApplicationArgs args{};
 *     args.applicationName = "My Game";
 *     vlk::Application::Start(args);
 *  }
 *  \endcode
 *
 * \subsection event Events
 *
 * In Valkyrie Engine, events are the primary way that inter-module communication occurs. Instead of calling a function from another module directly, send an event. Events help keep Valkyrie Engine modular and your code maintainable, both by yourself and others.
 *
 * \subsubsection event_create Creating Events
 * In Valkyrie Engine, any object or class can be an event, though we recommend using POD structs.
 * \code{.cpp}
 * struct MyEvent
 * {
 *     const vlk::Int foo = 10;
 *     const vlk::Float bar = 1.0f;
 * };
 * 
 * \endcode
 *
 * \subsubsection event_defined Pre-Defined Events
 * Valkyrie Engine Core comes with a few events that help form the main update loop of the engine. All of these events are simply empty structs with no data but their usage varies significantly:
 * 
 * <ul>
 * <li>vlk::ApplicationStartEvent is sent once, just before the first event loop of the application runs. This is used primarily for second-stage module initialization and to set up the initial state of your game.
 * <li>vlk::ApplicationExitEvent is sent once, after the event loop exits and the application is due to terminate. This is used to perform cleanup on any entities or systems that need it.
 * <li>vlk::PreUpdateEvent is sent at the beginning of every update loop but should not be considered part of it. It is mostly used to update modules and should not be used to change the state of your game.
 * <li>vlk::EarlyUpdateEvent is sent after vlk::PreUpdateEvent and is mostly used to read the state of other entities before vlk::UpdateEvent is called.
 * <li>vlk::UpdateEvent is sent after vlk::EarlyUpdateEvent and is used to update the state of most of entities present in the game.
 * <li>vlk::LateUpdateEvent is sent after vlk::UpdateEvent and is mostly used to react to the changes that occured during the vlk::UpdateEvent.
 * <li>vlk::PostUpdateEvent is sent after vlk::LateUpdateEven and is similar to the vlk::PreUpdateEvent in that it is mostly used to update modules and should not be used to alter the game state in any way, rendering would usually take place during a PostUpdate.
 *
 * \subsubsection event_send Sending Events
 *
 * Sending events in Valkyrie Engine is done through the <tt>EventBus</tt> class. This will raise the event handler for every listener of your event.
 * \code{.cpp}
 * vlk::SendEvent(MyEvent{});
 * \endcode
 *
 * \subsubsection event_get Recieving Events
 *
 * In order to recieve an event, you need to define a class that inherits from <tt>vlk::EventListener</tt> and construct an instance of it. This instance will then recieve any events of the appropriate type you send down the event bus.
 *
 * \code{.cpp}
 * class MyEventListener : public vlk::EventListener<MyEvent>
 * {
 *     void OnEvent(const MyEvent& ev) final override
 *     {
 *         // Do something here
 *	   }
 * };
 * \endcode
 *
 * \subsubsection event_all Putting it all together
 *
 * \code{.cpp}
 * #include "ValkyrieEngine/ValkyrieEngine.hpp"
 * #include <iostream>
 * #include <string>
 * 
 * struct MyEvent
 * {
 *     const vlk::Int foo;
 *     const std::string bar;
 * };
 * 
 * class MyEventListener : public vlk::EventListener<MyEvent>
 * {
 *     public:
 *     MyEventListener() = default;
 * 
 *     private:
 *     void OnEvent(const MyEvent& ev) final override
 *     {
 *         std::cout << "Event handler raised: " << std::to_string(ev.foo) << std::endl;
 *         std::cout << "Cow says: " << ev.bar << std::endl;
 *     }
 * };
 * 
 * int main()
 * {
 *     MyEventListener listener;
 * 
 *     vlk::SendEvent(MyEvent{ 51, "Moo!" });
 * 
 * }
 * \endcode
 *
 * \subsection ecs Entity-Component System
 *
 * Valkyrie Engine core defines a basic entity-component system that serves as the underlying framework for any game produced in it.
 *
 * \subsubsection entity Entities
 * 
 * Entities are represented by a primitive ID and are used used to associate components to one another. The <tt>Entity</tt> namespace defines several functions for creating and destroying entities.
 * 
 * \code{.cpp}
 * #include "ValkyrieEngine/ValkyrieEngine.hpp"
 * void Foo()
 * {
 *     vlk::EntityID myEntity = vlk::Entity::Create();
 * 
 *     ...
 * 
 *     vlk::Entity::Delete(myEntity);
 * }
 * \endcode
 *
 * \subsubsection component Components
 *
 * Components are data types that define a particular behaviour or property for the entity they're attached to. It is important to note that components shouldn't be event listeners. This is mostly to avoid congestion on the event bus as constantly adding and removing event listeners can significantly impact performance.
 *
 * <tt>Component<T>::Create</tt> takes the ID of the entity the component is to be attached to, any subsequent arguments are forwarded to the constructor of your data type.
 * \code{.cpp}
 * #include "ValkyrieEngine/ValkyrieEngine.hpp"
 * class Counter
 * {
 *     public:
 *     Counter(vlk::UInt _score)
 *     {
 *         this->score = _score;
 *     }
 *
 *     vlk::UInt score;
 * };
 *
 * void Foo()
 * {
 *     vlk::EntityID myEntity = vlk::Entity::Create();
 *     vlk::Component<Counter>* c = vlk::Component<Counter>::Create(myEntity, 10);
 * }
 * \endcode
 *
 * \subsubsection system Systems
 *
 * Where components define behaviour for entities, systems are where that behaviour is implemented. Most systems will take the form of several event listeners with any necessary data stored in member variables or the like. Since systems perform operations on/with components, they need a way to access every instance of that component at runtime. vlk::Component has two static member functions called vlk::Component<T>::ForEach and vlk::Component<T>::CForEach that do exactly that.
 *
 * \code{.cpp}
 * #include "ValkyrieEngine/ValkyrieEngine.hpp"
 * 
 * class MySystem : 
 * public vlk::EventListener<vlk::UpdateEvent>
 * public vlk::EventListener<vlk::LateUpdateEvent>
 * {
 *     public:
 *     MySystem() = default;
 *
 *     vlk::UInt sumTotal;
 *
 *     private:
 *     void OnEvent(const vlk::UpdateEvent& ev) final override
 *     {
 *         // Modifying all counter components, use for each
 *         Component<Counter>::ForEach([](Component<Counter>* c)
 *         {
 *             // Add 1 to score every update.
 *             c->score++;
 *         });
 *     }
 *
 *     void OnEvent(const vlk::LateUpdateEvent& ev) final override
 *     {
 *         sumTotal = 0;
 *    
 *         // Not modifying components, const for each makes more sense	
 *         Component<Counter>::CForEach([&sumTotal](const vlk::Component<Counter>* c)
 *         {
 *             // Update sum total of all counters
 *             sumTotal += c->score;
 *         });
 *     }
 * };
 * \endcode
 */

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*!
 * \page contribnotes Notes for contributors
 * \tableofcontents
 *
 * This page provides info and guidelines for contributors and module creators regarding keeping Valkyrie Engine compatable with older C++ standards and portable across different compilers and operating systems.
 *
 * \section compat14 Backwards Compatability
 *
 * Officially, Valkyrie Engine only supports C++14 and above, though C++17 and C++20 features may be enabled if the compiler supports them. The \ref Config.hpp header provides some compiler macros related to these optional features and may help with their implementation. The minimum supported C++ standard is likely to change as time goes on and new major versions of the engine are released, but as a rule of thumb, should always extend at least three major C++ versions back.
 *
 * We also advise community-made modules to adhere to these guidelines to ensure compatability on older machines, though we respect that it's not always possible.
 *
 * \section compat20 C++20
 *
 * At the time of writing, C++20 is still not fully supported by any major compiler. If using C++20 language or STL features in Valkyrie Engine, please make sure they have adequate compiler support. If a feature is not supported by any of: GCC, Clang, Apple Clang or MSVC, either don't use it or provide equivelant code via use of a compiler macro or similar. We will be using the list on cppreference to track support for the forseeable future. If you know of a more up-to-date list, please let us know. 
 *
 * https://en.cppreference.com/w/cpp/compiler_support
 *
 * \section compatCompiler Code Portability
 *
 * To ensure compatability across different compilers, we advise against using compiler-specific features, flags or macros in your code and build pipeline that aren't part of the C++ standard. This includes things like the <tt>-Wall</tt> and <tt>-Wextra</tt> flags, the <tt>__COUNTER__</tt> and <tt>_DEBUG</tt> macros (an exception is made for the <tt>NDEBUG</tt> macro as it's <i>technically</i> standard) and anything starting with <tt>\#pragma</tt> (this includes <tt>\#pragma once</tt>).
 */

namespace vlk
{
	/*!
	 * \brief Sent once every frame, just before the update loop starts.
	 *
	 * Used for parts of the engine that need to periodically update before the main part of the event loop.
	 *
	 * Please don't use this for game logic.
	 */
	struct PreUpdateEvent {};

	/*!
	 * \brief Sent once every frame, just after the update loop ends.
	 * Please don't use this for game logic.
	 */
	struct PostUpdateEvent {};
	
	/*!
	 * \brief First event sent in the update loop.
	 * Always before UpdateEvent
	 */
	struct EarlyUpdateEvent {};

	/*!
	 * \brief Second event sent in the update loop.
	 * Always after EarlyUpdateEvent
	 * Always before LateUpdateEvent
	 */
	struct UpdateEvent {};

	/*!
	 * \brief Last event sent in the update loop.
	 * Always after UpdateEvent.
	 */
	struct LateUpdateEvent {};

	/*!
	 * \brief Sent when the application starts and the first update loop is about to begin
	 */
	struct ApplicationStartEvent {};

	/*!
	 * \brief Sent when the final update loop has taken place and the application is about to exit
	 */
	struct ApplicationExitEvent {};

	/*!
	 * \brief Container for application-related metadata
	 */
	struct ApplicationArgs
	{
		//! Name of the application
		const std::string applicationName = "ValkyrieEngine Application";

		//! Name of the developer
		const std::string developerName = "Default Developer";

		const UInt verMajor = 0; //! Major version of the application
		const UInt verMinor = 0; //! Minor version of the application
		const UInt verPatch = 1; //! Patch version of the application
		const UInt verRevis = 0; //! Revision version of the application
	};

	/*!
	 * \brief Starts and stops the update loop
	 */
	class Application final
	{
		public:

		/*!
		 * \brief Starts the update loop, sends an ApplicationStartEvent.
		 *
		 * \ts
		 * Must only be called from the main thread.<br>
		 * No resources are locked.<br>
		 * This function will block the calling thread until it terminates.<br>
		 *
		 * \sa Application::Stop()
		 */
		static void Start(const ApplicationArgs& args);

		/*!
		 * \brief Sets a flag to stop the update loop. Returns immediately.
		 *
		 * The current update loop will continue as normal.
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * No resources are locked.<br>
		 * This function will not block the calling thread.<br>
		 *
		 * \sa Application::Start(const ApplicationArgs&)
		 */
		static void Stop();
	};
}

#endif
