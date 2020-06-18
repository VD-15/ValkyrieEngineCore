#pragma once
#include "ValkyrieEngine/Allocator.hpp"
#include <shared_mutex>

namespace vlk
{
	/*!
	 * \brief Allocator backed by multiple fixed-size chunks
	 * \tparam Capacity maximum number of elements that can fit inside a single chunk 
	 */
	template <typename T, Size Capacity>
	class ChunkAllocator
	{
		class Chunk
		{
			public:
			T* allocations;
			std::array<bool, Capacity> isOccupied;

			Chunk()
			{
				allocations = reinterpret_cast<T*>(malloc(sizeof(T) * Capacity));

				for (Size i = 0; i < Capacity; i++)
					isOccupied[i] = false;
			}

			~Chunk()
			{
				free(allocations);
			}
		};

		class Internal
		{
			public:
			std::vector<Chunk*> chunks;
			std::shared_mutex mtx;

			Internal() {}
		};

		static Internal internal;

		static_assert(Capacity != 0, "Capacity must be larger than 0");

		public:

		/*!
		 * \brief Allocates space for a new instance of T.
		 * Does not construct T.
		 */
		[[nodiscard]] static void* Allocate()
		{
			std::unique_lock ulock(internal.mtx);
			for (auto chunk : internal.chunks)
			{
				for (Size i = 0; i < Capacity; i++)
				{
					if (chunk->isOccupied[i] == false)
					{
						chunk->isOccupied[i] = true;
						return &chunk->allocations[i];
					}
				}
			}

			Chunk* c = new Chunk();
			internal.chunks.push_back(c);
			c->isOccupied.at(0) = true;
			return &c->allocations[0];
		}

		/*!
		 * \brief Deallocates the memory taken up by ptr
		 * Does not call T's destructor
		 * \param ptr A pointer to an instance of T allocated by this allocator
		 */
		static void Deallocate(void* ptr)
		{
			std::unique_lock ulock(internal.mtx);
			T* t  = reinterpret_cast<T*>(ptr);
			
			//Search for chunk that contains ptr 
			for (auto chunk : internal.chunks)
			{
				if (t >= &chunk->allocations[0])
				{
					//if ptr is infront of chunk, we can calculate index and check if it lies within the chunk
					//if ptr is behind chunk, then it's not in the chunk anyway
					Size index = t - chunk->allocations;
					if (index < Capacity)
					{
						chunk->isOccupied[index] = false;

						//if the chunk is empty, then delete it.
						if (std::find(chunk->isOccupied.begin(), chunk->isOccupied.end(), true) == chunk->isOccupied.end())
						{
							internal.chunks.erase(std::find(internal.chunks.begin(), internal.chunks.end(), chunk));
							delete chunk;
						}

						return;
					}
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
			for (auto chunk : internal.chunks)
			{
				for (Size i = 0; i < Capacity; i++)
				{
					if (chunk->isOccupied[i] == true)
					{
						func(&chunk->allocations[i]);
					}
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
			for (auto chunk : internal.chunks)
			{
				for (Size i = 0; i < Capacity; i++)
				{
					if (chunk->isOccupied[i] == true && filter(&chunk->allocations[i]))
					{
						func(&chunk->allocations[i]);
					}
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

			for (auto chunk : internal.chunks)
			{
				for (Size i = 0; i < Capacity; i++)
				{
					if (chunk->isOccupied[i] == true) s++;
				}
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

			for (auto chunk : internal.chunks)
			{
				for (Size i = 0; i < Capacity; i++)
				{
					if (chunk->isOccupied[i] == true && filter(&chunk->allocations[i])) s++;
				}
			}

			return s;
		}

		/*!
		 * \brief Returns the number of chunks allocated by this allocator
		 */
		static Size ChunkCount()
		{
			return internal.chunks.size();
		}
	};

	template<typename T, Size Capacity>
	typename ChunkAllocator<T, Capacity>::Internal ChunkAllocator<T, Capacity>::internal;
}
