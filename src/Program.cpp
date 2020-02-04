#include "ValkyrieEngine/core/Application.h"

#include "ValkyrieEngine/window/Window.h"
#include "ValkyrieEngine/opengl/GLRenderer.h"

using namespace vlk;

int main()
{
	Window window({});
	GLRenderer glRenderer({});

	Application::Run();
}