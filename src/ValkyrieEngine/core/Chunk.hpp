#pragma once

#include <stdlib.h>
#include <array>

#include "ValkyrieEngine.h"

namespace vlk
{
	template <class T, ULong CHUNK_SIZE>
	class Chunk
	{
		public:
		Chunk<T, CHUNK_SIZE>() :
			allocations {false}
		{
			//TODO: replace malloc/free with new[]/delete[]
			start = (T*)malloc(CHUNK_SIZE * sizeof(T));
			instances.reserve(CHUNK_SIZE);
		}

		//Delete copy and move constructors as well as assignment operator.
		//We don't want chunks to be cloned as that would be HORRIBLY inefficient.
		Chunk<T, CHUNK_SIZE>(const Chunk<T, CHUNK_SIZE>& old) = delete;
		Chunk<T, CHUNK_SIZE>(Chunk<T, CHUNK_SIZE>&& old) = delete;
		Chunk<T, CHUNK_SIZE>& operator=(const Chunk<T, CHUNK_SIZE>& other) = delete;
		Chunk<T, CHUNK_SIZE>& operator=(const Chunk<T, CHUNK_SIZE>&& other) = delete;

		~Chunk<T, CHUNK_SIZE>()
		{
			free(start);
			start = nullptr;
		}

		//Insert a new element into this chunk.
		template<class... U>
		T* InsertNew(U&&... args)
		{
			//Find first free allocation
			for (ULong i = 0; i < CHUNK_SIZE; i++)
			{
				if (!allocations[i])
				{
					//Mark region as allocated
					allocations[i] = true;
					T* where = start + i;

					T* t = new(where) T(std::forward<U>(args)...);

					//Add to instances
					instances.push_back(where);

					return where;
				}
			}

			//If no allocation is found, return nullptr
			return nullptr;
		}

		//Removes an element from this chunk.
		void Remove(T* allocation)
		{
			for (ULong i = 0; i < CHUNK_SIZE; i++)
			{
				if ((start + i) == allocation)
				{
					//Call destructor
					allocation->~T();

					//Mark region as unallocated
					allocations[i] = false;

					//Remove from instances
					instances.erase(std::remove(instances.begin(), instances.end(), allocation), instances.end());

					return;
				}
			}
		}

		//Returns true if this chunk is full.
		Boolean IsFull() const
		{
			return instances.size() >= instances.capacity();
		}

		//Returns true if this chunk is empty.
		Boolean IsEmpty() const
		{
			return instances.size() == 0;
		}

		//Gets the number of allocations this chunk holds
		ULong GetCount() const
		{
			return instances.size();
		}

		//Returns true if the pointer lies inside of this chunk.
		Boolean Contains(T* t) const
		{
			return (t >= start && t <= start + (CHUNK_SIZE - 1));
		}

		template<class F>
		F ForEach(F f)
		{
			return std::for_each(instances.begin(), instances.end(), f);
		}

		private:
		std::array<Boolean, CHUNK_SIZE> allocations;
		std::vector<T*> instances;
		T* start;
	};
}