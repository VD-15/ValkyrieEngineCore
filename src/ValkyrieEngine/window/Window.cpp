#include "Window.h"

#include "GLFW/glfw3.h"

#include "Application.h"

#include <vector>

using namespace vlk;

namespace
{
	GLFWwindow* WINDOW_INSTANCE = nullptr;
	WindowWaitMode WINDOW_WAIT_MODE = WindowWaitMode::Poll;
	Double WINDOW_WAIT_TIMEOUT = 1.0;

	void CHAR_CALLBACK(GLFWwindow* window, UInt codepoint)
	{
		LogTrace("Window", "Char callback");

		EventBus<CharEvent>::Send(CharEvent { codepoint });
	}

	void CURSOR_ENTER_CALLBACK(GLFWwindow* window, Int entered)
	{
		LogTrace("Window", "Cursor enter callback");

		EventBus<CursorEnterEvent>::Send(CursorEnterEvent { entered });
	}

	void CURSOR_POS_CALLBACK(GLFWwindow* window, Double x, Double y)
	{
		LogTrace("Window", "Cursor pos callback");

		EventBus<CursorPosEvent>::Send(CursorPosEvent { x, y });
	}

	void FILE_DROP_CALLBACK(GLFWwindow* window, Int fileCount, const char** paths)
	{
		LogTrace("Window", "File drop callback");

		EventBus<WindowFileDropEvent>::Send(WindowFileDropEvent { fileCount, paths });
	}

	void FRAMEBUFFER_CALLBACK(GLFWwindow* window, Int width, Int height)
	{
		LogTrace("Window", "Framebuffer callback");

		EventBus<WindowFramebufferEvent>::Send(WindowFramebufferEvent { width, height });
	}

	void KEY_CALLBACK(GLFWwindow* window, Int key, Int scancode, Int action, Int mods)
	{
		LogTrace("Window", "Key callback");

		EventBus<KeyEvent>::Send(KeyEvent { key, scancode, action, mods });
	}

	void MOUSE_BUTTON_CALLBACK(GLFWwindow* window, Int button, Int action, Int mods)
	{
		LogTrace("Window", "Mouse button callback");

		EventBus<MouseButtonEvent>::Send(MouseButtonEvent { button, action, mods });
	}

	void SCROLL_CALLBACK(GLFWwindow* window, Double x, Double y)
	{
		LogTrace("Window", "Scroll callback");

		EventBus<ScrollEvent>::Send(ScrollEvent { x, y });
	}

	void CLOSE_CALLBACK(GLFWwindow* window)
	{
		LogTrace("Window", "Close callback");

		EventBus<WindowCloseEvent>::Send();
	}

	void FOCUS_CALLBACK(GLFWwindow* window, Int focused)
	{
		LogTrace("Window", "Focus callback");

		EventBus<WindowFocusEvent>::Send(WindowFocusEvent { focused });
	}

	void ICONIFY_CALLBACK(GLFWwindow* window, Int iconified)
	{
		LogTrace("Window", "Iconify callback");

		EventBus<WindowIconifyEvent>::Send(WindowIconifyEvent { iconified });
	}

	void MOVE_CALLBACK(GLFWwindow* window, Int x, Int y)
	{
		LogTrace("Window", "Move callback");

		EventBus<WindowMoveEvent>::Send(WindowMoveEvent { x, y });
	}

	void REFRESH_CALLBACK(GLFWwindow* window)
	{
		LogTrace("Window", "Refresh callback");

		EventBus<WindowRefreshEvent>::Send();
	}

	void RESIZE_CALLBACK(GLFWwindow* window, Int width, Int height)
	{
		LogTrace("Window", "Resize callback");

		EventBus<WindowResizeEvent>::Send(WindowResizeEvent { width, height });
	}
	
	void GLFW_ERROR_CALLBACK(Int code, const char* message)
	{
		LogError("GLFW", std::to_string(code) + std::string(message));
	}

	struct WindowSystem :
		public EventListener<ApplicationStartEvent>,
		public EventListener<ApplicationExitEvent>,
		public EventListener<PreUpdateEvent>
	{

		void OnEvent(const ApplicationStartEvent& ev) final override
		{
			Window::Show();
		}

		void OnEvent(const ApplicationExitEvent& ev) final override
		{
			Window::Hide();
		}

		void OnEvent(const PreUpdateEvent& ev) final override
		{
			LogTrace("Window", "Processing events.");

			glfwSwapBuffers(WINDOW_INSTANCE);

			switch (WINDOW_WAIT_MODE)
			{
				case WindowWaitMode::Poll:
					glfwPollEvents();
					break;
				case WindowWaitMode::Wait:
					glfwWaitEvents();
					break;
				case WindowWaitMode::WaitTimeout:
					glfwWaitEventsTimeout(WINDOW_WAIT_TIMEOUT);
					break;
				default:
					glfwPollEvents();
					return;
			}

			if (glfwWindowShouldClose(WINDOW_INSTANCE))
			{
				vlk::Application::Exit();
			}
		}

	};

	WindowSystem* sys;
}

