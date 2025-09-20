#pragma once

#include <iostream>
#include <unordered_map>
#include <array>
#include <functional>

#include "Object.h"

//My EventBus implementation is a slightly changed version of ECS(Entity-Component-System) from this article:https://austinmorlan.com/posts/entity_component_system/

namespace event
{
	struct CollisionEvent
	{
		int n;
	};

	struct Killed
	{
		int id;
	};
}

using Obj = size_t;

class IEventArray { };

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

	void Do(const T& t)
	{
		for (int i = 0; i < size; i++)
		{
			data[i](t);
		}
	}

private:
	std::array<std::function<void(const T&)>, 1000> data;
	std::unordered_map<Obj, size_t> objectToIndex;
	std::array<Obj, 100> objects;

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

	void RegisterObject(Object* obj)
	{
		objectToId[obj] = size;
		size++;
	}

	template<typename T>
	void AddListener(Object* obj, const std::function<void(const T&)>& func)
	{
		const char* name = typeid(T).name();

		if (eventArrays.find(name) == eventArrays.end())
			RegisterEventType<T>();

		if (objectToId.find(obj) == objectToId.end())
		{
			RegisterObject(obj);
		}

		getEventArray<T>()->addData(objectToId[obj], func);
	}

	template<typename T>
	void RemovedListener(Object* obj)
	{
		const char* name = typeid(T).name();

		getEventArray<T>()->removeData(objectToId[obj]);
	}

	template<typename T>
	void Push(const T& t)
	{
		getEventArray<T>()->Do(t);
	}

private:
	std::unordered_map<const char*, std::shared_ptr<IEventArray>> eventArrays;
	std::unordered_map<Object*, Obj> objectToId;

	template<typename T>
	std::shared_ptr<EventArray<T>> getEventArray()
	{
		const char* name = typeid(T).name();

		return std::static_pointer_cast<EventArray<T>>(eventArrays[name]);
	}

	size_t size;
};

