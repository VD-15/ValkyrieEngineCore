#pragma once

#include <functional>

#include "../core/ValkyrieEngine.h"

namespace vlk
{
	template <typename T>
	class IAllocator
	{
		public:
		IAllocator()
		{
			typedef T::Type Type;

			Type* (*Test_AllocNew) (...) = T::AllocateNew;
			void  (*Test_ForEach)  (const std::function<void(Type*)>&) = T::ForEach;
			ULong (*Test_GetCount) () = T::GetCount();
			Bool  (*Test_HasAllocations) () = T::HasAllocations;
			void  (*Test_Delete) (Type*) = T::Delete;
		}
	};
}