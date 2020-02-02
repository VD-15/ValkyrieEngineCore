#include "DrawTextureComponent.h"

using namespace vlk;

DrawTextureComponent2D::DrawTextureComponent2D(IEntity* e, const TransformComponent2D* transform, const Texture2D* texture) :
	Component<DrawTextureComponent2D>(e),
	DrawBaseComponent2D(transform)
{
	this->transform = transform;
	this->texture = texture;
}