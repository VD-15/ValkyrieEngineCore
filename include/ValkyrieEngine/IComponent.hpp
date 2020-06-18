#pragma once
#include "ValkyrieEngine/IEntity.hpp"

namespace vlk
{
	class IComponent : public ISignalListener
	{
		IEntity* const entity;

		public:
		IComponent(IEntity* e) :
			entity(e)
		{
			this->entity->AddSignalListener(static_cast<ISignalListener*>(this));
		}

		IComponent(const IComponent&) = delete;
		IComponent(IComponent&&) = delete;
		IComponent& operator=(const IComponent&) = delete;
		IComponent& operator=(IComponent&&) = delete;
		virtual ~IComponent()
		{
			this->entity->RemoveSignalListener(static_cast<ISignalListener*>(this));
		}

		inline IEntity* GetEntity() { return this->entity; }

		virtual void OnSignal(const Signal& signal) override {}
	};
}
