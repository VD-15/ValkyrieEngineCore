#include "CameraComponent.h"
#include "../core/Window.h"

using namespace vlk;

namespace
{
	struct CameraResizeSystem : public EventListener<WindowFramebufferEvent>
	{
		void OnEvent(const WindowFramebufferEvent& ev) final override
		{
			CameraComponent2D::ForEach([&ev](CameraComponent2D* c)
			{
				if (c->autoResize)
				{
					c->viewport.x = ev.width / 2.0f;
					c->viewport.y = ev.height / 2.0f;

				}
			});
		}
	};
}

CameraComponent2D* CameraComponent2D::ACTIVE = nullptr;

CameraComponent2D::CameraComponent2D(IEntity* e, TransformComponent2D* transform) :
	Component(e)
{
	static CameraResizeSystem sys;

	this->transform = transform;
	this->autoResize = true;
	this->zoom = Vector2(1.0f);
	this->viewport = Vector2(1.0f);

	if (!CameraComponent2D::ACTIVE)
	{
		CameraComponent2D::ACTIVE = this;
	}
}

void CameraComponent2D::Activate()
{
	CameraComponent2D::ACTIVE = this;
}

Matrix4 CameraComponent2D::GetProjection() const
{
	return CreateOrthographic(	viewport.x *  (zoom.x), 
								-viewport.x * (zoom.x), 
								viewport.y *  (zoom.y), 
								-viewport.y * (zoom.y), 
								-1048576.0f, 
								1048576.0f);
}

Matrix4 CameraComponent2D::GetView() const
{
	return CreateLookAt(Vector3(this->transform->location, 0.0f), Vector3(this->transform->location, -1.0f), Vectors::UnitY3);
}