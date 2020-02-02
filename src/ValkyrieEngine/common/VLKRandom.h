#pragma once

#include "ValkyrieEngine/core/ValkyrieEngine.h"

namespace vlk
{
	namespace VLKRandom
	{
		void Init();
		void Destroy();

		//TODO: simplify these, god damn, what was I thinking!?

		//Gets a random number between 0 and the maximum value of T
		//For floats and doubles, this value is within the range 0..1
		//Valid for:
		//Short
		//UShort
		//Int
		//UInt
		//Long
		//ULong
		//Float
		//Double
		template<typename T>
		T GetRandom();

		//Gets a random number between min and max
		//Valid for:
		//Short
		//UShort
		//Int
		//UInt
		//Long
		//ULong
		//Float
		//Double
		template<typename T>
		T GetRandom(T min, T max);
	}
}