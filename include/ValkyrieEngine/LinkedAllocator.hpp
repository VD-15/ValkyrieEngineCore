#pragma once

#include "ValkyrieEngine/Allocator.hpp"
#include <shared_mutex>

namespace vlk
{
	/*!
	 * \brief Allocator backed by an unbounded linked-list
	 */
	template <typename T>
	class LinkedAllocator
	{
		class Node
		{
			public:
			Node* next;
			Node* prev;
			T* data;

			Node()
			{
				data = reinterpret_cast<T*>(malloc(sizeof(T)));
				next = nullptr;
				prev = nullptr;
			}

			Node(Node* _prev)
			{
				data = reinterpret_cast<T*>(malloc(sizeof(T)));
				next = nullptr;
				prev = _prev;
				_prev->next = this;
			}

			~Node()
			{
				free(data);
			}
		};

		class Internal
		{
			public:
			Node* front;
			Node* back;
			std::shared_mutex mtx;

			public:
			Internal()
			{
				front = nullptr;
				back = nullptr;
			}
		};

		static Internal internal;

		public:

		/*!
		 * \brief Allocates space for a new instance of T.
		 * Does not construct T.
		 */
		[[nodiscard]] static void* Allocate()
		{
			std::unique_lock ulock(internal.mtx);

			if (internal.front == nullptr)
			{
				internal.front = new Node();
				internal.back = internal.front;
				return internal.front->data;
			}
			else
			{
				Node* n = new Node(internal.back);
				internal.back = n;
				return n->data;
			}
		}

		/*!
		 * \brief Deallocates the memory taken up by ptr
		 * Does not call T's destructor
		 * \param ptr A pointer to an instance of T allocated by this allocator
		 */
		static void Deallocate(void* ptr)
		{
			std::unique_lock ulock(internal.mtx);

			for (Node* n = internal.front; n != nullptr; n = n->next)
			{
				if (reinterpret_cast<T*>(ptr) == n->data)
				{
					if (n->prev != nullptr && n->next != nullptr) // n is neither front nor back
					{
						n->prev->next = n->next;
						n->next->prev = n->prev;
					}
					else if (n->next == nullptr && n->prev != nullptr) // n is back
					{
						n->prev->next = nullptr;
						internal.back = n->prev;
					}
					else if (n->prev == nullptr && n->next != nullptr) // n is front
					{
						n->next->prev = nullptr;
						internal.front = n->next;
					}
					else // n is front and  back
					{
						internal.front = nullptr;
						internal.back = nullptr;
					}

					delete n;
					return;
				}
			}

			throw std::runtime_error("Pointer is not owned by this allocator.");
		}

		/*!
		 * \brief performs a function on every active instance of T owned by this allocator
		 */
		static void ForEach(std::function<void(T*)> func)
		{
			std::shared_lock ulock(internal.mtx);
			for (Node* n = internal.front; n != nullptr; n = n->next)
			{
				func(n->data);
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
			for (Node* n = internal.front; n != nullptr; n = n->next)
			{
				if (filter(n->data)) func(n->data);
			}
		}

		/*!
		 * \brief Returns the number of active instances of T owned by this allocator
		 */
		static Size Count()
		{
			std::shared_lock slock(internal.mtx);
			Size s = 0;
			
			for (Node* n = internal.front; n != nullptr; n = n->next)
			{
				s++;
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
			
			for (Node* n = internal.front; n != nullptr; n = n->next)
			{
				if (filter(n->data)) s++;
			}

			return s;
		}
	};

	template <typename T>
	typename LinkedAllocator<T>::Internal LinkedAllocator<T>::internal;
}
