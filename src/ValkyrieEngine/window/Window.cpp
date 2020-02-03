#include "Window.h"
#include "ValkyrieEngine/core/Application.h"

#include "GLFW/glfw3.h"

#include <vector>
#include <iostream>

using namespace vlk;

namespace
{
	GLFWwindow* WINDOW_INSTANCE = nullptr;
	WindowWaitMode WINDOW_WAIT_MODE = WindowWaitMode::Poll;
	Double WINDOW_WAIT_TIMEOUT = 1.0;

	void CHAR_CALLBACK(GLFWwindow* window, UInt codepoint)
	{
		EventBus<CharEvent>::Send(CharEvent { codepoint });
	}

	void CURSOR_ENTER_CALLBACK(GLFWwindow* window, Int entered)
	{
		EventBus<CursorEnterEvent>::Send(CursorEnterEvent { entered });
	}

	void CURSOR_POS_CALLBACK(GLFWwindow* window, Double x, Double y)
	{
		EventBus<CursorPosEvent>::Send(CursorPosEvent { x, y });
	}

	void FILE_DROP_CALLBACK(GLFWwindow* window, Int fileCount, const char** paths)
	{
		EventBus<WindowFileDropEvent>::Send(WindowFileDropEvent { fileCount, paths });
	}

	void FRAMEBUFFER_CALLBACK(GLFWwindow* window, Int width, Int height)
	{
		EventBus<WindowFramebufferEvent>::Send(WindowFramebufferEvent { width, height });
	}

	void KEY_CALLBACK(GLFWwindow* window, Int key, Int scancode, Int action, Int mods)
	{
		EventBus<KeyEvent>::Send(KeyEvent { key, scancode, action, mods });
	}

	void MOUSE_BUTTON_CALLBACK(GLFWwindow* window, Int button, Int action, Int mods)
	{
		EventBus<MouseButtonEvent>::Send(MouseButtonEvent { button, action, mods });
	}

	void SCROLL_CALLBACK(GLFWwindow* window, Double x, Double y)
	{
		EventBus<ScrollEvent>::Send(ScrollEvent { x, y });
	}

	void CLOSE_CALLBACK(GLFWwindow* window)
	{
		EventBus<WindowCloseEvent>::Send();
	}

	void FOCUS_CALLBACK(GLFWwindow* window, Int focused)
	{
		EventBus<WindowFocusEvent>::Send(WindowFocusEvent { focused });
	}

	void ICONIFY_CALLBACK(GLFWwindow* window, Int iconified)
	{
		EventBus<WindowIconifyEvent>::Send(WindowIconifyEvent { iconified });
	}

	void MOVE_CALLBACK(GLFWwindow* window, Int x, Int y)
	{
		EventBus<WindowMoveEvent>::Send(WindowMoveEvent { x, y });
	}

	void REFRESH_CALLBACK(GLFWwindow* window)
	{
		EventBus<WindowRefreshEvent>::Send();
	}

	void RESIZE_CALLBACK(GLFWwindow* window, Int width, Int height)
	{
		EventBus<WindowResizeEvent>::Send(WindowResizeEvent { width, height });
	}
	
	void GLFW_ERROR_CALLBACK(Int code, const char* message)
	{
		std::cout << "GLFW Error: " << std::to_string(code) + std::string(message) << std::endl;
	}
}

Window::Window(const WindowArgs& args)
{
	if (WINDOW_INSTANCE)
	{
		throw std::exception("Window already initialized!");
	}

	glfwSetErrorCallback(GLFW_ERROR_CALLBACK);

	Int glfwError = glfwInit();

	if (!glfwError)
	{
		throw std::exception("GLFW failed to initialize.", glfwError);
	}

	WINDOW_WAIT_MODE = args.waitMode;
	WINDOW_WAIT_TIMEOUT = args.waitTimeout;
	
	//TODO: expose this somewhere
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	SetCursorMode(args.cursorMode);

	#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	#endif

	WINDOW_INSTANCE = glfwCreateWindow(args.size.x, args.size.y, args.title.c_str(), nullptr, nullptr);

	if (!WINDOW_INSTANCE)
	{
		throw std::exception("Failed to create a window.");
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

	glfwMakeContextCurrent(WINDOW_INSTANCE);
	glfwSwapInterval(args.swapInterval);
}

Window::~Window()
{
	glfwDestroyWindow(WINDOW_INSTANCE);

	glfwTerminate();
	
	WINDOW_INSTANCE = nullptr;
}

void Window::SetTitle(const std::string& title)
{
	glfwSetWindowTitle(WINDOW_INSTANCE, title.c_str());
}

void Window::SetSize(Point size)
{
	glfwSetWindowSize(WINDOW_INSTANCE, size.x, size.y);
}

void Window::SetSwapInterval(Int interval)
{
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

void Window::SetCursorMode(CursorMode mode)
{
	switch (mode)
	{
		case CursorMode::Normal:
			glfwSetInputMode(WINDOW_INSTANCE, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			break;
		case CursorMode::Hidden:
			glfwSetInputMode(WINDOW_INSTANCE, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			break;
		case CursorMode::Disabled:
			glfwSetInputMode(WINDOW_INSTANCE, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			break;
		default:
			break;
	}
}

void Window::Show()
{
	glfwShowWindow(WINDOW_INSTANCE);
	glfwRequestWindowAttention(WINDOW_INSTANCE);
}

void Window::Hide()
{
	glfwHideWindow(WINDOW_INSTANCE);
}

Point Window::GetSize()
{
	Point size;

	glfwGetWindowSize(WINDOW_INSTANCE, &size.x, &size.y);

	return size;
}

void Window::OnEvent(const ApplicationStartEvent& ev)
{
	Window::Show();
}

void Window::OnEvent(const ApplicationExitEvent& ev)
{
	Window::Hide();
}

void Window::OnEvent(const PreUpdateEvent& ev)
{
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