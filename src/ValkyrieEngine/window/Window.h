#pragma once
#include <string>

#include "ValkyrieEngine/core/ValkyrieEngine.h"
#include "ValkyrieEngine/common/Vector.h"
#include "ValkyrieEngine/core/Event.hpp"

namespace vlk
{
	//The waiting method a Window will use inbetween frames.
	enum class WindowWaitMode
	{
		//Process any events that are in the queue, then return.
		Poll,

		//Wait until an event is present in the queue, then process them.
		Wait,

		//Wait until an event is present in the queue, or until a specified amount of time elapses.
		WaitTimeout
	};

	enum class CursorMode
	{
		//The cursor behaves normally
		Normal,

		//The cursor is invisible when over the window
		Hidden,

		//Hides the cursor and locks it to the window
		Disabled
	};

	#pragma region Callbacks
	//Called when a unicode character is input to the window.
	//Use this for text input.
	struct CharEvent
	{
		const UInt codepoint; 
	};

	//Called whenever the cursor enters or leaves the window.
	struct CursorEnterEvent
	{
		const Int entered;
	};

	//Called whenever the cursor moves.
	struct CursorPosEvent
	{
		const Double x; 
		const Double y; 
	};

	//Called whenever a key is pressed or released.
	//Use this for non-text keyboard input.
	struct KeyEvent
	{
		const Int key; 
		const Int scancode; 
		const Int action; 
		const Int mods; 
	};

	//Called whenever a mouse button is pressed or released.
	//Use this for mouse input.
	struct MouseButtonEvent
	{
		const Int button; 
		const Int action; 
		const Int mods; 
	};

	//Called whenever the mouse is scrolled.
	//Use this for mouse scrolling.
	struct ScrollEvent
	{
		const Double x; 
		const Double y; 
	};

	//Called whenever a file is dropped into the window.
	//Use this for graphical file input.
	struct WindowFileDropEvent
	{
		const Int fileCount; 
		const char* const* const paths; 
	};

	//Called whenever the window's frame buffer is resized.
	//Use this for UI / renderer resizes.
	struct WindowFramebufferEvent
	{
		const Int width; 
		const Int height; 
	};

	//Called when the window is closed.
	struct WindowCloseEvent {};

	//Called when the window becomes focused or unfocused.
	struct WindowFocusEvent
	{
		const Int focused; 
	};

	//Called when the window is iconified or restored.
	struct WindowIconifyEvent
	{
		const Int iconified; 
	};

	//Called when the window is moved.
	struct WindowMoveEvent
	{
		const Int x; 
		const Int y; 
	};

	//Called when the window is refreshed.
	struct WindowRefreshEvent {};

	//Called when the window is resized.
	struct WindowResizeEvent
	{
		const Int width; 
		const Int height; 
	};
	#pragma endregion

	struct WindowArgs
	{
		const std::string title = "ValkyrieEngine Application";
		const Point size = { 640, 480 };
		//Position
		//Monitor
		//Fullscreen
		const WindowWaitMode waitMode = WindowWaitMode::Poll;
		const CursorMode cursorMode = CursorMode::Normal;
		const Double waitTimeout = 1.0;
		const UInt swapInterval = 0;
	};

	class Window :
		public EventListener<ApplicationStartEvent>,
		public EventListener<ApplicationExitEvent>,
		public EventListener<PreUpdateEvent>
	{
		public:
		Window(const WindowArgs& args);
		~Window();

		//Sets the title for the window.
		static void SetTitle(const std::string& title);

		//Sets the width and height of the window, in pixels.
		static void SetSize(Point size);

		static Point GetSize();

		//Sets the swap interval of the window.
		//A value of 1 can be used to implement V-Sync,
		//Values greater than 1 can be used to implement buffered V-Sync,
		//Default value is 0.
		//Maybe don't set this to a negative number...
		static void SetSwapInterval(Int interval);

		//Sets how the window should wait for events.
		static void SetWaitMode(WindowWaitMode mode);

		//Sets how long the window should wait for events when on the WindowWaitMode::WaitTimeout setting
		static void SetWaitTimeout(Double waitTimeout);

		//Sets the cursor mode
		static void SetCursorMode(CursorMode mode);

		//Makes the window visible.
		static void Show();

		//Makes the window invisible.
		static void Hide();

		private:
		void OnEvent(const ApplicationStartEvent& ev) final override;
		void OnEvent(const ApplicationExitEvent& ev) final override;
		void OnEvent(const PreUpdateEvent& ev) final override;
	};
}