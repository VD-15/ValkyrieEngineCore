#pragma once
#include "ValkyrieEngine/ValkyrieEngine.hpp"

class SampleEvent final
{
	public:
	SampleEvent(vlk::Int _data) :
		data(_data)
	{}

	const  vlk::Int data;
};

class AnotherEvent final
{
	public:
	AnotherEvent(const std::string& _data) :
		data(_data)
	{}

	const std::string data;
};
