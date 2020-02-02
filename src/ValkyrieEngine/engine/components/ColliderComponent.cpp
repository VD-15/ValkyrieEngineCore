#include "ColliderComponent.h"

using namespace vlk;
/*
namespace
{
	std::vector<std::pair<ULong, ULong>> collidingLayers;

	void OnLateUpdate(const LateUpdateEvent& ev)
	{
		std::vector<ColliderBaseComponent2D*> colliders2D;
		std::vector<std::pair<ColliderBaseComponent2D*, ColliderBaseComponent2D*>> collisions;

		ULong circleCount = CircleCollider2D::GetCount();
		ULong rectangleCount = RectangleCollider2D::GetCount();

		colliders2D.reserve(circleCount + rectangleCount);

		auto getCircles = CircleCollider2D::ForEach([&colliders2D](CircleCollider2D* c)
		{
			colliders2D.push_back(dynamic_cast<ColliderBaseComponent2D*>(c));
		});

		auto getRectangles = RectangleCollider2D::ForEach([&colliders2D](RectangleCollider2D* c)
		{
			colliders2D.push_back(dynamic_cast<ColliderBaseComponent2D*>(c));
		});

		for (auto first = colliders2D.begin(); first != colliders2D.end(); first++)
		{
			for (auto second = first + 1; second != colliders2D.end(); second++)
			{
				ColliderBaseComponent2D* c1 = *first;
				ColliderBaseComponent2D* c2 = *second;

				if (std::find(collidingLayers.begin(), collidingLayers.end(), std::make_pair(c1->layer, c2->layer)) == collidingLayers.end()) continue;

				//List of axes to test
				std::vector<Vector2> axes;
				{
					std::vector<Vector2> axes1 = c1->GetAxes();
					std::vector<Vector2> axes2 = c2->GetAxes();

					axes.reserve(axes1.size() + axes2.size());
					axes.insert(axes.end(), axes1.begin(), axes1.end());
					axes.insert(axes.end(), axes2.begin(), axes2.end());

					if (axes1.empty() || axes2.empty())
					{
						axes.push_back(c1->transform->location - c2->transform->location);
					}
				}

				Boolean colliding = true;

				for (auto it = axes.begin(); it != axes.end(); it++)
				{
					Vector2 range1;
					Vector2 range2;

					c1->GetProjection(*it, range1.x, range1.y);
					c2->GetProjection(*it, range2.x, range2.y);

					if (vlk::Compare(range1.y, range2.x) == vlk::Compare(range1.x, range2.y))
					{
						colliding = false;
						break;
					}
				}

				if (colliding)
				{
					collisions.push_back(std::make_pair(c1, c2));
				}
			}
		}

		for (auto it = collisions.begin(); it != collisions.end(); it++)
		{
			//CollisionEvent2D ev(it->first, it->second);
			EventBus<CollisionEvent2D>::Send(it->first, it->second);
		}
	}
}

void CollisionSystem::Init()
{
	EventBus<LateUpdateEvent>::AddListener(OnLateUpdate);
}

void CollisionSystem::Destroy()
{
	EventBus<LateUpdateEvent>::RemoveListener(OnLateUpdate);
}

void CollisionSystem::AddCollidingLayer(ULong layer1, ULong layer2)
{
	collidingLayers.push_back(std::make_pair(layer1, layer2));
	collidingLayers.push_back(std::make_pair(layer2, layer1));
}

Boolean CollisionSystem::DoLayersCollide(ULong layer1, ULong layer2)
{
	return std::find(collidingLayers.begin(), collidingLayers.end(), std::make_pair(layer1, layer2)) != collidingLayers.end();
}

ColliderBaseComponent2D::ColliderBaseComponent2D(TransformComponent2D* transform)
{
	this->transform = transform;
	this->layer = 0;
}

IEntity* ColliderBaseComponent2D::GetParent()
{
	return transform->GetParent();
}

CircleCollider2D::CircleCollider2D(IEntity* e, TransformComponent2D* transform) :
	ColliderBaseComponent2D(transform),
	Component<CircleCollider2D>(e)
{

}

void CircleCollider2D::GetProjection(const Vector2& axis, Float& min, Float& max) const
{
	Float proj = Vector2::GetProjection(transform->location, axis);
	Float theta = Vector2::GetAngleBetween(Vector2(1, 0), axis);
	Float radMag = Vector2(cosf(theta) * (radii.x * transform->scale.x), sinf(theta) * (radii.y * transform->scale.y)).Magnitude();

	min = proj - radMag;
	max = proj + radMag;
}

std::vector<Vector2> CircleCollider2D::GetAxes() const
{
	return std::vector<Vector2>();
}

RectangleCollider2D::RectangleCollider2D(IEntity* e, TransformComponent2D* transform) :
	ColliderBaseComponent2D(transform),
	Component<RectangleCollider2D>(e)
{

}

void RectangleCollider2D::GetProjection(const Vector2& axis, Float& min, Float& max) const
{
	Vector2 sizeSc(radii.x * transform->scale.x, radii.y * transform->scale.y);

	Vector2 vecs[4]
	{
		Vector2(
			transform->location.x - (sizeSc.x),
			transform->location.y + (sizeSc.y)
		),

		Vector2(
			transform->location.x + (sizeSc.x),
			transform->location.y + (sizeSc.y)
		),

		Vector2(
			transform->location.x - (sizeSc.x),
			transform->location.y - (sizeSc.y)
		),

		Vector2(
			transform->location.x + (sizeSc.x),
			transform->location.y - (sizeSc.y)
		)
	};

	std::vector<Float> projs { 0.0f, 0.0f, 0.0f, 0.0f };

	for (UInt i = 0; i < 4; i++)
	{
		vecs[i].RotateAround(transform->location, transform->rotation);
		projs[i] = vecs[i].GetProjection(axis);
	}

	auto result = std::minmax_element(projs.begin(), projs.end());

	min = *result.first;
	max = *result.second;
}

std::vector<Vector2> RectangleCollider2D::GetAxes() const
{
	Vector2 xAxis = Vector2::Rotate(Vector2::RIGHT, transform->rotation);
	Vector2 yAxis = Vector2::Rotate(Vector2::UP, transform->rotation);

	return std::vector<Vector2>({ xAxis, yAxis });
}

CollisionEvent2D::CollisionEvent2D(ColliderBaseComponent2D* c1, ColliderBaseComponent2D* c2) :
	Event(),
	collider1(c1),
	collider2(c2)
{

}*/