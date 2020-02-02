#pragma once

#include "ValkyrieEngine.h"
#include "IGameEntity.h"
#include "GameComponent.hpp"
#include "ChunkAllocator.hpp"

namespace vlk
{
	template <class T, class Allocator = ChunkAllocator<T>>
	class Entity : public IEntity
	{
		public:
		Entity()
		{
			static_assert(std::is_base_of<IEntity, T>::value, "T must inherit from IEntity");
		}

		Entity(const Entity& other) = delete;
		Entity(Entity&& other) = delete;
		Entity& operator=(const Entity& other) = delete;

		template<class... U>
		static T* CreateEntity(U&&... args)
		{
			T* t = Allocator::AllocateNew(std::forward<U>(args)...);

			return t;
		}

		template<class U, class... V>
		U* AddComponent(V&&... args)
		{
			static_assert(std::is_base_of<IComponent, U>::value, "U must inherit from IComponent");

			return U::Allocator::AllocateNew(static_cast<IEntity*>(this), std::forward<V>(args)...);
		}

		//This will crash the entire application if you're calling this from within a ForEach.
		//Don't call this from within a ForEach. Please.
		virtual void Delete() final override
		{
			Allocator::Delete(dynamic_cast<T*>(this));
		}
	};
}
