#pragma once

#include "ValkyrieEngine.h"
#include "../utils/Vector.h"

namespace vlk
{
	typedef Int MouseButton;

	namespace MouseButtons
	{
		extern const MouseButton BUTTON_1;
		extern const MouseButton BUTTON_2;
		extern const MouseButton BUTTON_3;
		extern const MouseButton BUTTON_4;
		extern const MouseButton BUTTON_5;
		extern const MouseButton BUTTON_6;
		extern const MouseButton BUTTON_7;
		extern const MouseButton BUTTON_8;

		extern const MouseButton BUTTON_LEFT;
		extern const MouseButton BUTTON_RIGHT;
		extern const MouseButton BUTTON_MIDDLE;

		extern const MouseButton BUTTON_LAST;
	}

	namespace Mouse
	{
		void Init();
		void Destroy();

		Boolean IsButtonDown(MouseButton button);

		Boolean IsButtonPressed(MouseButton button);

		Boolean IsButtonReleased(MouseButton button);

		Vector2 GetPosition();
		Vector2 GetPositionDelta();

		Vector2 GetScroll();
		Vector2 GetScrollDelta();
	}
}