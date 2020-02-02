#include "Mouse.h"
#include "Window.h"

#include <unordered_set>

using namespace vlk;

const MouseButton MouseButtons::BUTTON_1 = 0;
const MouseButton MouseButtons::BUTTON_2 = 1;
const MouseButton MouseButtons::BUTTON_3 = 2;
const MouseButton MouseButtons::BUTTON_4 = 3;
const MouseButton MouseButtons::BUTTON_5 = 4;
const MouseButton MouseButtons::BUTTON_6 = 5;
const MouseButton MouseButtons::BUTTON_7 = 6;
const MouseButton MouseButtons::BUTTON_8 = 7;

const MouseButton MouseButtons::BUTTON_LEFT = 0;
const MouseButton MouseButtons::BUTTON_RIGHT = 1;
const MouseButton MouseButtons::BUTTON_MIDDLE = 2;

const MouseButton MouseButtons::BUTTON_LAST = 7;

namespace
{
	std::unordered_set<MouseButton> downButtons;
	std::unordered_set<MouseButton> pressedButtons;
	std::unordered_set<MouseButton> releasedButtons;

	Vector2 position;
	Vector2 oldPosition;

	Vector2 scroll;
	Vector2 oldScroll;

	struct MoyseSystem : 
		public EventListener<CursorPosEvent>,
		public EventListener<MouseButtonEvent>,
		public EventListener<ScrollEvent>,
		public EventListener<PostUpdateEvent>
	{
		void OnEvent(const CursorPosEvent& ev) final override
		{
			position.x = static_cast<Float>(ev.x);
			position.y = static_cast<Float>(ev.y);
		}

		void OnEvent(const MouseButtonEvent& ev) final override
		{
			switch (ev.action)
			{
				case 0: //Release
					releasedButtons.insert(ev.button);
					downButtons.erase(ev.button);
					LogVerbose("Mouse", "Button released: " + std::to_string(ev.button));
					break;
				case 1: //Press
					pressedButtons.insert(ev.button);
					downButtons.insert(ev.button);
					LogVerbose("Mouse", "Button pressed: " + std::to_string(ev.button));
					break;
				default:
					return;
			}
		}

		void OnEvent(const ScrollEvent& ev) final override
		{
			scroll.x += static_cast<Float>(ev.x);
			scroll.y += static_cast<Float>(ev.y);
		}

		void OnEvent(const PostUpdateEvent& ev) final override
		{
			LogTrace("Mouse", "Clearing Input.");

			pressedButtons.clear();
			releasedButtons.clear();

			oldPosition = position;
			oldScroll = scroll;
		}
	};

	MoyseSystem* sys;
}

void Mouse::Init()
{
	LogInfo("Mouse", "Initializing");

	downButtons.clear();
	pressedButtons.clear();
	releasedButtons.clear();

	position = Vector2();
	oldPosition = Vector2();

	scroll = Vector2();
	oldScroll = Vector2();

	sys = new MoyseSystem();
}

void Mouse::Destroy()
{
	LogInfo("Mouse", "Destroying.");

	downButtons.clear();
	pressedButtons.clear();
	releasedButtons.clear();

	position = Vector2();
	oldPosition = Vector2();

	scroll = Vector2();
	oldScroll = Vector2();

	delete sys;
}

Boolean Mouse::IsButtonDown(MouseButton button)
{
	return downButtons.find(button) != downButtons.end();
}

Boolean Mouse::IsButtonPressed(MouseButton button)
{
	return pressedButtons.find(button) != pressedButtons.end();
}

Boolean Mouse::IsButtonReleased(MouseButton button)
{
	return releasedButtons.find(button) != releasedButtons.end();
}

Vector2 Mouse::GetPosition()
{
	return position;
}

Vector2 Mouse::GetPositionDelta()
{
	return position - oldPosition;
}

Vector2 Mouse::GetScroll()
{
	return scroll;
}

Vector2 Mouse::GetScrollDelta()
{
	return scroll - oldScroll;
}