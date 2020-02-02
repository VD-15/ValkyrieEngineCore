#pragma once

#include "DrawBaseComponent.h"
#include "../graphics/Texture.h"

namespace vlk
{
	struct DrawTextureComponent2D :public DrawBaseComponent2D, public Component<DrawTextureComponent2D>
	{
		DrawTextureComponent2D(IEntity* e, const TransformComponent2D* transform, const Texture2D* texture);

		const Texture2D* texture;
	};
}