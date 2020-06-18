#pragma once

#include "ValkyrieEngine/IComponent.hpp"
#include "ValkyrieEngine/ChunkAllocator.hpp"
#include <cassert>

namespace vlk
{
	/*!
	 * \brief Base class for user-defined components to inherit from.
	 * \tparam T The derrived component class
	 * \tparam ComponentAllocator The allocator class used to allocate instances of this component
	 *
	 * \code
	 * //MyComponent uses a LinkedAllocator to allocate its instances
	 * class MyComponent : public vlk::Component<MyComponent, vlk::LinkedAllocator<MyComponent>>
	 * {
	 *     public:
	 *     MyComponent(IEntity* e) :
	 *	       Component<MyComponent>(e)
	 *     { ... }
	 *
	 *     //...
	 * }
	 * \endcode
	 */
	template <typename T, typename ComponentAllocator = ChunkAllocator<T, 64>>
	class Component : public IComponent
	{
		// If you have a smaller mutex implementation that's reasonably fast, please let me know. ~Vee
		std::mutex mtx;

		public:

		typedef ComponentAllocator Allocator;

		/*!
		 * \brief Constructs a component
		 * \param e Pointer to the entity this component is attached to
		 */
		Component<T, Allocator>(IEntity* e) :
			IComponent(e)
		{
			assert(e != nullptr);
		}

		Component<T, Allocator>(const Component<T, Allocator>&) = delete;
		Component<T, Allocator>(Component<T, Allocator>&&) = delete;
		Component<T, Allocator>& operator=(const Component<T, Allocator>&) = delete;
		Component<T, Allocator>& operator=(Component<T, Allocator>&&) = delete;

		virtual ~Component<T, Allocator>()
		{
			static_assert(std::is_base_of<Component<T, Allocator>, T>::value, "T must inherit from Component");
			static_assert(IsAllocator<Allocator, T>::Value, "ComponentAllocator must meet the requirements for IsAllocator");
		}

		/*!
		 * \brief Overloads the \c new operator of this class to use an allocator instead of \c malloc
		 */
		[[nodiscard]] void* operator new(Size size)
		{
			return Allocator::Allocate();
		}

		/*!
		 * \brief Overloads the \c delete operator of this class to use an allocator instead of \c free
		 */
		void operator delete(void* ptr)
		{
			Allocator::Deallocate(ptr);
		}

		/*!
		 * \brief Locks this component to allow thread-safe access. Blocks the calling thread if this component is already locked.
		 * \return A unique_lock object that owns this component, destroy it to unlock the component.
		 */
		[[nodiscard]] inline std::unique_lock<std::mutex> Lock()
		{
			return std::unique_lock(mtx);
		}

		/*!
		 * \brief performs an action on every component of this type. Does not lock the component by default,
		 * if simultaneous access is a concern, see Lock()
		 * \param func The function object to be performed
		 * \sa #Lock()
		 */
		constexpr static void ForEach(std::function<void(T*)> func)
		{
			Allocator::ForEach(func);
		}

		/*!
		 * \brief performs an action on every component that matches a filter. Neither function locks the component by default,
		 * if simultaneous access is a concern, see Lock()
		 * \param func The function object to be performed
		 * \param fiter The filter for objects to be checked against
		 * \c func will be called on every component where \c filter returns true
		 * \sa #Lock()
		 */
		constexpr static void ForEach(std::function<void(T*)> func, std::function<bool(const T*)> filter)
		{
			Allocator::forEach(func, filter);
		}

		/*!
		 * \brief Returns the number of components currently exist
		 */
		constexpr static Size Count()
		{
			return Allocator::Count();
		}

		/*!
		 * \brief Returns the number of components for which \c filter returns true. The filter does not lock components
		 * by default, if simultaneous access is a concern, see Lock()
		 * \sa #Lock()
		 */
		constexpr static Size Count(std::function<bool(const T*)> filter)
		{
			return Allocator::Count(filter);
		}
	};
}
