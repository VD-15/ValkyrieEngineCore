/*!
 * \file IComponent.hpp
 * \brief Provides base class for components
 */

#ifndef VLK_ICOMPONENT_HPP
#define VLK_ICOMPONENT_HPP

#include "ValkyrieEngine/ECS.hpp"

namespace vlk
{
	/*!
	 * \brief Unspecialized base class for ValkyrieEngine components
	 *
	 * \sa vlk::Component<T>
	 */
	class IComponent
	{
		protected:
		//! The Entity this component is attached to.
		EntityID entity;

		public:
		//! Gets the id of the Entity this component is attached to.
		inline EntityID GetEntity() const { return entity; }

		/*!
		 * \copydoc Component<T>::Delete()
		 */
		virtual void Delete() = 0;
	};
}

#endif
