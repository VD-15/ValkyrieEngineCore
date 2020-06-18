#pragma once

#include "ValkyrieEngine/Allocator.hpp"
#include <shared_mutex>

namespace vlk
{
	/*!
	 * \brief Single-instance allocator
	 */
	template <typename T>
	class SingletonAllocator final
	{
		class Internal
		{
			public:
			T* allocation;
			std::shared_mutex mtx;

			Internal()
			{
				allocation = nullptr;
			}
		};

		static Internal internal;

		public:
		[[nodiscard]] static void* Allocate()
		{
			std::unique_lock ulock(internal.mtx);
			if (internal.allocation != nullptr)
			{
				throw std::runtime_error("Maximum number of allocations reached");
				return nullptr;
			}
			else
			{
				internal.allocation = reinterpret_cast<T*>(malloc(sizeof(T)));
				return internal.allocation;
			}
		}

		static void Deallocate(void* ptr)
		{
			std::unique_lock ulock(internal.mtx);
			if (ptr == internal.allocation)
			{
				free(internal.allocation);
				internal.allocation = nullptr;
			}
		}

		static void ForEach(std::function<void(T*)> func)
		{
			std::shared_lock ulock(internal.mtx);
			if (internal.allocation != nullptr) func(internal.allocation);
		}

		static void ForEach(std::function<void(T*)> func, std::function<bool(const T*)> filter)
		{
			std::shared_lock ulock(internal.mtx);
			if (internal.allocation != nullptr && filter(internal.allocation)) func(internal.allocation);
		}

		static Size Count()
		{
			std::shared_lock slock(internal.mtx);
			return (internal.allocation != nullptr)? 1 : 0;
		}

		static Size Count(std::function<bool(const T*)> filter)
		{
			std::shared_lock slock(internal.mtx);
			return (internal.allocation != nullptr && filter(internal.allocation))? 1 : 0;
		}
	};

	template <typename T>
	typename SingletonAllocator<T>::Internal SingletonAllocator<T>::internal;
}
