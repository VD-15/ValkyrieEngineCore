#include "ValkyrieEngine/Component.hpp"
#include "ValkyrieEngine/ChunkAllocator.hpp"
#include "ValkyrieEngine/Signal.hpp"
#include <type_traits>

namespace vlk
{
	/*!
	 * \brief Base class for user-defined entities to inherit from
	 * \sa Component
	 */
	template<class T, class EntityAllocator = ChunkAllocator<T, 64>>
	class Entity : public IEntity
	{
		std::vector<ISignalListener*> listeners;
		std::mutex mtx;

		public:

		typedef EntityAllocator Allocator;

		Entity<T, EntityAllocator>()
		{

		}

		virtual ~Entity<T, EntityAllocator>()
		{
			static_assert(std::is_base_of<Entity<T>, T>::value, "T must inherit from Entity");
			static_assert(IsAllocator<EntityAllocator, T>::Value, "EntityAllocator must meet the requirements for IsAllocator");
		}

		/*!
		 * \brief Overloads the \c new operator to use an allocator instead of \c malloc
		 */
		[[nodiscard]] void* operator new(Size size)
		{
			return EntityAllocator::Allocate();
		}

		/*!
		 * \brief  Overloads the \c dleete operator to use an allocator instead of \c delete
		 */
		void operator delete(void* ptr)
		{
			EntityAllocator::Deallocate(ptr);
		}

		/*!
		 * \brief Helper function to add a component to an entity
		 * \return The newly created component
		 * Assumes the first argument is an IEntity* to be given to the constructor of the component
		 * Equivelant to the following constructor:
		 * \code
		 * T(this, args...);
		 * \endcode
		 */
		template <typename U, typename... Args>
		[[nodiscard]] constexpr U* AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of<Component<U>, U>::value, "U is not a component.");
			return new U(static_cast<IEntity*>(this), std::forward<Args>(args)...);
		}

		/*!
		 * \brief Adds a signal listener to the calling list of this entity.
		 * \sa #RemoveSignalListener(vlk::ISignalListener*)
		 * \sa #SendSignal(const vlk::Signal&)
		 */
		virtual void AddSignalListener(ISignalListener* listener) final override
		{
			std::unique_lock ulock(mtx);
			if (std::find(listeners.begin(), listeners.end(), listener) == listeners.end())listeners.push_back(listener);
		}

		/*!
		 * \brief Removes a signal listener from the calling list of this entity.
		 * \sa #AddSignalListener(ISignalListener*)
		 * \sa #SendSignal(const vlk::Signal&)
		 */
		virtual void RemoveSignalListener(ISignalListener* listener) final override
		{
			std::unique_lock ulock(mtx);
			listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
		}

		/*!
		 * \brief Sends a signal to every signal listener attached to this entity.
		 * \sa #AddSignalListener(ISignalListener*)
		 * \sa #RemoveSignalListener(ISignalListener*)
		 */
		virtual void SendSignal(const Signal& signal) final override
		{
			std::unique_lock ulock(mtx);
			for (auto it = listeners.begin(); it != listeners.end(); it++)
			{
				(*it)->OnSignal(signal);
			}
		}
	};
}
