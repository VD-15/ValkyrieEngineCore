/*!
 * \file Entity.hpp
 * \brief Provides Entity utilities
 */

#ifndef VLK_ENTITY_HPP
#define VLK_ENTITY_HPP

#include "ValkyrieEngine/ValkyrieDefs.hpp"
#include "ValkyrieEngine/IComponent.hpp"
#include "ValkyrieEngine/ECS.hpp"

namespace vlk
{
	namespace Entity
	{
		/*!
		 * \brief A globally accessible entity with an ID of zero.
		 *
		 * This can be used as a persistent entity to attach components to.
		 */
		VLK_CXX14_CONSTEXPR EntityID global = static_cast<EntityID>(0);

		/*!
		 * \brief Creates an entity.
		 *
		 * EntityIDs are created by an internal counter.
		 * This counter starts at 1 and incriments by 1 each time Create() is called.
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking is handled internally.<br>
		 * Unique access to this namespace is acquired by the function.<br>
		 * This function may block the calling thread<br>
		 *
		 * \sa Delete(EntityID)
		 * \sa Component<T>::Create(EntityID, Args...)
		 * \sa Component<T>::Attach(EntityID)
		 */
		VLK_NODISCARD EntityID Create();

		/*!
		 * \brief Deletes an entity and all components attached to it.
		 *
		 * \param id The entity to delete.
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking is handled internally.<br>
		 * Unique access to this namespace is acquired by the function.<br>
		 * Unique access to the classes of any attached components is acquired by the function.<br>
		 * This function may block the calling thread<br>
		 */
		void Delete(EntityID eId);
	};
}

#endif
