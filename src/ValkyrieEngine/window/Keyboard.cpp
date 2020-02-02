#include "Keyboard.h"

#include <unordered_set>

#include "Window.h"

using namespace vlk;

#pragma region keys
const Key Keys::UNDEFINED			= -1;
const Key Keys::SPACE				= 32;
const Key Keys::APOSTROPHE			= 39;
const Key Keys::COMMA				= 44;
const Key Keys::MINUS				= 45;
const Key Keys::PERIOD				= 46;
const Key Keys::SLASH				= 47;
const Key Keys::NUM0				= 48;
const Key Keys::NUM1				= 49;
const Key Keys::NUM2				= 50;
const Key Keys::NUM3				= 51;
const Key Keys::NUM4				= 52;
const Key Keys::NUM5				= 53;
const Key Keys::NUM6				= 54;
const Key Keys::NUM7				= 55;
const Key Keys::NUM8				= 56;
const Key Keys::NUM9				= 57;
const Key Keys::SEMICOLON			= 59;
const Key Keys::EQUAL				= 61;
const Key Keys::A					= 65;
const Key Keys::B					= 66;
const Key Keys::C					= 67;
const Key Keys::D					= 68;
const Key Keys::E					= 69;
const Key Keys::F					= 70;
const Key Keys::G					= 71;
const Key Keys::H					= 72;
const Key Keys::I					= 73;
const Key Keys::J					= 74;
const Key Keys::K					= 75;
const Key Keys::L					= 76;
const Key Keys::M					= 77;
const Key Keys::N					= 78;
const Key Keys::O					= 79;
const Key Keys::P					= 80;
const Key Keys::Q					= 81;
const Key Keys::R					= 82;
const Key Keys::S					= 83;
const Key Keys::T					= 84;
const Key Keys::U					= 85;
const Key Keys::V					= 86;
const Key Keys::W					= 87;
const Key Keys::X					= 88;
const Key Keys::Y					= 89;
const Key Keys::Z					= 90;
const Key Keys::LEFT_BRACKET		= 91;
const Key Keys::BACKSLASH			= 92;
const Key Keys::RIGHT_BRACKET		= 93;
const Key Keys::GRAVE_ACCENT		= 96;
const Key Keys::WORLD_1				= 16;
const Key Keys::WORLD_2				= 16;

