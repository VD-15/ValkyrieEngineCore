#pragma once

#include "ValkyrieEngine/Allocator.hpp"
#include <shared_mutex>
#include <array>

namespace vlk
{ 
	/*!
	 * \brief Allocator backed by a fixed-size bounded array
	 * \tparam Capacity maximum number of elements that can be allocated at once
	 */
	template<typename T, Size Capacity>
	class ArrayAllocator
	{
		class Internal
		{
			public:
			T* allocations;
			std::array<bool, Capacity> isOccupied;
			std::shared_mutex mtx;

			Internal()
			{
				allocations = reinterpret_cast<T*>(std::malloc(sizeof(T) * Capacity));
				for (Size i = 0; i < Capacity; i++)
					isOccupied[i] = false;
			}
		};

		static Internal internal;

		static_assert(Capacity != 0, "Capacity must be larger than 0");

		public:

		/*!
		 * \brief Allocates space for a new instance of T.
		 * Does not construct T.
		 * \return a pointer to a region of unitialized memory with size equal to \c sizeof(T)
		 * \throw std::bad_alloc if the allocator is out of space
		 */
		[[nodiscard]] static void* Allocate()
		{
			std::unique_lock ulock(internal.mtx);
			for (Size s = 0; s < Capacity; s++)
			{
				if (internal.isOccupied[s] == false)
				{
					internal.isOccupied[s] = true;
					return internal.allocations + s;
				}
			}

			throw std::bad_alloc();
			return nullptr;
		}

		/*!
		 * \brief Deallocates the memory taken up by ptr
		 * Does not call T's destructor
		 * \param ptr A pointer to an instance of T allocated by this allocator
		 */
		static void Deallocate(void* ptr)
		{
			std::unique_lock ulock(internal.mtx);

			if (ptr >= internal.allocations)
			{
				Size index = reinterpret_cast<T*>(ptr) - internal.allocations;
				if (index < Capacity)
				{
					internal.isOccupied[index] = false;
					return;
				}
			}

			throw std::range_error("Pointer is not owned by this allocator.");
		}

		/*!
		 * \brief performs a function on every active instance of T owned by this allocator
		 */
		static void ForEach(std::function<void(T*)> func)
		{
			std::shared_lock ulock(internal.mtx);
			for (Size i = 0; i < Capacity; i++)
			{
				if (internal.isOccupied[i] == true)
				{
					func(internal.allocations + i);
				}
			}
		}


		/*!
		 * \brief Performs a function on every active instance of T owned by this allocator that matches a filter
		 * \param func The modifier function to perform on every instance of T that passes the filter
		 * \param filter The non-modifying filter function that every instance of T is checked against
		 */
		static void ForEach(std::function<void(T*)> func, std::function<bool(const T*)> filter)
		{
			std::shared_lock ulock(internal.mtx);
			for (Size i = 0; i < Capacity; i++)
			{
				if (internal.isOccupied[i] == true && filter(internal.allocations + i))
				{
					func(internal.allocations + i);
				}
			}
		}

		/*!
		 * \brief Returns the number of active instances of T owned by this allocator
		 */
		static Size Count()
		{
			std::shared_lock slock(internal.mtx);
			Size s = 0;

			for (Size i = 0; i < Capacity; i++)
			{
				if (internal.isOccupied[i] == true) s++;
			}

			return s;
		}		

		/*!
		 * \brief Returns the number of active instance of T owned by this allocator that match a filter
		 * \param filter The non-modifying function that every instance of T is checked against
		 */
		static Size Count(std::function<bool(const T*)> filter)
		{
			std::shared_lock slock(internal.mtx);
			Size s = 0;

			for (Size i = 0; i < Capacity; i++)
			{
				if (internal.isOccupied[i] == true && filter(&internal.allocations[i])) s++;
			}

			return s;
		}
	};

	template <typename T, Size Capacity>
	typename ArrayAllocator<T, Capacity>::Internal ArrayAllocator<T, Capacity>::internal;
}
