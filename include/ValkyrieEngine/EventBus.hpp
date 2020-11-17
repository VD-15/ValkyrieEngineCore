/*!
 * \file EventBus.hpp
 * \brief Provides listeners for event loop
 */

#ifndef VLK_EVENTBUS_HPP
#define VLK_EVENTBUS_HPP

#include <vector>
#include <algorithm>
#include <shared_mutex>
#include <mutex>

namespace vlk
{
	/*!
	 * \brief Base class for all event listeners.
	 *
	 * Direct use is discouraged. Prefer inheriting from EventListener<T> instead.
	 *
	 * \sa EventListener<T>
	 * \sa OnEvent(const T&)
	 */
	template <typename T>
	class IEventListener
	{
		public:

		/*!
		 * \brief Callback raised whenever an event T is recieved.
		 *
		 * This must be overridden in the event listener implementation.
		 *
		 * \sa vlk::EventBus::Send(const T&)
		 * \sa vlk::EventBus::AddListener(IEventListener<T>*)
		 */
		virtual void OnEvent(const T&) = 0;
	};

	/*!
	 * \brief Facilitates sending events to user-defined event listeners.
	 *
	 * \sa EventListener<T>
	 */
	template <typename T>
	class EventBus
	{
		//All event listeners subscribed to this event bus
		static std::vector<IEventListener<T>*> listeners;

		/*!
		 * \brief Controls shared access to \link #listeners \endlink
		 */
		static std::shared_mutex mtx;

		public:

		/*!
		 * \brief Adds an event listener to the event bus
		 *
		 * Once added, an event listener will then have IEventListener<T>::OnEvent(const T&) called whenever an event is sent.
		 * If the listener is already present, it is not added again.
		 * The engine makes no guarantee as to what order event listeners are called in.
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking is handled internally.<br>
		 * Unique access to this class is required.<br>
		 * This function may block the calling thread.<br>
		 *
		 * \sa vlk::EventListener
		 * \sa #RemoveListener(IEventListener<T>*)
		 * \sa #Send(const T& t)
		 */
		static void AddListener(IEventListener<T>* listener)
		{
			//Write access is required here, so unique lock.
			std::unique_lock lock(mtx);

			for (auto it = listeners.begin(); it != listeners.end(); it++)
			{// Check if the listener is already present
				if ((*it) == listener) return;
			}

			listeners.push_back(listener);
		}
		
		/*!
		 * \brief Removes an event listener from this event bus
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking is handled internally.<br>
		 * Unique access to this class is required.<br>
		 * This function may block the calling thread.<br>
		 *
		 * \sa vlk::EventListener
		 * \sa AddListener(IEventListener<T>*)
		 * \sa Send(const T& t)
		 */
		static void RemoveListener(IEventListener<T>* listener)
		{
			//Write access is required, so unique lock.
			std::unique_lock lock(mtx);
			listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
		}

		/*!
		 * \brief Raises the IEventListener<T>::OnEvent callback for every IEventListener present in the bus.
		 *
		 * All listeners get called immediately, one after the other, on the calling thread. This function returns once all listeners have finished executing.
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking of this class is handled internally.<br>
		 * Event listeners must implement their own resource locking.<br>
		 * Shared access to this class is required.<br>
		 * This function may block the calling thread.<br>
		 *
		 * \sa EventListener<T>
		 * \sa AddListener(IEventListener<T>*)
		 * \sa RemoveListener(IEventListener<T>*)
		 */
		static void Send(const T& t)
		{
			//Only read access is required, so shared lock
			std::shared_lock lock(mtx);

			for (auto it = listeners.begin(); it != listeners.end(); it++)
			{
				IEventListener<T>* listener(*it);

				listener->OnEvent(t);
			}
		}
	};
	
	template <typename T>
	std::vector<IEventListener<T>*> EventBus<T>::listeners;

	template <typename T>
	std::shared_mutex EventBus<T>::mtx;

	/*!
	 * \brief Base class for event listeners to inherit from.
	 * Automatically registers iteself to the appropriate vlk::EventBus<T> when constructed and removes itself when destructed.
	 * 
	 * \sa vlk::EventBus
	 * \sa vlk::IEventListener
	 */
	template<typename T>
	class EventListener : public IEventListener<T>
	{
		protected:

		/*!
		 * \brief Registers this event listener to the EventBus<T>.
		 */
		EventListener<T>()
		{
			EventBus<T>::AddListener(static_cast<IEventListener<T>*>(this));
		}

		/*!
		 * \brief Removes this event listener from the vlk::EventBus<T>.
		 */
		~EventListener<T>()
		{
			EventBus<T>::RemoveListener(static_cast<IEventListener<T>*>(this));
		}
	};

	/*!
	 * \brief Shorthand event sending function.
	 *
	 * Equivelant to:
	 *
	 * \code{.cpp}
	 * EventBus<T>::SendEvent(t);
	 * \endcode
	 */
	template<typename T>
	inline void SendEvent(const T& t)
	{
		vlk::EventBus<T>::Send(t);
	}

}

#endif
