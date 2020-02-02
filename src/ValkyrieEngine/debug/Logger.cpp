#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <sstream>

#include "../core/ValkyrieEngine.h"

using namespace vlk;

Logger::Logger()
{
	_logLevel = 1;
	_doSecondaryOutput = false;
}

Logger::~Logger()
{
	if (_output.is_open())
	{
		_output.close();
	}
}

void Logger::Open(std::string path)
{
	_output.open(path, std::ios::out | std::ios::trunc);

	if (!_output.is_open()) throw std::exception("Failed to open log file.");
}

void Logger::Close()
{
	_output.close();
}

void Logger::EnableSecondaryOutput()
{
	_doSecondaryOutput = true;
}

void Logger::DisableSecondaryOutput()
{
	_doSecondaryOutput = false;
}

void Logger::SetLogLevel(UInt level)
{
	_logLevel = level;
}

void Logger::Log(const std::string& source, UInt level, const std::string& message)
{
	if (level >= _logLevel)
	{
		std::stringstream ss;
		ss << std::left << std::setw(48) << source << " [" << vlk::LogLevelToString(level) << "]: " << message << std::endl;

		_output << ss.str();

		if (_doSecondaryOutput) std::cout << ss.str();
	}
}

UInt Logger::GetLogLevel() const
{
	return _logLevel;
}

std::string vlk::LogLevelToString(UInt level)
{
	switch (level)
	{
		case vlk::LOG_LEVEL_TRACE:
			return "TRCE";
		case vlk::LOG_LEVEL_VERBOSE:
			return "VERB";
		case vlk::LOG_LEVEL_INFO:
			return "INFO";
		case vlk::LOG_LEVEL_WARNING:
			return "WARN";
		case vlk::LOG_LEVEL_ERROR:
			return "EROR";
		case vlk::LOG_LEVEL_FATAL:
			return "FATL";
		case vlk::LOG_LEVEL_ANNOUNCE:
			return "ANCE";
		default:
			return "NONE";
	}
}