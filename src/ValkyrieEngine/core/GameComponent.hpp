#pragma once

#include "ValkyrieEngine.h"
#include "IGameComponent.h"
#include "IGameEntity.h"
#include "ChunkAllocator.hpp"

namespace vlk
{
	template<class T, class Allocator = ChunkAllocator<T>>
	class Component : public IComponent
	{
		public:
		Component(IEntity* e) : IComponent(e) {}

		Component() = delete;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		static inline void ForEach(const std::function<void(T*)>& f)
		{
			Allocator::ForEach(f);
		}

		static inline ULong GetCount()
		{
			return Allocator::GetCount();
		}

		static inline Boolean HasInstances()
		{
			return Allocator::HasInstances();
		}

		virtual void Delete() final override
		{
			Allocator::Delete(dynamic_cast<T*>(this));
		}
	};
}