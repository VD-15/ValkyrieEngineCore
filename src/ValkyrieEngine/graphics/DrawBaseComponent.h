#pragma once

#include "../core/ValkyrieEngine.h"
#include "../utils/Color.h"
#include "TransformComponent.h"

namespace vlk
{
	const Byte VLK_TRANSLUCENT_BIT =		0b00000001;
	const Byte VLK_VERTICAL_FLIP_BIT =		0b00000010;
	const Byte VLK_HORIZONTAL_FLIP_BIT =	0b00000100;
	const Byte VLK_SCREEN_RELATIVE_BIT =	0b00001000;

	struct DrawBaseComponent2D
	{
		DrawBaseComponent2D(const TransformComponent2D* transform);

		Color color;
		Float depth;
		Byte flags;

		const TransformComponent2D* transform;
	};
}
