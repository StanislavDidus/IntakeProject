#pragma once

#include <iostream>
#include <unordered_map>
#include <array>
#include <functional>
#include <queue>

#include "Object.h"

//My EventBus implementation is a slightly changed version of ECS(Entity-Component-System) from this article:https://austinmorlan.com/posts/entity_component_system/

constexpr int MAX_OBJECTS = 1000;

class Listener
{
public:
	Listener()
	{

	}
	virtual ~Listener()
	{

	}
private:

};

namespace event
{
	struct createBullet
	{
		Tmpl8::vec2 pos;
		Tmpl8::vec2i size;
		Tmpl8::vec2 direction;
		float angle;
	};

	struct objectDestroyed
	{
		Object* obj;
	};
}

using Obj = size_t;

class IEventArray 
{ 
public:
	virtual void Do() = 0;
};

template<typename T>
class EventArray : public IEventArray
{
public:
	EventArray() : size(0)
	{

	}

	virtual ~EventArray()
	{

	}

	void addData(Obj id, const std::function<void(const T&)>& func)
	{
		size_t newSize = size;

		objectToIndex[id] = newSize;
		data[newSize] = func;
		objects[newSize] = id;
		size++;
	}

	void removeData(Obj id)
	{
		size_t removedIndex = objectToIndex[id];
		size_t lastIndex = size - 1;

		std::swap(data[lastIndex], data[removedIndex]);
		std::swap(objects[lastIndex], objects[removedIndex]);

		objectToIndex[objects[removedIndex]] = removedIndex;

		objectToIndex.erase(id);
		size--;
	}

	const std::function<void(const T&)>& getData(Obj id)
	{
		return data[objectToIndex[id]];
	}

	void Push(const T& t)
	{
		dataQueue.push(t);
	}

	void Do() override
	{
		while (!dataQueue.empty())
		{
			auto& d = dataQueue.front();
			dataQueue.pop();

			for (int i = 0; i < size; i++)
			{
				data[i](d);
			}

		}
	}

private:
	std::array<std::function<void(const T&)>, MAX_OBJECTS> data;
	std::unordered_map<Obj, size_t> objectToIndex;
	std::array<Obj, MAX_OBJECTS> objects;
	std::queue<T> dataQueue;

	size_t size;
};

class EventBus
{
public:
	EventBus() : size(0)
	{

	}

	virtual ~EventBus()
	{

	}

	static EventBus& Get();

	template<typename T>
	void RegisterEventType()
	{
		const char* name = typeid(T).name();
		
		auto eventArray = std::make_shared<EventArray<T>>();

		eventArrays[name] = eventArray;
	}

	void RegisterListener(Listener* obj)
	{
		objectToId[obj] = size;
		size++;
	}

	template<typename T>
	void AddListener(Listener* obj, const std::function<void(const T&)>& func)
	{
		const char* name = typeid(T).name();

		if (eventArrays.find(name) == eventArrays.end())
			RegisterEventType<T>();

		if (objectToId.find(obj) == objectToId.end())
		{
			RegisterListener(obj);
		}

		getEventArray<T>()->addData(objectToId[obj], func);
	}

	template<typename T>
	void RemovedListener(Listener* obj)
	{
		const char* name = typeid(T).name();

		getEventArray<T>()->removeData(objectToId[obj]);
	}

	template<typename T>
	void Push(const T& t)
	{
		const char* name = typeid(T).name();

		getEventArray<T>()->Push(t);
		eventQueue.push(name);
	}

	void process()
	{
		while(!eventQueue.empty())
		{
			auto& eventName = eventQueue.front();
			eventQueue.pop();

			eventArrays[eventName]->Do();
		}
	}

private:
	std::unordered_map<const char*, std::shared_ptr<IEventArray>> eventArrays;
	std::unordered_map<Listener*, Obj> objectToId;
	std::queue<const char*> eventQueue;

	template<typename T>
	std::shared_ptr<EventArray<T>> getEventArray()
	{
		const char* name = typeid(T).name();

		return std::static_pointer_cast<EventArray<T>>(eventArrays[name]);
	}

	size_t size;
};

