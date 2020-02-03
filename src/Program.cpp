#include "ValkyrieEngine/core/Application.h"
#include "ValkyrieEngine/core/GameComponent.hpp"
#include "ValkyrieEngine/core/GameEntity.hpp"
#include "ValkyrieEngine/core/Event.hpp"

#include "ValkyrieEngine/window/Window.h"

using namespace vlk;

int main()
{
	Window window(WindowArgs {});

	vlk::Init();

	vlk::Application::Run();

	vlk::Destroy();
}