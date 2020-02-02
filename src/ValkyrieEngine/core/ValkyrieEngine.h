#pragma once
#include <string>
#include <typeinfo>

namespace vlk
{
	typedef uint8_t		Byte;
	typedef uint8_t		Char;
	typedef int8_t		SByte;
	typedef uint16_t	UShort;
	typedef int16_t		Short;
	typedef uint32_t	UInt;
	typedef int32_t		Int;
	typedef uint64_t	ULong;
	typedef int64_t		Long;

	typedef bool		Boolean;
	typedef bool		Bool;

	typedef double		Double;
	typedef float		Single;
	typedef float		Float;

	typedef size_t		Size;

	constexpr Float PiOverTwo =	1.57079632679f;
	constexpr Float Pi =		3.14159265359f;
	constexpr Float TwoPi =		6.28318530718f;
	
	//TODO: Remove calls to logging functions from core module
	//TODO: Move logging functions to seperate module

	typedef void LogFun(const std::string& source, const std::string& message);

	//Logs a mesasage with trace severity.
	//Used to map flow within a function or algorithm.
	LogFun LogTrace;

	//Logs a mesasage with verbose severity.
	//Used to map flow between functions and routines.
	LogFun LogVerbose;

	//Logs a mesasage with info severity.
	//Used to map overall program flow and important routines.
	LogFun LogInfo;

	//Logs a mesasage with warning severity.
	//Used to warn when something could lead to an error.
	LogFun LogWarning;

	//Logs a mesasage with error severity.
	//Used when an error has occured that the program can recover from.
	LogFun LogError;

	//Logs a mesasage with fatal severity.
	//Used when an error has occured that the program can not recover from.
	LogFun LogFatal;

	//Logs a mesasage with announce severity.
	LogFun LogAnnounce;

	//TODO: We shouldn't need these

	//Initializes ValkyrieEngine and it's dependencies
	void Init();

	//Destroys ValkyrieEngine and it's dependencies
	void Destroy();

	//TODO: once C++20 is finalized, start putting things into actual CPP modules
	//TODO: modularize VLKRandom
	//TODO: modularize window & renderer
	//TODO: move everything below here to a utility header

	//Converts a pointer to a hexadecimal string
	std::string PointerToString(const void* ptr);

	//Converts an std::type_info to a string
	std::string TypeInfoToString(const std::type_info& t);

	//Check if a string ends with another string
	Boolean StringEndsWith(const std::string& str, const std::string& end);

	//Check if a string begins with another string
	Boolean StringBeginsWith(const std::string& str, const std::string& beg);

	//Converts a type to a string
	template<class T>
	constexpr std::string TypeToString()
	{
		return TypeInfoToString(typeid(T));
	}
}