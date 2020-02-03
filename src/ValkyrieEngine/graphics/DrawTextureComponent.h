#pragma once

#include "ValkyrieEngine/common/TransformComponent.h"
#include "Texture.h"

namespace vlk
{
	struct DrawTextureComponent2D : public Component<DrawTextureComponent2D>
	{
		DrawTextureComponent2D(IEntity* e, const TransformComponent2D* transform, const Texture2D* texture);

		const Texture2D* texture;
		const TransformComponent2D* transform;

		Color color;
		Float depth;
		Byte flags;
	};
}