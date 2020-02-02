#pragma once

#include "Allocator.hpp"

#include <mutex>
#include <algorithm>

namespace vlk
{
	template <class T>
	class HeapAllocator final : public IAllocator<HeapAllocator<T>>
	{
		public:
		HeapAllocator() = delete;
		~HeapAllocator() = delete;
		HeapAllocator<T>(const HeapAllocator<T>&) = delete;
		HeapAllocator<T>(HeapAllocator<T>&&) = delete;
		HeapAllocator<T>& operator=(const HeapAllocator<T>&) = delete;
		HeapAllocator<T>& operator=(const HeapAllocator<T>&&) = delete;

		typedef T Type;

		template<class... U>
		static T* AllocateNew(U&&... args)
		{
			std::lock_guard<std::mutex> lock(allocMtx);

			T* allocation = new T(std::forward<U>(args)...);
			allocations.push_back(allocation);
			return allocation;
		}
		
		static void ForEach(const std::function<void(T*)>& f)
		{
			std::lock_guard<std::mutex> lock(allocMtx);

			std::for_each(allocations.begin(), allocations.end(), f);
		}

		static ULong GetCount()
		{
			return allocations.size();
		}

		static Boolean HasAllocations()
		{
			return allocations.size() > 0;
		}

		static void Delete(T* obj)
		{
			std::lock_guard<std::mutex> lock(allocMtx);

			auto it = std::remove(allocations.begin(), allocations.end(), obj);

			if (it != allocations.end())
			{
				obj->~T();

				allocations.erase(it, allocations.end());
			}
			else
			{
				throw std::exception("Failed to remove object.");
			}
		}

		private:
		static std::vector<T*> allocations;
		static std::mutex allocMtx;
	};

	template <typename T> std::vector<T*> HeapAllocator<T>::allocations;
	template <typename T> std::mutex HeapAllocator<T>::allocMtx;
}