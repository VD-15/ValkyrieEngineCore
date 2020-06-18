#pragma once
#include "ValkyrieEngine/ValkyrieDebug.hpp"
#include "ValkyrieEngine/Entity.hpp"
#include "ValkyrieEngine/ArrayAllocator.hpp"
#include "ValkyrieEngine/ChunkAllocator.hpp"
#include "ValkyrieEngine/LinkedAllocator.hpp"
#include "ValkyrieEngine/SingletonAllocator.hpp"
#include "ValkyrieEngine/EventBus.hpp"

namespace vlk
{
	/*!
	 * \brief Sent once every frame, just before the update loop starts.
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
		/*!
		 * \brief True if the update loop is running
		 */
		static bool isRunning;

		public:

		/*!
		 * \brief Container for application-related metadata
		 */
		struct ApplicationArgs
		{
			std::string applicationName = "ValkyrieEngine Application";
			std::string developerName = "Unknown Developer";
			UInt verMajor = 0;
			UInt verMinor = 0;
			UInt verPatch = 1;
			UInt verRevis = 0;
		};

		/*!
		 * \brief Starts the update loop, sends an ApplicationStartEvent.
		 */
		static void Start(const ApplicationArgs& args);

		/*!
		 * \brief Sets a flag to stop the update loop. Returns immediately.
		 */
		static void Stop();
	};
}
