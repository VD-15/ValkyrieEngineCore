#include "ValkyrieEngine/ValkyrieEngine.hpp"

using namespace vlk;

class SampleComponent
{
	public:
	SampleComponent() = default;
	~SampleComponent() = default;
	Int i;
};

class OtherComponent
{
	public:
	OtherComponent(double _i) : i(_i) { }
	~OtherComponent() = default;
	const ULong i;
};

class Counter
{
	static int i;

	public:
	Counter()
	{
		++i;
	}

	Counter(Counter&&) = delete;
	Counter(const Counter&) = delete;
	Counter& operator=(Counter&&) = delete;
	Counter& operator=(const Counter&) = delete;

	~Counter()
	{
		--i;
	}

	static inline int GetNum() { return i; }
};

struct SimpleData
{
	int i = 5;
	double d = 6.0;
};

