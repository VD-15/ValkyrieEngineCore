#include "VLKRandom.h"
#include <random>
#include <ctime>
#include <cassert>

using namespace vlk;

namespace
{
	std::mt19937 RNG;
}

void VLKRandom::Init()
{
	RNG.seed(static_cast<UInt>(std::time(NULL)));
}

void VLKRandom::Destroy()
{

}

template<>
Short VLKRandom::GetRandom()
{
	static std::uniform_int_distribution<Short> dist;
	return dist(RNG);
}

template<>
UShort VLKRandom::GetRandom()
{
	static std::uniform_int_distribution<UShort> dist;
	return dist(RNG);
}

template<>
Int VLKRandom::GetRandom()
{
	static std::uniform_int_distribution<Int> dist;
	return dist(RNG);
}

template<>
UInt VLKRandom::GetRandom()
{
	static std::uniform_int_distribution<UInt> dist;
	return dist(RNG);
}

template<>
Long VLKRandom::GetRandom()
{
	static std::uniform_int_distribution<Long> dist;
	return dist(RNG);
}

template<>
ULong VLKRandom::GetRandom()
{
	static std::uniform_int_distribution<ULong> dist;
	return dist(RNG);
}

template<>
Float VLKRandom::GetRandom()
{
	static std::uniform_real_distribution<Float> dist(0.0f, 1.0f);
	return dist(RNG);
}

template<>
Double VLKRandom::GetRandom()
{
	static std::uniform_real_distribution<Double> dist(0.0, 1.0);
	return dist(RNG);
}

template<>
Short VLKRandom::GetRandom(Short min, Short max)
{
	assert(min >= 0 && max >= 0);
	assert(min <= max);

	return std::uniform_int_distribution<Short>(min, max)(RNG);
}

template<>
UShort VLKRandom::GetRandom(UShort min, UShort max)
{
	assert(min <= max);

	return std::uniform_int_distribution<UShort>(min, max)(RNG);
}

template<>
Int VLKRandom::GetRandom(Int min, Int max)
{
	assert(min >= 0 && max >= 0);
	assert(min <= max);

	return std::uniform_int_distribution<Int>(min, max)(RNG);
}

template<>
UInt VLKRandom::GetRandom(UInt min, UInt max)
{
	assert(min <= max);

	return std::uniform_int_distribution<UInt>(min, max)(RNG);
}

template<>
Long VLKRandom::GetRandom(Long min, Long max)
{
	assert(min >= 0 && max >= 0);
	assert(min <= max);

	return std::uniform_int_distribution<Long>(min, max)(RNG);
}

template<>
ULong VLKRandom::GetRandom(ULong min, ULong max)
{
	assert(min <= max);

	return std::uniform_int_distribution<ULong>(min, max)(RNG);
}

template<>
Float VLKRandom::GetRandom(Float min, Float max)
{
	assert(min >= 0.0f && max >= 0.0f);
	assert(min <= max);

	return std::uniform_real_distribution<Float>(min, max)(RNG);
}

template<>
Double VLKRandom::GetRandom(Double min, Double max)
{
	assert(min >= 0.0 && max >= 0.0);
	assert(min <= max);

	return std::uniform_real_distribution<Double>(min, max)(RNG);
}