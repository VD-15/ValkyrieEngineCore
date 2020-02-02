#include "UIComponent.h"
#include "../core/Window.h"
#include "../components/CameraComponent.h"

using namespace vlk;

namespace
{
	struct UISystem : public EventListener<WindowFramebufferEvent>, public EventListener<UpdateEvent>
	{
		Vector2 topRight;
		Vector2 bottomLeft;

		UISystem() :
			topRight(),
			bottomLeft()
		{

		}

		void OnEvent(const WindowFramebufferEvent& ev) final override
		{
			this->topRight = Vector2(ev.width / 2.0f, ev.height / 2.0f);
			this->bottomLeft = Vector2(-ev.width / 2.0f, -ev.height / 2.0f);
		}

		void OnEvent(const UpdateEvent& ev)
		{
			UIComponent::ForEach([this](UIComponent* c)
			{
				switch (c->dock)
				{
					case DockType::TopLeft:
						c->transform->location = Vector2(c->offset.x + bottomLeft.x, c->offset.y + topRight.y);
						break;
					case DockType::TopCenter:
						c->transform->location = Vector2(c->offset.x, c->offset.y + topRight.y);
						break;
					case DockType::TopRight:
						c->transform->location = Vector2(c->offset.x + topRight.x, c->offset.y + topRight.y);
						break;
					case DockType::CenterLeft:
						c->transform->location = Vector2(c->offset.x + bottomLeft.x, c->offset.y);
						break;
					case DockType::Center:
						c->transform->location = Vector2(c->offset.x, c->offset.y);
						break;
					case DockType::CenterRight:
						c->transform->location = Vector2(c->offset.x + topRight.x, c->offset.y);
						break;
					case DockType::BottomLeft:
						c->transform->location = Vector2(c->offset.x + bottomLeft.x, c->offset.y + bottomLeft.y);
						break;
					case DockType::BottomCenter:
						c->transform->location = Vector2(c->offset.x, c->offset.y + bottomLeft.y);
						break;
					case DockType::BottomRight:
						c->transform->location = Vector2(c->offset.x + topRight.x, c->offset.y + bottomLeft.y);
						break;
					default:
						break;
				}
			});
		}
	};
}

UIComponent::UIComponent(IEntity* e, TransformComponent2D* transform) :
	Component(e),
	offset()
{
	static UISystem sys;

	this->transform = transform;
	this->dock = DockType::Center;
}