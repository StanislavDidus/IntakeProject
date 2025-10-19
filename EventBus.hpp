#pragma once

#include <functional>
#include <unordered_map>

class IListener
{
public:
	IListener() {}
	virtual ~IListener() {}
};

enum EventTypes
{
	ShipDestroyed,
	UsedUpgrade,
};

class EventBus
{
public:
	void AddListener(IListener& l, EventTypes type, const std::function<void()>& func);
	void RemoveListener(IListener& l, EventTypes type);
private:
	std::unordered_map<EventTypes, std::vector<std::function<void()>>> events;
	std::unordered_map<IListener, std::function<void()>> subscribers;
};

