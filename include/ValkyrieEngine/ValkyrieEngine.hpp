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
 * To ensure compatability across different compilers, we advise against using compiler-specific features, flags or macros in your code and build pipeline that aren't part of the C++ standard. This includes things like the <tt>-Wall</tt> and <tt>-Wextra</tt> flags, the <tt>__COUNTER__</tt>, <tt>__FILE_NAME__</tt> and <tt>_DEBUG</tt> macros (an exception is made for the <tt>NDEBUG</tt> macro) and anything starting with <tt>\#pragma</tt> (this includes <tt>\#pragma once</tt>).
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
	 * \brief Starts and stops the update loop
	 */
	class Application final
	{
		public:

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
		 * \brief Starts the update loop, sends an ApplicationStartEvent.
		 *
		 * \ts
		 * Must only be called from the main thread.<br>
		 * No resources are locked.<br>
		 * This function will block the calling thread until it terminates.<br>
		 *
		 * \sa Stop()
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
		 * \sa Start(const ApplicationArgs&)
		 */
		static void Stop();
	};
}

#endif
