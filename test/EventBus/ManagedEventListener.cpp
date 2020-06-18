#include "catch.hpp"
#include "SampleEvents.hpp"

class ManagedEventListener final : public vlk::EventListener<SampleEvent>
{
	vlk::Int lastData;

	public:
	ManagedEventListener() = default;
	ManagedEventListener(ManagedEventListener&&) = delete;
	ManagedEventListener(const ManagedEventListener&) = delete;
	ManagedEventListener& operator=(ManagedEventListener&&) = delete;
	ManagedEventListener& operator=(const ManagedEventListener&) = delete;
	virtual ~ManagedEventListener() = default;

	inline vlk::Int GetLastData() const { return this->lastData; }

	private:
	void OnEvent(const SampleEvent& ev) override
	{
		this->lastData = ev.data;
	}
};

TEST_CASE("Managed event listeners are added/removed appropriately")
{
	ManagedEventListener* mel = new ManagedEventListener();

	SampleEvent ev1(456);
	vlk::EventBus<SampleEvent>::Send(ev1);

	SECTION("EventListeners are added to the EventBus properly")
	REQUIRE(mel->GetLastData() == 456);

	delete mel;

	SampleEvent ev2(1024);

	SECTION("EventListeners are removed from the EventBus properly")
	{
		try
		{
			vlk::EventBus<SampleEvent>::Send(ev2);
			REQUIRE(true);
		}
		catch (std::exception ex)
		{
			REQUIRE(false);
		}
	}
}

TEST_CASE("Shorthand event sending works properly")
{
	ManagedEventListener* mel = new ManagedEventListener();

	SampleEvent se1(6001);
	vlk::SendEvent(se1);

	REQUIRE(mel->GetLastData() == 6001);

	delete mel;
}

