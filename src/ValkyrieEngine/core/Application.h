#pragma once

#include "ValkyrieEngine.h"

namespace vlk
{
	namespace Application
	{
		//Runs the Application
		void Run();

		//Used to terminate the application.
		//The application may not terminate immediately after this is called.
		void Exit();
	}
}