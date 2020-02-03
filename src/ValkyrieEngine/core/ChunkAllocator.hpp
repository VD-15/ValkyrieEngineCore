#pragma once
#include <cstdlib>
#include <vector>
#include <iterator>
#include <mutex>

#include "Chunk.hpp"
#include "Allocator.hpp"

namespace vlk
{
	template<class T, ULong CHUNK_SIZE = 64>
	class ChunkAllocator final : public IAllocator<ChunkAllocator<T>>
	{
		public:
		ChunkAllocator() = delete;
		ChunkAllocator<T, CHUNK_SIZE>(const ChunkAllocator<T, CHUNK_SIZE>&) = delete;
		ChunkAllocator<T, CHUNK_SIZE>(ChunkAllocator<T, CHUNK_SIZE>&&) = delete;
		ChunkAllocator<T, CHUNK_SIZE>& operator=(const ChunkAllocator<T, CHUNK_SIZE>&) = delete;
		ChunkAllocator<T, CHUNK_SIZE>& operator=(const ChunkAllocator<T, CHUNK_SIZE>&&) = delete;
		~ChunkAllocator() = delete;

		typedef T Type;

		template<class... U>
		static T* AllocateNew(U&&... args)
		{
			std::lock_guard lock(allocMtx);

			for (auto it = chunks.begin(); it != chunks.end(); it++)
			{
				Chunk<T, CHUNK_SIZE>& c = **it;
				if (!c.IsFull())
				{
					T* t = c.InsertNew(std::forward<U>(args)...);

					return t;
				}
			}

			Chunk<T, CHUNK_SIZE>* newChunk = new Chunk<T, CHUNK_SIZE>();

			T* t = newChunk->InsertNew(std::forward<U>(args)...);

			chunks.push_back(newChunk);
			return t;
		}

		static void ForEach(std::function<void(T*)> f)
		{
			std::lock_guard<std::mutex> lock(allocMtx);

			for (auto it = chunks.begin(); it != chunks.end(); it++)
			{
				Chunk<T, CHUNK_SIZE>* c = *it;
				c->ForEach(f);
			}
		}

		static ULong GetCount()
		{
			ULong count = 0;

			for (auto it = chunks.begin(); it != chunks.end(); it++)
			{
				Chunk<T>& c = **it;

				count += c.GetCount();
			}

			return count;
		}

		static Bool HasAllocations()
		{
			for (auto it = chunks.begin(); it != chunks.end(); it++)
			{
				Chunk<T>& c = **it;

				if (c.GetCount() > 0) return true;
			}

			return false;
		}

		static void Delete(T* obj)
		{
			std::lock_guard<std::mutex> lock(allocMtx);

			for (auto it = chunks.begin(); it != chunks.end(); it++)
			{
				Chunk<T, CHUNK_SIZE>& c = **it;

				if (c.Contains(obj))
				{
					c.Remove(obj);

					return;
				}
			}
		}

		private:
		//Probably don't need to worry about a destructor as alocations will be cleared 
		//when program terminates, along with the object.

		static std::vector<Chunk<T, CHUNK_SIZE>*> chunks;
		static std::mutex allocMtx;
	};

	template <typename T, ULong CHUNK_SIZE> 
	std::vector<Chunk<T, CHUNK_SIZE>*> ChunkAllocator<T, CHUNK_SIZE>::chunks;

	template <typename T, ULong CHUNK_SIZE> 
	std::mutex ChunkAllocator<T, CHUNK_SIZE>::allocMtx;
}