#pragma once

/*
 * Wuh-oh, you freaking idiot, you just got VECTORED!!!
 * That's a mathematical term, a quantity represented
 * by an arrow with both direction and magnitude.
 * VECTOR! (That's me) Because I'm comitting crimes
 * with both direction AND MMMAGNITUDE! OH YEAH!
 */
#include <vector>
#include <algorithm>
#include <shared_mutex>
#include <mutex>
#include "ValkyrieEngine/ThreadPool.hpp"

namespace vlk
{
	/*!
	 * \brief Base class for all event listeners. Do not use this directly, prefer \link vlk::EventListener \endlink, that will automatically subscribe itself to the appropriate \link vlk::EventBus \endlink when constructed.
	 * \sa vlk::EventListener
	 * \sa #OnEvent(const T&)
	 */
	template <typename T>
	class IEventListener
	{
		public:

		/*!
		 * \brief Called whenever event T is sent
		 * \sa vlk::EventBus::Send(const T&)
		 * \sa vlk::EventBus::AddListener(IEventListener<T>*)
		 */
		virtual void OnEvent(const T&) = 0;
	};

	/*!
	 * \brief Facilitates sending events to user-defined event listeners
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
		 * \brief Adds an event listener to this event bus
		 * Once registered, this event listener will then have \link vlk::IEventListener<T>::OnEvent(const T&) OnEvent(const T&) \endlink called whenever an event is sent
		 * The engine makes no guarantee as to what order event listeners are called in
		 * \sa vlk::EventListener
		 * \sa #RemoveListener(IEventListener<T>*)
		 * \sa #Send(const T& t)
		 */
		static void AddListener(IEventListener<T>* listener)
		{
			//TODO: Check if the listener is already present

			//Write access is required here, so unique lock.
			std::unique_lock lock(mtx);
			listeners.push_back(listener);
		}
		
		/*!
		 * \brief Removes all instances of an event listener from this event bus
		 * \sa vlk::EventListener
		 * \sa #AddListener(IEventListener<T>*)
		 * \sa #Send(const T& t)
		 */
		static void RemoveListener(IEventListener<T>* listener)
		{
			//Write access is required, so unique lock.
			std::unique_lock lock(mtx);
			listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
		}

		/*!
		 * \brief Raises \link vlk::IEventListener::OnEvent(const T& t) OnEvent() \endlink for all \link vlk::IEventListener event listeners \endlink on this event bus.
		 * All listeners get called immediately, one after the other, on the calling thread. This function returns once all listeners have finished executing.
		 * \sa vlk::EventListener
		 * \sa #SendAsynchronous
		 * \sa #AddListener(IEventListener<T>*)
		 * \sa #RemoveListener(IEventListener<T>*)
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

		/*!
		 * \brief Raises \link vlk::IEventListener::OnEvent(const T& t) OnEvent() \endlink for all \link vlk::IEventListener event listeners \endlink on this event bus.
		 * All listeners get sent to the \link vlk::ThreadPool thread pooler \endlink to be executed later.
		 * This function returns once all listeners have been sent to the thread pooler, not necessarily when they've completed.
		 * \sa vlk::EventListener
		 * \sa #Send
		 * \sa #AddListener(IEventListener<T>*)
		 * \sa #RemoveListener(IEventListener<T>*)
		 */
		static void SendAsynchronous(const T& t)
		{
			//Only read access is required, so shared lock
			std::shared_lock slock(mtx);

			for (auto it = listeners.begin(); it != listeners.end(); it++)
			{
				IEventListener<T>* listener(*it);

				ThreadPool::Enqueue(std::bind(&IEventListener<T>::OnEvent, listener));
			}

		}
	};
	
	template <typename T>
	std::vector<IEventListener<T>*> EventBus<T>::listeners;

	template <typename T>
	std::shared_mutex EventBus<T>::mtx;

	/*!
	 * \brief Helper class for event listeners to inherit from.
	 * Automatically registers iteself to the appropriate \link vlk::EventBus event bus \endlink when constructed and removes itself when destructed.
	 * 
	 * \sa vlk::EventBus
	 * \sa vlk::IEventListener
	 */
	template<typename T>
	class EventListener : public IEventListener<T>
	{
		protected:

		/*!
		 * \brief Registers this event listener to the \link vlk::EventBus event bus \endlink appropriate for event T.
		 */
		EventListener<T>()
		{
			EventBus<T>::AddListener(static_cast<IEventListener<T>*>(this));
		}

		/*!
		 * \brief Removes this event listener from the \link vlk::EventBus event bus \endlink appropriate for event T.
		 * Please note that in your subclass, you must either specify your destructor as virtual or explicitly call this destructor in order for it to be removed from the eventbus properly.
		 */
		virtual ~EventListener<T>()
		{
			EventBus<T>::RemoveListener(static_cast<IEventListener<T>*>(this));
		}
	};

	/*!
	 * \brief Helper function. Equivelant to:
	 * \code{.cpp}
	 * EventBus<T>::SendEvent(t);
	 * \endcode
	 */
	template<typename T>
	constexpr void SendEvent(const T& t)
	{
		vlk::EventBus<T>::Send(t);
	}

}
