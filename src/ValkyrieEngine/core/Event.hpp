#pragma once

#include "ValkyrieEngine.h"
#include <vector>
#include <mutex>
#include <thread>

namespace vlk
{
	template <typename T>
	struct IEventListener
	{
		virtual void OnEvent(const T& ev) = 0;
	};

	template <typename T>
	struct EventBus
	{
		static void AddListener(IEventListener<T>* listener)
		{
			std::unique_lock lock(mtx, std::defer_lock);

			if (lock.try_lock())
			{
				#ifdef _DEBUG
				for (auto it = listeners.begin(); it != listeners.end(); it++)
				{
					if (*it == listener)
					{
						lock.unlock();
						return;
					}
				}
				#endif

				listeners.push_back(listener);

				lock.unlock();
			}
			else
			{
				std::thread t(AddListenerDeferred, listener);
				t.detach();
			}
		}

		static void RemoveListener(IEventListener<T>* listener)
		{
			std::unique_lock lock(mtx, std::defer_lock);

			if (lock.try_lock())
			{
				auto it = std::find(listeners.begin(), listeners.end(), listener);

				#ifdef _DEBUG
				if (it == listeners.end())
				{
					lock.unlock();
					return;
				}
				#endif

				listeners.erase(it);
				lock.unlock();
			}
			else
			{
				std::thread t(RemoveListenerDeferred, listener);
				t.detach();
			}
		}

		template <typename... U>
		static void Send(U&&... args)
		{
			T t(args...);

			std::lock_guard lock(mtx);

			for (auto it = listeners.begin(); it != listeners.end(); it++)
			{
				(*it)->OnEvent(t);
			}
		}

		private:
		static std::mutex mtx;
		static std::vector<IEventListener<T>*> listeners;

		static void AddListenerDeferred(IEventListener<T>* listener)
		{
			std::unique_lock lock(mtx);

			#ifdef _DEBUG
			for (auto it = listeners.begin(); it != listeners.end(); it++)
			{
				if (*it == listener)
				{
					lock.unlock();
					return;
				}
			}
			#endif

			listeners.push_back(listener);

			lock.unlock();
		}

		static void RemoveListenerDeferred(IEventListener<T>* listener)
		{
			std::unique_lock lock(mtx);

			auto it = std::find(listeners.begin(), listeners.end(), listener);

			#ifdef _DEBUG
			if (it == listeners.end())
			{
				lock.unlock();
				return;
			}
			#endif

			listeners.erase(it);

			lock.unlock();
		}
	};

	template <typename T> std::mutex EventBus<T>::mtx;
	template <typename T> std::vector<IEventListener<T>*> EventBus<T>::listeners;

	template <class T>
	struct EventListener : public IEventListener<T>
	{
		EventListener<T>()
		{
			EventBus<T>::AddListener(static_cast<IEventListener<T>*>(this));
		}

		~EventListener<T>()
		{
			EventBus<T>::RemoveListener(static_cast<IEventListener<T>*>(this));
		}

		EventListener<T>(const EventListener<T>&) = delete;
		EventListener<T>(EventListener<T>&&) = delete;
		EventListener<T>& operator=(const EventListener<T>&) = delete;
		EventListener<T>& operator=(EventListener<T>&&) = delete;
	};

	struct LogEvent
	{
		const std::string& source; 
		const UInt severity; 
		const std::string& message; 
	};

	//Signals that the application has started
	struct ApplicationStartEvent {};

	//Signals that the application is exiting
	struct ApplicationExitEvent {};

	struct PreUpdateEvent {};
	struct EarlyUpdateEvent {};
	struct UpdateEvent {};
	struct LateUpdateEvent {};
	struct PostUpdateEvent {};
}