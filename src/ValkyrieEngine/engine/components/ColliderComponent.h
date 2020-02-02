#pragma once

#include "../core/GameComponent.hpp"
#include "TransformComponent.h"
#include "../core/Event.hpp"

namespace vlk
{
	namespace CollisionSystem
	{
		void Init();
		void Destroy();

		void AddCollidingLayer(ULong layer1, ULong layer2);
		Boolean DoLayersCollide(ULong layer1, ULong layer2);
	}

	struct ColliderBaseComponent2D
	{
		ColliderBaseComponent2D(TransformComponent2D* transform);

		//Gets the projection of the shape of this collider onto the given axis.
		//min and max are two floats to be written to that, when multiplied by
		//axis, give two Vector2's representing the bounds of the projection.
		virtual void GetProjection(const Vector2& axis, Float& min, Float& max) const = 0;

		virtual std::vector<Vector2> GetAxes() const = 0;

		IEntity* GetParent();
		
		TransformComponent2D* transform;

		ULong layer;
	};

	struct CircleCollider2D : public ColliderBaseComponent2D, public Component<CircleCollider2D>
	{
		CircleCollider2D(IEntity* e, TransformComponent2D* transform);

		void GetProjection(const Vector2& axis, Float& min, Float& max) const override;

		std::vector<Vector2> GetAxes() const override;

		Vector2 radii;
	};
	
	struct RectangleCollider2D : public ColliderBaseComponent2D, public Component<RectangleCollider2D>
	{
		RectangleCollider2D(IEntity* e, TransformComponent2D* transform);

		void GetProjection(const Vector2& axis, Float& min, Float& max) const override;

		std::vector<Vector2> GetAxes() const override;

		Vector2 radii;
	};
	
	struct CollisionEvent2D
	{
		ColliderBaseComponent2D* const collider1;
		ColliderBaseComponent2D* const collider2;
	};

}