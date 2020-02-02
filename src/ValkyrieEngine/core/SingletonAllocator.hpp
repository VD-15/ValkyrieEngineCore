#pragma once

#include "ValkyrieEngine.h"
#include "Allocator.hpp"

#include <mutex>
#include <algorithm>

namespace vlk
{
	template <class T>
	class SingletonAllocator final : public IAllocator<SingletonAllocator<T>>
	{
		public:
		SingletonAllocator() = delete;
		~SingletonAllocator() = delete;
		SingletonAllocator<T>(const SingletonAllocator<T>&) = delete;
		SingletonAllocator<T>(SingletonAllocator<T>&&) = delete;
		SingletonAllocator<T>& operator=(const SingletonAllocator<T>&) = delete;
		SingletonAllocator<T>& operator=(const SingletonAllocator<T>&&) = delete;

		typedef T Type;

		template<class... U>
		static T* AllocateNew(U&&... args)
		{
			std::unique_lock lock(allocMtx);

			if (allocation)
			{
				delete allocation;
			}

			allocation = new T(args...);

			lock.unlock();

			return allocation;
		}

		static void ForEach(std::function<void(T*)> f)
		{
			std::lock_guard<std::mutex> lock(allocMtx);

			if (allocation)
			{
				f(allocation);
			}
		}

		static ULong GetCount()
		{
			std::lock_guard<std::mutex> lock(allocMtx);

			return static_cast<Boolean>(allocation) ? 1 : 0;
		}

		static Boolean HasAllocations()
		{
			std::lock_guard<std::mutex> lock(allocMtx);

			return static_cast<Boolean>(allocation);
		}

		static void Delete(T* obj)
		{
			std::lock_guard<std::mutex> lock(allocMtx);

			if (obj == allocation)
			{
				delete allocation;
				allocation = nullptr;
			}
			else
			{
				throw std::exception("Object did not match allocation!");
			}
		}
		
		private:
		static T* allocation;
		static std::mutex allocMtx;
	};

	template <typename T> T* SingletonAllocator<T>::allocation;
	template <typename T> std::mutex SingletonAllocator<T>::allocMtx;
}