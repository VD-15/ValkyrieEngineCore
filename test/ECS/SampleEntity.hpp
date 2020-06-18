#include "ValkyrieEngine/ValkyrieEngine.hpp"

using namespace vlk;

class SampleComponent : public Component<SampleComponent>
{
	public:
	SampleComponent(IEntity* e) :
		Component<SampleComponent>(e)
	{
		this->i = 0;
	}

	~SampleComponent()
	{

	}

	void OnSignal(const Signal& signal) final override
	{
		auto val = signal.GetArg<int>("Value");
		if (val.has_value())
		{
			i = val.value();
		}
		else
		{
			i = -1;
		}
	}

	Int i;
};

class SampleEntity : public Entity<SampleEntity>
{
	public:
	SampleEntity()
	{
		this->sampleComponent = new SampleComponent(this);
	}

	~SampleEntity()
	{
		delete this->sampleComponent;
	}

	SampleComponent* sampleComponent;
};
