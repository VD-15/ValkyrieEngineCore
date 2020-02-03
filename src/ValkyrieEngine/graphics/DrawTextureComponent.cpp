#include "DrawTextureComponent.h"

using namespace vlk;

DrawTextureComponent2D::DrawTextureComponent2D(IEntity* e, const TransformComponent2D* transform, const Texture2D* texture) :
	Component<DrawTextureComponent2D>(e)
{
	this->transform = transform;
	this->texture = texture;
	this->color = Colors::White;
	this->depth = 0.0f;
	this->flags = 0;
}