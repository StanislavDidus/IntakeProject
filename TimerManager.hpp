#pragma once

#include<vector>
#include <functional>

struct Timer
{
	float time;
	std::function<void()> func;
};

class TimerManager
{
public:
	TimerManager() {}
	virtual ~TimerManager() {}

	void addTimer(float time, const std::function<void()>& func)
	{
		timers.emplace_back(time, func);
	}

	void update(float deltaTime)
	{
		for (auto it = timers.begin(); it != timers.end();)
		{
			Timer& timer = *it;
			timer.time -= deltaTime;
			if (timer.time <= 0.f)
			{
				timer.func();
				it = timers.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

private:
	std::vector<Timer> timers;
};