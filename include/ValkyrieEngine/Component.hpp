/*!
 * \file Component.hpp
 * \brief Provides ECS component class
 */

#ifndef VLK_COMPONENT_HPP
#define VLK_COMPONENT_HPP

#include "ValkyrieEngine/AllocChunk.hpp"
#include "ValkyrieEngine/IComponent.hpp"
#include "ValkyrieEngine/Entity.hpp"

#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <functional>
#include <shared_mutex>

namespace vlk
{
	/*!
	 * \brief Hint struct used to specify some component-related behaviour.
	 *
	 * \sa GetComponentHints()
	 * \sa Component
	 */
	struct ComponentHints
	{
		/*!
		 * \brief Number of components in a single allocation block.
		 *
		 * Memory for components is allocated internally using fixed-size storage blocks.
		 * This hint is used to specify the number of components that can fit in each storage block.
		 *
		 * \sa allocAutoResize
		 */
		const Size allocBlockSize = 64;

		/*!
		 * \brief Whether component allocation blocks should be created as they are needed.
		 *
		 * Memory for cmponents is allocated internally using fixed-size storage blocks.
		 * 
		 * If this hint is true, new storage blocks will be created as they fill up,
		 * providing effectively unbounded storage for components.
		 *
		 * If this hint is false, a maximum of one block will be allocated, bounding the
		 * total number of components that can exist at any one time to allocBlockSize.
		 * If the storage block is full, attempting to allocate another component will
		 * throw a <tt>std::range_error</tt>.
		 *
		 * \sa allocBlockSize
		 */
		const bool allocAutoResize = true;
	};

	/*!
	 * \brief Function used to retrieve the hints for a specified component type.
	 *
	 * This function can be specialized to override the hints used by a component type.
	 *
	 * \code
	 * struct SomeData {...};
	 *
	 * // Specify hints used by Component<SomeData>
	 * template <>
	 * VLK_CXX14_CONSTEXPR inline ComponentHints GetComponentHints<SomeData> { return ComponentHints {10, false}; }
	 * \endcode
	 *
	 * \sa ComponentHints
	 * \sa Component
	 */
	template <typename T>
	VLK_CXX14_CONSTEXPR inline ComponentHints GetComponentHints() { return ComponentHints {}; }

	/*!
	 * \brief Template class for ECS components
	 *
	 * Components are storage objects that define little to no logic.
	 * Components can be attached to entities with Attach() to provide the entity with some sort of property.
	 * A Component attached to an entity can then be fetched using FindOne(EntityID) or FindMany(EntityID).
	 * Components can also be accessed or modified by passing a function object to either ForEach(std::function<void(const T*)>) or ForEach(std::function<void(T*)>)
	 *
	 * Some behaviour related to Components can be controlled by specializing GetComponentHints() for T.
	 *
	 * \tparam T The data this entity is storing. Ideally this would be a POD struct, but any type with at least one public constructor and a public destructor will work.
	 *
	 * \sa EntityID
	 * \sa Entity::Create()
	 * \sa ComponentHints
	 * \sa GetComponentHints()
	 */
	template <typename T>
	class Component final : public IComponent, public T
	{
		public:
		static VLK_CXX14_CONSTEXPR Size ChunkSize = GetComponentHints<T>().allocBlockSize;
		static VLK_CXX14_CONSTEXPR bool AllocResize = GetComponentHints<T>().allocAutoResize;
		typedef AllocChunk<Component<T>, ChunkSize> ChunkType;
		//typedef typename std::vector<ChunkType*>::iterator Iterator;
		//typedef typename std::vector<ChunkType*>::const_iterator ConstIterator;

		VLK_STATIC_ASSERT_MSG(std::is_class<T>::value, "T must be a class or struct type.");

		private:
		static VLK_SHARED_MUTEX_TYPE s_mtx;
		static std::vector<ChunkType*> s_chunks;

		///////////////////////////////////////////////////////////////////////

		template <typename... Args>
		Component<T>(Args... args) :
			T(std::forward<Args>(args)...)
		{ }

		~Component<T>() = default;

		///////////////////////////////////////////////////////////////////////

