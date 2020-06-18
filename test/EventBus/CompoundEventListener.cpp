#include "catch.hpp"
#include "SampleEvents.hpp"

class CompoundEventListener final : 
	public vlk::EventListener<SampleEvent>,
	public vlk::EventListener<AnotherEvent>
{
	vlk::Int lastInt;
	std::string lastString;

	public:
	CompoundEventListener() = default;
	CompoundEventListener(CompoundEventListener&&) = delete;
	CompoundEventListener(const CompoundEventListener&) = delete;
	CompoundEventListener& operator=(CompoundEventListener&&) = delete;
	CompoundEventListener& operator=(const CompoundEventListener&) = delete;
	virtual ~CompoundEventListener() = default;

	inline vlk::Int GetLastInt() const { return this->lastInt; }
	inline const std::string& GetLastString() const { return this->lastString; }

	private:
	void OnEvent(const SampleEvent& ev) override
	{
		this->lastInt = ev.data;
	}

	void OnEvent(const AnotherEvent& ev) override
	{
		this->lastString = ev.data;
	}
};

TEST_CASE("Compound event listeners function properly")
{
	CompoundEventListener* cel = new CompoundEventListener();

	SampleEvent se1(5000);
	AnotherEvent ae1("UwU");
	vlk::EventBus<SampleEvent>::Send(se1);
	vlk::EventBus<AnotherEvent>::Send(ae1);

	REQUIRE(cel->GetLastInt() == 5000);
	REQUIRE(cel->GetLastString() == "UwU");

	delete cel;
}

