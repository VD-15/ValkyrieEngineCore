#include <catch2/catch.hpp>
#include "SampleEvents.hpp"

/*!
 * Stores the data of the last SampleEvent it recieves
 */
class RawEventListener final : public vlk::IEventListener<SampleEvent>
{
	vlk::Int lastData;

	public:
	RawEventListener() = default;
	RawEventListener(RawEventListener&&) = delete;
	RawEventListener(const RawEventListener&) = delete;
	RawEventListener& operator=(RawEventListener&&) = delete;
	RawEventListener& operator=(const RawEventListener&) = delete;
	virtual ~RawEventListener() = default;

	inline vlk::Int GetLastData() const { return this->lastData; }

	private:
	void OnEvent(const SampleEvent& ev) override
	{
		this->lastData = ev.data;
	}
};

TEST_CASE("IEventListeners recieve events")
{
	RawEventListener* rel = new RawEventListener();
	vlk::EventBus<SampleEvent>::AddListener(rel);
	
	SampleEvent ev1(5);

	vlk::EventBus<SampleEvent>::Send(ev1);

	SECTION("IEventListeners recieve an event")
	REQUIRE(rel->GetLastData() == 5);

	SampleEvent ev2(16);
	vlk::EventBus<SampleEvent>::Send(ev2);

	SECTION("IEventListeners recieve multiple events")
	REQUIRE(rel->GetLastData() == 16);

	vlk::EventBus<SampleEvent>::RemoveListener(rel);

	SampleEvent ev3(528);
	vlk::EventBus<SampleEvent>::Send(ev2);

	SECTION("IEventListeners are removed properly")
	REQUIRE(rel->GetLastData() == 16);

	delete rel;
}

