#pragma once
#include "ValkyrieEngine/ValkyrieEngine.hpp"

class SampleData final
{
	public:
	SampleData(vlk::Int _data) :
		data(_data)
	{}

	vlk::Int data;
};

class MoreData final
{
	public:
	MoreData(const std::string& _data) :
		data(_data)
	{}

	std::string data;
};