const Key Keys::ESCAPE				= 256;
const Key Keys::ENTER				= 257;
const Key Keys::TAB					= 258;
const Key Keys::BACKSPACE			= 259;
const Key Keys::INSERT				= 260;
const Key Keys::DELETE				= 261;
const Key Keys::RIGHT				= 262;
const Key Keys::LEFT				= 263;
const Key Keys::DOWN				= 264;
const Key Keys::UP					= 265;
const Key Keys::PAGE_UP				= 266;
const Key Keys::PAGE_DOWN			= 267;
const Key Keys::HOME				= 268;
const Key Keys::END					= 269;
const Key Keys::CAPS_LOCK			= 280;
const Key Keys::SCROLL_LOCK			= 281;
const Key Keys::NUM_LOCK			= 282;
const Key Keys::PRINT_SCREEN		= 283;
const Key Keys::PAUSE				= 284;
const Key Keys::F1					= 290;
const Key Keys::F2					= 291;
const Key Keys::F3					= 292;
const Key Keys::F4					= 293;
const Key Keys::F5					= 294;
const Key Keys::F6					= 295;
const Key Keys::F7					= 296;
const Key Keys::F8					= 297;
const Key Keys::F9					= 298;
const Key Keys::F10					= 299;
const Key Keys::F11					= 300;
const Key Keys::F12					= 301;
const Key Keys::F13					= 302;
const Key Keys::F14					= 303;
const Key Keys::F15					= 304;
const Key Keys::F16					= 305;
const Key Keys::F17					= 306;
const Key Keys::F18					= 307;
const Key Keys::F19					= 308;
const Key Keys::F20					= 309;
const Key Keys::F21					= 310;
const Key Keys::F22					= 311;
const Key Keys::F23					= 312;
const Key Keys::F24					= 313;
const Key Keys::F25					= 314;
const Key Keys::NUMPAD_0			= 320;
const Key Keys::NUMPAD_1			= 321;
const Key Keys::NUMPAD_2			= 322;
const Key Keys::NUMPAD_3			= 323;
const Key Keys::NUMPAD_4			= 324;
const Key Keys::NUMPAD_5			= 325;
const Key Keys::NUMPAD_6			= 326;
const Key Keys::NUMPAD_7			= 327;
const Key Keys::NUMPAD_8			= 328;
const Key Keys::NUMPAD_9			= 329;
const Key Keys::NUMPAD_DECIMAL		= 330;
const Key Keys::NUMPAD_DIVIDE		= 331;
const Key Keys::NUMPAD_MULTIPLY		= 332;
const Key Keys::NUMPAD_SUBTRACT		= 333;
const Key Keys::NUMPAD_ADD			= 334;
const Key Keys::NUMPAD_ENTER		= 335;
const Key Keys::NUMPAD_EQUAL		= 336;
const Key Keys::LEFT_SHIFT			= 340;
const Key Keys::LEFT_CONTROL		= 341;
const Key Keys::LEFT_ALT			= 342;
const Key Keys::LEFT_START			= 343;
const Key Keys::RIGHT_SHIFT			= 344;
const Key Keys::RIGHT_CONTROL		= 345;
const Key Keys::RIGHT_ALT			= 346;
const Key Keys::RIGHT_START			= 347;
const Key Keys::MENU				= 348;

const Key Keys::LAST = Keys::MENU;
#pragma endregion

namespace
{
	std::string input;

	//Keys pressed this frame
	std::unordered_set<Key> pressedKeys;

	//keys released this frame
	std::unordered_set<Key> releasedKeys;

	//Keys currently down
	std::unordered_set<Key> downKeys;

	struct KeyboardSystem : public EventListener<CharEvent>, 
							public EventListener<KeyEvent>,
							public EventListener<PostUpdateEvent>
	{
		void OnEvent(const CharEvent& ev) final override
		{
			input += (Byte)ev.codepoint;
		}

		void OnEvent(const KeyEvent& ev) final override
		{
			switch (ev.action)
			{
				case 0:	//Release
					releasedKeys.insert(ev.key);
					downKeys.erase(ev.key);
					LogVerbose("Keyboard", "Key released: " + std::to_string(ev.key));
					break;
				case 1:	//Press
					pressedKeys.insert(ev.key);
					downKeys.insert(ev.key);
					LogVerbose("Keyboard", "Key pressed: " + std::to_string(ev.key));
					break;
				default:
					return;
			}
		}

		void OnEvent (const PostUpdateEvent& ev) final override
		{
			LogTrace("Keyboard", "Clearing Input.");

			input.clear();
			pressedKeys.clear();
			releasedKeys.clear();
		}
	};

	KeyboardSystem* sys;
}

void Keyboard::Init()
{
	LogInfo("Keyboard", "Initializing.");

	input.clear();
	pressedKeys.clear();
	releasedKeys.clear();
	downKeys.clear();

	sys = new KeyboardSystem();
}

void Keyboard::Destroy()
{
	LogInfo("Keyboard", "Destroying.");

	input.clear();
	pressedKeys.clear();
	releasedKeys.clear();
	downKeys.clear();

	delete sys;
}

Boolean Keyboard::IsKeyDown(Key key)
{
	return downKeys.find(key) != downKeys.end();
}

Boolean Keyboard::IsKeyPressed(Key key)
{
	return pressedKeys.find(key) != pressedKeys.end();
}

Boolean Keyboard::IsKeyReleased(Key key)
{
	return releasedKeys.find(key) != releasedKeys.end();
}

std::string Keyboard::GetTextInput()
{
	return input;
}