		public:
		Component<T>(const Component<T>&) = delete;
		Component<T>(Component<T>&&) = delete;
		Component<T>& operator=(const Component<T>&) = delete;
		Component<T>& operator=(Component<T>&&) = delete;

		///////////////////////////////////////////////////////////////////////

		/*!
		 * \brief Creates a component.
		 *
		 * \param eId The entity to attach the new component to.
		 * \param args Arguments to be forwarded to the constructor for T.
		 *
		 * \return A pointer to the new component.
		 * This component should be destroyed either by calling Delete on it, or calling Entity::Delete on the entity it's attached to.
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking is handled internally.<br>
		 * Unique access to this class is required.<br>
		 * This function may block the calling thread.<br>
		 *
		 * \code{.cpp}
		 * class MyData
		 * {
		 *     public:
		 *     int foo;
		 *
		 *     MyData(int f) : foo(f) { }
		 *     ~MyData() = default;
		 * };
		 *
		 * EntityID eId = Entity::Create();
		 * 
		 * Component<MyData>* component = Component<MyData>::Create(eId, 5);
		 * \endcode
		 *
		 * \sa Entity::Create()
		 * \sa Delete()
		 */	
		template <typename... Args>
		static Component<T>* Create(EntityID eId, Args... args)
		{
			std::unique_lock<VLK_SHARED_MUTEX_TYPE> ulock(s_mtx);

			VLK_STATIC_ASSERT_MSG((std::is_constructible<T, Args...>::value), "Cannot construct an instance of T from the provided args.");

			for (auto it = s_chunks.begin(); it != s_chunks.end(); it++)
			{
				ChunkType* ch = *it;
				if (!ch->Full())
				{
					Component<T>* c = new (ch->Allocate()) Component<T>(std::forward<Args>(args)...);
					c->entity = eId;
					ECRegistry<IComponent>::AddEntry(eId, static_cast<IComponent*>(c));
					ECRegistry<Component<T>>::AddEntry(eId, c);
					return c;
				}
			}

			if (AllocResize | (s_chunks.size() == 0))
			{
				//emplace_back returns void until C++17, so we can't use it here
				s_chunks.push_back(new ChunkType());
				Component<T>* c = new (s_chunks.back()->Allocate()) Component<T>(std::forward<Args>(args)...);
				c->entity = eId;
				ECRegistry<IComponent>::AddEntry(eId, static_cast<IComponent*>(c));
				ECRegistry<Component<T>>::AddEntry(eId, c);
				return c;
			}
			else
			{
				throw std::range_error("Maximum number of component allocations reached.");
				return nullptr;
			}
		}

		///////////////////////////////////////////////////////////////////////

		/*!
		 * \brief Deletes this component.
		 *
		 * If this component is attached to an entity, it is detached.
		 * Also calls the destructor for this object.
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking is handled internally.<br>
		 * Unique access to this class is required.<br>
		 * Unique access to the ECRegistry<IComponent> class is required.<br>
		 * Unique access to the ECRegistry<Component<T>> class is required.<br>
		 * This function may block the calling thread.<br>
		 *
		 * \code{.cpp}
		 * Component<MyData>* component = Component<MyData>::Create(eId, 5);
		 *
		 * // Do something with component.
		 *
		 * component->Delete();
		 * \endcode
		 *
		 * \sa Create(EntityID, Args)
		 */
		virtual void Delete() final override
		{
			std::unique_lock<VLK_SHARED_MUTEX_TYPE> ulock(s_mtx);

			ECRegistry<IComponent>::RemoveOne(this->entity, static_cast<IComponent*>(this));
			ECRegistry<Component<T>>::RemoveOne(this->entity, this);

			// Call destructor
			this->~Component<T>();

			// Free chunk memory
			for (auto it = s_chunks.begin(); it != s_chunks.end(); it++)
			{
				ChunkType* c = *it;

				if (c->OwnsPointer(this))
				{
					// Free Memory occupied by this component
					c->Deallocate(this);

					// Erase chunk if empty
					if (c->Empty())
					{
						// Invalidates iterator
						s_chunks.erase(it);
					}

					return;
				}
			}
		}

