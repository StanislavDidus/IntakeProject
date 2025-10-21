#pragma once

#include <iostream>
#include <unordered_map>
#include <functional>
#include <vector>
#include <queue>

enum class EventType
{
	GAMEOVER,
	RESTART,
};

class Listener { };

struct Event
{
	Listener* subscriber;
	std::function<void()> function;
};

class EventBus
{
public:
	static EventBus& Get()
	{
		static EventBus instance;
		return instance;
	}

	template<EventType T>
	void subscribe(Listener* l, const std::function<void()>& func)
	{
		events[T].emplace_back(l, func);
	}

	template<EventType T>
	void unsubscribe(Listener* l)
	{
		auto& vector = events[T];
		vector.erase(std::remove_if(vector.begin(), vector.end(), [l](const Event& e) {if (e.subscriber == l) return true; }), vector.end());
	}

	template<EventType T>
	void push()
	{
		for (auto& event : events[T])
		{
			eventQueue.push(event);
		}
	}

	void process()
	{
		while (!eventQueue.empty())
		{
			eventQueue.front().function();
			eventQueue.pop();
		}
	}

private:
	std::unordered_map<EventType, std::vector<Event>> events;
	std::queue<Event> eventQueue;
};

