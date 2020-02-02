#pragma once
#include <fstream>
#include <string>
#include <queue>
#include <tuple>

#include <thread>
#include <condition_variable>

#include "ValkyrieEngine/core/ValkyrieEngine.h"

namespace vlk
{
	constexpr UInt LOG_LEVEL_TRACE = 0;
	constexpr UInt LOG_LEVEL_VERBOSE = 1;
	constexpr UInt LOG_LEVEL_INFO = 2;
	constexpr UInt LOG_LEVEL_WARNING = 3;
	constexpr UInt LOG_LEVEL_ERROR = 4;
	constexpr UInt LOG_LEVEL_FATAL = 5;
	constexpr UInt LOG_LEVEL_ANNOUNCE = 6;

	class Logger
	{
		public:
		Logger();
		virtual ~Logger();

		void EnableSecondaryOutput();
		void DisableSecondaryOutput();

		void Open(std::string path);
		void Close();

		void SetLogLevel(UInt level);
		void Log(const std::string& source, UInt level, const std::string& message);

		UInt GetLogLevel() const;

		private:
		std::ofstream _output;
		UInt _logLevel;
		bool _doSecondaryOutput;
	};

	std::string LogLevelToString(UInt level);
}