		///////////////////////////////////////////////////////////////////////

		/*!
		 * \brief Attaches this component to an entity.
		 *
		 * \param eId The entity to attach this component to.
		 *
		 * A component can only be attached to one entity at a time,
		 * if this component is already attached to an entity when Attach is called,
		 * it is detached from that entity and attached to the new one.
		 * Components cannot exist detached from an entity.
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking is handled internally.<br>
		 * Unique access to this class is required.<br>
		 * Unique access to the ECRegistry<IComponent> class is required.<br>
		 * Unique access to the ECRegistry<Component<T>> class is required.<br>
		 * This function may block the calling thread.<br>
		 *
		 * \code{.cpp}
		 * EntityID e1 = Entity::Create();
		 * EntityID e2 = Entity::Create();
		 *
		 * Component<MyData>* component = Component<MyData>::Create(e1);
		 * component->Attach(e2);
		 * \endcode
		 *
		 * \sa Entity::Create()
		 * \sa Entity::Global
		 * \sa FindOne(EntityID)
		 */
		void Attach(EntityID eId)
		{
			std::unique_lock<VLK_SHARED_MUTEX_TYPE> ulock(s_mtx);
			ECRegistry<IComponent>::RemoveOne(this->entity, static_cast<IComponent*>(this));
			ECRegistry<Component<T>>::RemoveOne(this->entity, this);

			this->entity = eId;

			ECRegistry<IComponent>::AddEntry(this->entity, static_cast<IComponent*>(this));
			ECRegistry<Component<T>>::AddEntry(this->entity, this);
		}

		///////////////////////////////////////////////////////////////////////

		/*!
		 * \brief Find a component attached to an entity.
		 *
		 * \return One component of this type attached to the given entity.
		 * \return <tt>nullptr</tt> if no component of this type is found.
		 * If multiple components of this type are attached to the given entity,
		 * no guarantees are made as to which component will be returned.
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking is handled internally.<br>
		 * Shared access to the ECRegistry<Component<T>> class is required.<br>
		 * This function may block the calling thread.<br>
		 *
		 * \code{.cpp}
		 * Component<MyData>* component = Component<MyData>::FindOne(entity);
		 * \endcode
		 *
		 * \sa Attach(EntityID)
		 * \sa FindAll(EntityID)
		 */
		VLK_NODISCARD static inline Component<T>* FindOne(EntityID id)
		{
			return ECRegistry<Component<T>>::LookupOne(id);
		}

		///////////////////////////////////////////////////////////////////////

		/*!
		 * \brief Find components attached to an entity.
		 *
		 * \param id The entity to find components for.
		 *
		 * \param vecOut A vector to write the found components to.
		 * Existing contents are not modified, but the vector may be resized.
		 *
		 * \return The number of components appended to vecOut
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking is handled internally.<br>
		 * Shared access to the ECRegistry<Component<T>> class is required.<br>
		 * This function may block the calling thread.<br>
		 *
		 * \code{.cpp}
		 * std::vector<Component<MyData>*> components;
		 * Component<MyData>::FindMany(entity, components);
		 *
		 * for (Component<MyData>* : components)
		 * {
		 *     ...
		 * }
		 * \endcode
		 *
		 * \sa Attach(EntityID)
		 * \sa FindOne(EntityID)
		 */
		static inline Size FindAll(EntityID id, std::vector<Component<T>*>& vecOut)
		{
			return ECRegistry<Component<T>>::LookupAll(id, vecOut);
		}

		///////////////////////////////////////////////////////////////////////

