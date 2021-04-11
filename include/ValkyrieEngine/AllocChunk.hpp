/*!
 * \file AllocChunk.hpp
 *
 * \brief Provides the AllocChunk<T, S> template class.
 */

#ifndef VLK_ALLOC_CHUNK_HPP
#define VLK_ALLOC_CHUNK_HPP

#include "ValkyrieEngine/ValkyrieDefs.hpp"
#include <memory>
#include <bitset>

namespace vlk
{
	/*!
	 * \brief Fixed-capacity, variable-size storage block. Used internally by Component<T>.
	 *
	 * This class does not construct or destruct any instances of T, it only allocates space
	 * for them. You must handle construction and destruction of instances yourself.
	 *
	 * \tparam T The type this Chunk is storing.
	 * \tparam S The number of instances this chunk can allocate at once.
	 */
	template <typename T, Size S>
	class AllocChunk
	{
		public:
		static VLK_CXX14_CONSTEXPR Size ChunkSize = S;
		typedef AllocChunk<T, S> SelfType;
		typedef T ValueType;
		typedef T* PointerType;

		VLK_STATIC_ASSERT_MSG(ChunkSize > 0, "Component block size must be greater than zero");

		private:
		std::bitset<ChunkSize> occupations;
		typename std::aligned_storage<sizeof(T), alignof(T)>::type storage[S];

		public:
		AllocChunk<T, S>() = default;
		AllocChunk<T, S>(const SelfType&) = delete;
		AllocChunk<T, S>(SelfType&& a) = delete;
		SelfType& operator=(const SelfType&) = delete;
		SelfType& operator=(SelfType&& a) = delete;

		/*!
		 * \brief Destroys this AllocChunk
		 *
		 * This does not destroy any alocated instances,
		 * your code must handle construction and destruction of objects.
		 */
		~AllocChunk()
		{
			/*for (Size i = 0; i < ChunkSize; i++)
			{
				if (occupations[i]) At(i)->~T();
			}*/
		}

		/*!
		 * \brief Returns true if none of this chunk's allocation spaces are filled.
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking must be handled externally.<br>
		 * Access to this object is not restricted.<br>
		 * This function does not block the calling thread.<br>
		 */
		inline bool Empty() const { return occupations.none(); }

		/*!
		 * \brief Returns true if all of this chunk's allocation spaces are filled.
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking must be handled externally.<br>
		 * Access to this object is not restricted.<br>
		 * This function does not block the calling thread.<br>
		 */
		inline bool Full() const { return occupations.all(); }

		/*!
		 * \brief Returns true if the allocation space at position <tt>i</tt> is occupied.
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking must be handled externally.<br>
		 * Access to this object is not restricted.<br>
		 * This function does not block the calling thread.<br>
		 */
		inline bool IsOccupied(Size i) const { return occupations[i]; }

		/*!
		 * \brief Returns a pointer to the allocation space at position <tt>i</tt>.
		 *
		 * This allocation space may not contain an initialized object, it is necessary to
		 * check if the space is occupied first before dereferencing the returned pointer.
		 *
		 * \param i The index of the allocation space to retrieve, must be less than <tt>S</tt>
		 *
		 * \sa vlk::AllocChunk<T, S>::IsOccupied(T*)
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking must be handled externally.<br>
		 * Access to this object is not restricted.<br>
		 * This function does not block the calling thread.<br>
		 */
		inline T* At(Size i)
		{
			return reinterpret_cast<T*>(&storage[i]);
		}

		/*!
		 * \copydoc At(Size)
		 */
		inline const T* At(Size i) const
		{
			return reinterpret_cast<const T*>(&storage[i]);
		}

		/*!
		 * \brief Returns true if the pointer <tt>t</tt> lies within the space occupied by this chunk's storage.
		 *
		 * This does not verify that <tt>t</tt> points to a valid object.
		 *
		 * \sa vlk::AllocChunk<T, S>::IsOccupied(Size)
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking must be handled externally.<br>
		 * Access to this object is not restricted.<br>
		 * This function does not block the calling thread.<br>
		 */
		inline bool OwnsPointer(T* t) const
		{
			return ((t >= At(0)) & (t <= At(ChunkSize - 1)));
		}

		/*!
		 * \brief Marks the first unoccupied allocation space in this chunk as occupied
		 * and returns a pointer to it.
		 *
		 * This does not initialize an instance of <tt>T</tt>. You must construct
		 * an instance of <tt>T</tt> at this pointer before it is safe to dereference.
		 *
		 * \throws std::bad_alloc If the chunk is full when the function is invoked.
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking must be handled externally.<br>
		 * Access to this object is not restricted.<br>
		 * This function does not block the calling thread.<br>
		 */
		VLK_NODISCARD T* Allocate()
		{
			for (Size i = 0; i < ChunkSize; i++)
			{
				if (!occupations[i])
				{
					occupations.set(i);
					return At(i);
				}
			}

			throw std::bad_alloc();
			return nullptr;
		}

		/*!
		 * \brief Marks a pointer owned by this chunk as unoccupied.
		 *
		 * This does not destroy the object at the pointer.
		 * This does not free any memory.
		 *
		 * \param t A pointer to an allocation space that this chunk owns.
		 *
		 * \sa vlk::AllocChunk<T, S>::OwnsPointer(T*)
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking must be handled externally.<br>
		 * Access to this object is not restricted.<br>
		 * This function does not block the calling thread.<br>
		 */
		inline void Deallocate(T* t)
		{
			Size s = t - At(0);
			occupations.reset(s);
		}

		/*!
		 * \brief Returns the number of currently allocated spaces.
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking must be handled externally.<br>
		 * Access to this object is not restricted.<br>
		 * This function does not block the calling thread.<br>
		 */
		inline Size Count() const
		{
			return occupations.count();
		}
	};

	template <typename T, Size S>
	VLK_CXX14_CONSTEXPR Size AllocChunk<T, S>::ChunkSize;
}

#endif
