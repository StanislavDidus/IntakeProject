#include "EventBus.h"

void EventBus::AddListener(IListener& l, EventTypes type, const std::function<void()>& func)
{
	//events[type].push_back(func);
	//subscribers[l] = func;
}

void EventBus::RemoveListener(IListener& l, EventTypes type)
{

}
