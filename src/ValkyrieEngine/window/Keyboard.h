#pragma once

#include "ValkyrieEngine.h"

namespace vlk
{
	//Typedef for all named keys
	typedef Int Key;

	namespace Keys
	{
		extern const Key UNDEFINED;
		extern const Key SPACE;
		extern const Key APOSTROPHE;
		extern const Key COMMA;
		extern const Key MINUS;
		extern const Key PERIOD;
		extern const Key SLASH;
		extern const Key NUM0;
		extern const Key NUM1;
		extern const Key NUM2;
		extern const Key NUM3;
		extern const Key NUM4;
		extern const Key NUM5;
		extern const Key NUM6;
		extern const Key NUM7;
		extern const Key NUM8;
		extern const Key NUM9;
		extern const Key SEMICOLON;
		extern const Key EQUAL;
		extern const Key A;
		extern const Key B;
		extern const Key C;
		extern const Key D;
		extern const Key E;
		extern const Key F;
		extern const Key G;
		extern const Key H;
		extern const Key I;
		extern const Key J;
		extern const Key K;
		extern const Key L;
		extern const Key M;
		extern const Key N;
		extern const Key O;
		extern const Key P;
		extern const Key Q;
		extern const Key R;
		extern const Key S;
		extern const Key T;
		extern const Key U;
		extern const Key V;
		extern const Key W;
		extern const Key X;
		extern const Key Y;
		extern const Key Z;
		extern const Key LEFT_BRACKET;
		extern const Key BACKSLASH;
		extern const Key RIGHT_BRACKET;
		extern const Key GRAVE_ACCENT;
		extern const Key WORLD_1;
		extern const Key WORLD_2;

		extern const Key ESCAPE;
		extern const Key ENTER;
		extern const Key TAB;
		extern const Key BACKSPACE;
		extern const Key INSERT;
		extern const Key DELETE;
		extern const Key RIGHT;
		extern const Key LEFT;
		extern const Key DOWN;
		extern const Key UP;
		extern const Key PAGE_UP;
		extern const Key PAGE_DOWN;
		extern const Key HOME;
		extern const Key END;
		extern const Key CAPS_LOCK;
		extern const Key SCROLL_LOCK;
		extern const Key NUM_LOCK;
		extern const Key PRINT_SCREEN;
		extern const Key PAUSE;
		extern const Key F1;
		extern const Key F2;
		extern const Key F3;
		extern const Key F4;
		extern const Key F5;
		extern const Key F6;
		extern const Key F7;
		extern const Key F8;
		extern const Key F9;
		extern const Key F10;
		extern const Key F11;
		extern const Key F12;
		extern const Key F13;
		extern const Key F14;
		extern const Key F15;
		extern const Key F16;
		extern const Key F17;
		extern const Key F18;
		extern const Key F19;
		extern const Key F20;
		extern const Key F21;
		extern const Key F22;
		extern const Key F23;
		extern const Key F24;
		extern const Key F25;
		extern const Key NUMPAD_0;
		extern const Key NUMPAD_1;
		extern const Key NUMPAD_2;
		extern const Key NUMPAD_3;
		extern const Key NUMPAD_4;
		extern const Key NUMPAD_5;
		extern const Key NUMPAD_6;
		extern const Key NUMPAD_7;
		extern const Key NUMPAD_8;
		extern const Key NUMPAD_9;
		extern const Key NUMPAD_DECIMAL;
		extern const Key NUMPAD_DIVIDE;
		extern const Key NUMPAD_MULTIPLY;
		extern const Key NUMPAD_SUBTRACT;
		extern const Key NUMPAD_ADD;
		extern const Key NUMPAD_ENTER;
		extern const Key NUMPAD_EQUAL;
		extern const Key LEFT_SHIFT;
		extern const Key LEFT_CONTROL;
		extern const Key LEFT_ALT;
		extern const Key LEFT_START;
		extern const Key RIGHT_SHIFT;
		extern const Key RIGHT_CONTROL;
		extern const Key RIGHT_ALT;
		extern const Key RIGHT_START;
		extern const Key MENU;

		extern const Key LAST;
	}

	namespace Keyboard
	{
		//Initializes the keyboard
		void Init();

		//Destroys the keyboard
		void Destroy();

		//Is the key currently down?
		Boolean IsKeyDown(Key key);

		//Was the key pressed this frame?
		Boolean IsKeyPressed(Key key);

		//Was the key released this frame?
		Boolean IsKeyReleased(Key key);

		//Gets any text that was typed to the window this frame
		std::string GetTextInput();
	}
}