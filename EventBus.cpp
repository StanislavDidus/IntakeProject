#include "EventBus.h"

EventBus& EventBus::Get()
{
	static EventBus instance;
	return instance;
}
