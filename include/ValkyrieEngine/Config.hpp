#pragma once

#include <string>

namespace vlk
{

	#ifdef VLK_ENABLE_TRACE_LOGGING
	constexpr bool enable_trace_logging = true;
	#else
	constexpr bool enable_trace_logging = false;
	#endif

	#ifdef VLK_DEBUG
	constexpr bool is_debug = true;
	#else
	constexpr bool is_debug = false;
	#endif
}
