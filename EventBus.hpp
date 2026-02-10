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
	PLAYER_USED_UPGRADE,
	//UPGRADE_PLAYER_ENGINE,

	//Debug
	KILL_ALL,
};

struct Event
{
	int caller_id;
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

	// Each object receives unique called_id (int) to then unsubscribe from an event
	template<EventType T>
	int subscribe(const std::function<void()>& func)
	{
		events[T].emplace_back(callers, func);
		return callers++;
	}

	template<EventType T>
	void unsubscribe(int caller_id)
	{
		auto& vector = events[T];
		vector.erase(std::remove_if(vector.begin(), vector.end(), [caller_id](const Event& e) { return e.caller_id == caller_id; }), vector.end());
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
	//The number of objects subscribed for any event
	int callers = 0;
};