void Window::Init()
{
	LogInfo("Window", "Initializing.");

	if (WINDOW_INSTANCE)
	{

	}

	glfwSetErrorCallback(GLFW_ERROR_CALLBACK);

	Int glfwError = glfwInit();

	if (!glfwError)
	{
		LogError("Window", "GLFW failed to initialize: " + std::to_string(glfwError));
		throw std::exception("GLFW failed to initialize.", glfwError);
	}

	WINDOW_WAIT_MODE = WindowWaitMode::Poll;
	WINDOW_WAIT_TIMEOUT = 1.0;
	
	//TODO: expose this somewhere
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	#endif

	WINDOW_INSTANCE = glfwCreateWindow(640, 480, "ValkyrieEngine Application", nullptr, nullptr);

	if (!WINDOW_INSTANCE)
	{
		LogError("Window", "Failed to create a window.");
	}

	glfwSetCharCallback(WINDOW_INSTANCE, CHAR_CALLBACK);
	glfwSetCursorEnterCallback(WINDOW_INSTANCE, CURSOR_ENTER_CALLBACK);
	glfwSetCursorPosCallback(WINDOW_INSTANCE, CURSOR_POS_CALLBACK);
	glfwSetDropCallback(WINDOW_INSTANCE, FILE_DROP_CALLBACK);
	glfwSetFramebufferSizeCallback(WINDOW_INSTANCE, FRAMEBUFFER_CALLBACK);
	glfwSetKeyCallback(WINDOW_INSTANCE, KEY_CALLBACK);
	glfwSetMouseButtonCallback(WINDOW_INSTANCE, MOUSE_BUTTON_CALLBACK);
	glfwSetScrollCallback(WINDOW_INSTANCE, SCROLL_CALLBACK);
	glfwSetWindowCloseCallback(WINDOW_INSTANCE, CLOSE_CALLBACK);
	glfwSetWindowFocusCallback(WINDOW_INSTANCE, FOCUS_CALLBACK);
	glfwSetWindowIconifyCallback(WINDOW_INSTANCE, ICONIFY_CALLBACK);
	glfwSetWindowPosCallback(WINDOW_INSTANCE, MOVE_CALLBACK);
	glfwSetWindowRefreshCallback(WINDOW_INSTANCE, REFRESH_CALLBACK);
	glfwSetWindowSizeCallback(WINDOW_INSTANCE, RESIZE_CALLBACK);

	sys = new WindowSystem();

	glfwMakeContextCurrent(WINDOW_INSTANCE);
	glfwSwapInterval(0);
}

void Window::Destroy()
{
	LogInfo("Window", "Destroying.");

	glfwDestroyWindow(WINDOW_INSTANCE);

	glfwTerminate();
	
	WINDOW_INSTANCE = nullptr;

	delete sys;
}

void Window::SetTitle(const std::string& title)
{
	LogInfo("Window", "Set window title to {" + title + "}");
	glfwSetWindowTitle(WINDOW_INSTANCE, title.c_str());
}

void Window::SetWidth(Int width)
{
	LogInfo("Window", "Set window width to: " + std::to_string(width));
	Int height = 0;

	glfwGetWindowSize(WINDOW_INSTANCE, nullptr, &height);
	glfwSetWindowSize(WINDOW_INSTANCE, width, height);
}

void Window::SetHeight(Int height)
{
	LogInfo("Window", "Set window height to: " + std::to_string(height));
	Int width = 0;

	glfwGetWindowSize(WINDOW_INSTANCE, &width, nullptr);
	glfwSetWindowSize(WINDOW_INSTANCE, width, height);
}

void Window::SetSize(Int width, Int height)
{
	LogInfo("Window", "Set window size to: " + std::to_string(width) + ", " + std::to_string(height));
	glfwSetWindowSize(WINDOW_INSTANCE, width, height);
}

void Window::SetSize(Point size)
{
	LogInfo("Window", "Set window size to: " + std::to_string(size.x) + ", " + std::to_string(size.y));
	glfwSetWindowSize(WINDOW_INSTANCE, size.x, size.y);
}

void Window::SetSwapInterval(Int interval)
{
	LogInfo("Window", "Set window swap interval to: " + std::to_string(interval));
	glfwSwapInterval(interval);
}

void Window::SetWaitMode(WindowWaitMode mode)
{
	WINDOW_WAIT_MODE = mode;
}

void Window::SetWaitTimeout(Double timeout)
{
	WINDOW_WAIT_TIMEOUT = timeout;
}

void Window::ShowCursor()
{
	glfwSetInputMode(WINDOW_INSTANCE, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::HideCursor()
{
	glfwSetInputMode(WINDOW_INSTANCE, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Window::DisableCursor()
{
	glfwSetInputMode(WINDOW_INSTANCE, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::Show()
{
	LogInfo("Window", "Showing window.");
	glfwShowWindow(WINDOW_INSTANCE);
	glfwRequestWindowAttention(WINDOW_INSTANCE);
}

void Window::Hide()
{
	LogInfo("Window", "Hiding window.");
	glfwHideWindow(WINDOW_INSTANCE);
}

Int Window::GetWidth()
{
	Int width = 0;

	glfwGetWindowSize(WINDOW_INSTANCE, &width, nullptr);

	return width;
}

Int Window::GetHeight()
{
	Int height = 0;

	glfwGetWindowSize(WINDOW_INSTANCE, nullptr, &height);

	return height;
}

Point Window::GetSize()
{
	Point size;

	glfwGetWindowSize(WINDOW_INSTANCE, &size.x, &size.y);

	return size;
}
