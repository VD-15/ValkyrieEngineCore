#pragma once

#include <queue>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <thread>
#include <functional>

#include "ValkyrieEngine/ValkyrieDefs.hpp"

namespace vlk
{
	/*!
	 * \brief Thread wrapper for use with the event pool.
	 * Please don't touch this unless you know what you're doing.
	 */
	class Thread final
	{
		std::condition_variable cv;
		std::function<void()> fn;
		std::thread thr;
		std::mutex mtx;
		bool isJoining;

		public:
		Thread();
		Thread(const Thread&) = delete;
		Thread(Thread&&) = delete;
		Thread& operator=(const Thread&) = delete;
		Thread& operator=(Thread&&) = delete;
		~Thread();

		/*!
		 * \brief Sets the function this thread is executing.
		 */
		void SetFunc(const std::function<void()>& newFunc);

		/*!
		 * \brief Returns true if this thread is idle.
		 */
		bool IsIdle();
	};

	/*!
	 * \brief Maintains a collection of persistent thread objects to enable asyncronous processing.
	 * Number of threads will be equal to std::thread::hardware_concurrency by default, but will 
	 * always be at least one.
	 */
	class ThreadPool
	{
		class Internal
		{
			public:
			Internal();
			~Internal();

			std::queue<std::function<void()>> pending;
			Thread* threads;
			Size numThreads;
			std::mutex mtx;
			std::condition_variable cv;
		};

		static Internal internal;

		[[nodiscard]] static std::function<void()> GetNext();
		static bool IsIdle();

		public:

		//Allows vlk::Thread to access #GetNext()
		friend Thread;

		/*!
		 * \brief Adds a function to the back of the execution queue.
		 * Retrurns immediately.
		 */
		static void Enqueue(const std::function<void()>& func);

		/*!
		 * \brief Blocks the calling thread until all threads have finished executing
		 * and the execution queue is empty.
		 */
		static void Wait();
	};
}
