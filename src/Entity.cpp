#include "ValkyrieEngine/Entity.hpp"
#include <unordered_map>
#include <mutex>
#include <vector>

using namespace vlk;

namespace
{
	std::mutex mtx;
}

EntityID Entity::Create()
{
	std::unique_lock ulock(mtx);
	static EntityID eId = static_cast<EntityID>(0);

	// Incriment and return; Should never return invalid unless overflow occurs
	return ++eId;
}

void Entity::Delete(EntityID id)
{
	std::unique_lock ulock(mtx);
	std::vector<IComponent*> toRemove;

	ECRegistry<IComponent>::LookupAll(id, toRemove);

	// Components call Unregister in their Delete function,
	// this would invalidate search iterators above and cause a resource deadlock.
	for (auto it = toRemove.begin(); it != toRemove.end(); it++)
	{// Delete component
		(*it)->Delete();
	}
}
