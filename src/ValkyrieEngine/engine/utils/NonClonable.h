#pragma once

#include "../core/ValkyrieEngine.h"

namespace vlk
{
	class NonClonable
	{
		public:
		NonClonable() = default;

		NonClonable(const NonClonable&) = delete;
		NonClonable(NonClonable&&) = delete;
		NonClonable& operator=(const NonClonable&) = delete;
	};
}