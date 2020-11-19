/*!
 * \file ECS.hpp
 * \brief Utilities required for the entity-component system to function.
 */

#ifndef VLK_ENTITYID_HPP
#define VLK_ENTITYID_HPP

#include "ValkyrieEngine/ValkyrieDefs.hpp"

#include <unordered_map>
#include <shared_mutex>
#include <vector>

namespace vlk
{
	/*!
	 * \brief Typedef for Entity Identifiers
	 *
	 * \sa Entity::Create()
	*/
	typedef ULong EntityID;

	/*!
	 * \brief Tracks what components are attached to what entities.
	 *
	 * Direct use is generally discouraged, consider using the wrappers available in Component<T> and Entity instead.
	 *
	 * \tparam C The component type the registry is tracking.
	 */
	template <typename C>
	class ECRegistry
	{
		static std::unordered_multimap<EntityID, C*> reg;
		static std::shared_mutex mtx;

		public:

		/*!
		 * \brief Association insertion function.
		 *
		 * Associates a component with an entity. Does not remove any existing associations <tt>component</tt> may have.
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking is handled internally.<br>
		 * Unique access to this class is required.<br>
		 * This function may block the calling thread<br> 
		 *
		 * \sa RemoveOne(EntityID, C*)
		 * \sa Component<T>::Attach(EntityID)
		 */
		static void AddEntry(EntityID entity, C* component)
		{
			std::unique_lock ulock(mtx);

			/* I don't think this will ever come into play?
			{// Erase existing entry if exists
				auto search = reg.equal_range(entity);

				for (auto it = search.first; it != search.second; it++)
				{
					if (it->second == component)
					{
						reg.erase(it);
						break;
					}
				}
			}
			*/

			// Emplace entry
			reg.emplace(entity, component);
		}

		/*!
		 * \brief Association removal function.
		 *
		 * Removes every association an entity has with components of type C.
		 *
		 * \return The number of associations that have been erased.
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking is handled internally.<br>
		 * Unique access to this class is required.<br>
		 * This function may block the calling thread<br> 
		 *
		 * \sa RemoveOne(EntityID, C* component)
		 * \sa LookupAll(EntityID)
		 * \sa Entity::Delete(EntityID)
		 */
		static Size RemoveAll(EntityID entity)
		{
			std::unique_lock ulock(mtx);
			return reg.erase(entity);
		}

		/*!
		 * \brief Association removal function.
		 *
		 * Removes one association an entity has with a component.
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking is handled internally.<br>
		 * Unique access to this class is required.<br>
		 * This function may block the calling thread<br> 
		 *
		 * \sa RemoveAll(EntityID)
		 * \sa LookupOne(EntityID)
		 * \sa Component<T>::Attach(EntityID)
		 */
		static void RemoveOne(EntityID entity, C* component)
		{
			std::unique_lock ulock(mtx);

			{// Erase existing entry if exists
				auto search = reg.equal_range(entity);

				for (auto it = search.first; it != search.second; it++)
				{
					if (it->second == component)
					{
						reg.erase(it);
						return;
					}
				}
			}
		}

		/*!
		 * \brief Association lookup function
		 *
		 * \return One component of type C that has an association
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking is handled internally.<br>
		 * Shared access to this class is required.<br>
		 * This function may block the calling thread<br>
		 *
		 * \sa Component<T>::FindOne(EntityID)
		 */
		static C* LookupOne(EntityID entity)
		{
			std::shared_lock slock(mtx);

			auto it = reg.find(entity);

			return (it == reg.end()) ? nullptr : it->second;
		}

		/*!
		 * \brief Association lookup function.
		 *
		 * Retrieves all accosiations an entity has with components of type C.
		 *
		 * \param entity The entity to find associations for.
		 *
		 * \param vecOut A vector to write associated components to.
		 * Associated components are inserted at the end of the vector.
		 * Existing contents are not modified or rearranged.
		 * May be resized to accomodate new elements.
		 *
		 * \return The number of components written to the vector
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking is handled internally.<br>
		 * Shared access to this class is required.<br>
		 * This function may block the calling thread<br>
		 *
		 * \sa Component<T>::FindAll(EntityID, std::vector<Component<T>*>&)
		 */

		static Size LookupAll(EntityID entity, std::vector<C*>& vecOut)
		{
			std::shared_lock slock(mtx);

			auto search = reg.equal_range(entity);
			Size numEntries = std::distance(search.first, search.second);

			vecOut.reserve(vecOut.size() + numEntries);

			for (auto it = search.first; it != search.second; it++)
			{
				vecOut.push_back(it->second);
			}

			return numEntries;
		}
	};

	template <typename C>
	std::unordered_multimap<EntityID, C*> ECRegistry<C>::reg;

	template <typename C>
	std::shared_mutex ECRegistry<C>::mtx;
}

#endif
