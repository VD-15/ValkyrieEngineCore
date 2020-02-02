#include "IGameComponent.h"

using namespace vlk;

IComponent::IComponent(IEntity* e) :
	parent(e)
{

}