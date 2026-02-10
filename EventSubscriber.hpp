#pragma once

#include "EventBus.hpp"

// RAII class that subscriber for an event and then automatically unsubscribed at the end of the object's lifecycle
template<EventType T>
class EventSubscriber
{
public:
	EventSubscriber(const std::function<void()>& func)
	{
		caller_id = EventBus::Get().subscribe<T>(func);
	}

	~EventSubscriber()
	{
		EventBus::Get().unsubscribe<T>(caller_id);
	}

	EventSubscriber(const EventSubscriber& other) = delete;
	EventSubscriber(EventSubscriber&& other) noexcept = delete;
	EventSubscriber& operator=(const EventSubscriber& other) = delete;
	EventSubscriber& operator=(EventSubscriber&& other) noexcept = delete;
private:
	int caller_id;
};