#pragma once

#include "ValkyrieEngine.h"
#include "IGameEntity.h"

namespace vlk 
{
	class IComponent
	{
		public:
		IComponent(IEntity* e);

		virtual void Delete() = 0;

		inline IEntity* GetParent() { return parent; }

		private:
		IEntity* parent;
	};
}