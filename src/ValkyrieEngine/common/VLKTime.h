#pragma once

#include "../core/Module.hpp"
#include "../core/Event.hpp"

namespace vlk
{
	struct VLKTimeArgs
	{
		//Specifies whether to use a fixed timestep or varying timestep.
		Boolean isFixedTimestep = false;

		//Specifies the number of nanoseconds for the fixed timestep.
		//Has no effect unless isFixedTimestep is set to true.
		ULong fixedTimestep = 0;
	};

	class VLKTime
	{
		public:
		VLKTime(const VLKTimeArgs& a);
		~VLKTime();

		VLKTime() = delete;
		VLKTime(const VLKTime&) = delete;
		VLKTime(VLKTime&&) = delete;
		VLKTime& operator=(const VLKTime&) = delete;
		VLKTime& operator=(VLKTime&&) = delete;

		//Time, in seconds, since last game tick was completed.
		//Valid template arguments are: Float, Double
		template <class T = Double>
		static T DeltaTime();

		private:
		const VLKTimeArgs args;
	};
}