		/*!
		 * \brief Performs a mutating function on every instance of this component.
		 *
		 * \param func A function object that returns void and accepts a single pointer to a Component<T>.
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking is handled internally.<br>
		 * Unique access to this class is required.<br>
		 * This function may block the calling thread.<br>
		 *
		 * \code{.cpp}
		 * struct MyData
		 * {
		 *     int foo = 0;
		 *     bool bar = false;
		 * };
		 *
		 * Component<MyData>::ForEach([](Component<MyData*> c)
		 * {
		 *     if (c->bar)
		 *     {
		 *        c->foo = 15;
		 *     }
		 * });
		 *
		 * \endcode
		 *
		 * \sa vlk::Component<T>::CForEach(std::function<void(const Component<T>*)>)
		 * \sa vlk::Component<T>::ForEach(std::function<void(Iterator begin, Iterator end)>)
		 */
		static void ForEach(std::function<void(Component<T>*)> func)
		{
			std::unique_lock<VLK_SHARED_MUTEX_TYPE> ulock(s_mtx);

			for (auto it = s_chunks.begin(); it != s_chunks.end(); it++)
			{
				//Dereference here to avoid ambiguity
				ChunkType* ch = *it;

				for (Size i = 0; i < ChunkType::ChunkSize; i++)
				{
					if (ch->IsOccupied(i))
					{
						func(ch->At(i));
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////

		/*!
		 * \brief Performs a non-mutating function on every instance of this component.
		 *
		 * \param func A function object that returns void and accepts a single pointer to a const Component<T>.
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking is handled internally.<br>
		 * Shared access to this class is required.<br>
		 * This function may block the calling thread.<br>
		 *
		 * \code{.cpp}
		 * struct MyData
		 * {
		 *     bool bar = false;
		 * };
		 *
		 * std::vector<const Component<MyData*>> components;
		 *
		 * Component<MyData>::CForEach([](const Component<MyData*> c)
		 * {
		 *     if (c->foo) components.insert_back(c);
		 * });
		 * \endcode
		 *
		 * \sa ForEach(std::function<void(Component<T>*)>)
		 */
		static void CForEach(std::function<void(const Component<T>*)> func)
		{
			std::shared_lock<VLK_SHARED_MUTEX_TYPE> slock(s_mtx);

			for (auto it = s_chunks.cbegin(); it != s_chunks.cend(); it++)
			{
				//Dereference here to avoid ambiguity
				const ChunkType* ch = *it;

				for (Size i = 0; i < ChunkType::ChunkSize; i++)
				{
					if (ch->IsOccupied(i))
					{
						func(ch->At(i));
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////

		/*!
		 * \brief Returns the number of instances of this component that currently exist.
		 *
		 * If <tt>GetComponentHints<T>().allocAutoResize</tt> evaluates to true, this number will not exceed the value specified by <tt>GetComponentHints<T>().allocBlockSize</tt>
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking is handled internally.<br>
		 * Shared access to this class is required.<br>
		 * This function may block the calling thread.<br>
		 *
		 * \code{.cpp}
		 * struct MyData
		 * {
		 *     ...
		 * };
		 *
		 * std::vector<const Component<MyData>> components;
		 * components.reserve(Component<MyData>::Count());
		 *
		 * Component<MyData>::CForEach([](const Component<MyData> c)
		 * {
		 *     components.insert_back(c);
		 * });
		 * \endcode
		 *
		 * \sa ChunkCount()
		 */
		static Size Count()
		{
			std::shared_lock<VLK_SHARED_MUTEX_TYPE> slock(s_mtx);

			Size total = 0;

			for (auto it = s_chunks.cbegin(); it != s_chunks.cend(); it++)
			{
				total += (*it)->Count();
			}

			return total;
		}

		///////////////////////////////////////////////////////////////////////
		
		/*!
		 * \brief Returns the number of storage blocks this component has allocated.
		 *
		 * If <tt>GetComponentHints<T>().allocAutoResize</tt> evaluates to true, this number will not exceed 1.
		 *
		 * \ts
		 * May be called from any thread.<br>
		 * Resource locking is handled internally.<br>
		 * Shared access to this class is required.<br>
		 * This function may block the calling thread.<br>
		 *
		 * \sa Count()
		 */
		static Size ChunkCount()
		{
			std::shared_lock<VLK_SHARED_MUTEX_TYPE> slock(s_mtx);
			return s_chunks.size();
		}

		///////////////////////////////////////////////////////////////////////
	};

	template <typename T>
	VLK_SHARED_MUTEX_TYPE Component<T>::s_mtx;

	template <typename T>
	std::vector<typename Component<T>::ChunkType*> Component<T>::s_chunks;
}

#endif
