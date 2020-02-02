#pragma once

#include "DrawBaseComponent.h"

namespace vlk
{
	extern UInt ElipseResolution;

	struct DrawRectangleComponent2D : public DrawBaseComponent2D, public Component<DrawRectangleComponent2D>
	{
		DrawRectangleComponent2D(IEntity* e, const TransformComponent2D* transform);

		Vector2 size;
	};

	struct DrawTriangleComponent2D : public DrawBaseComponent2D, public Component<DrawTriangleComponent2D>
	{
		DrawTriangleComponent2D(IEntity* e, const TransformComponent2D* transform);

		Vector2 p1;
		Vector2 p2;
		Vector2 p3;
	};

	struct DrawElipseComponent2D : public DrawBaseComponent2D, public Component<DrawElipseComponent2D>
	{
		DrawElipseComponent2D(IEntity* e, const TransformComponent2D* transform);

		Vector2 radii;
	};
}
