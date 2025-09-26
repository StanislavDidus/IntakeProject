#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <map>

#include "Object.h"

struct Cell
{
	Tmpl8::vec2 pos;
	Tmpl8::vec2 size;

	std::vector<std::weak_ptr<Object>> objects;
	std::map<std::weak_ptr<Object>, int, std::owner_less<std::weak_ptr<Object>>> objectToIndex;

	void addObject(std::shared_ptr<Object> obj)
	{
		objects.push_back(obj);
		objectToIndex[obj] = capacity;
		capacity++;
	}

	void removeObject(std::shared_ptr<Object> obj)
	{
		if (objects.size() <= 0) return;

		int lastindex = capacity - 1;
		int removedIndex = objectToIndex[obj];

		std::swap(objects[removedIndex], objects[lastindex]);

		objectToIndex[objects[removedIndex]] = removedIndex;
		
		objects.erase(objects.end() - 1);
		objectToIndex.erase(obj);

		capacity--;
	}

	const std::vector<std::weak_ptr<Object>>& getObjects()
	{
		return objects;
	}

	int capacity;
};

class Grid
{
public:
	Grid(float x, float y, int width, int height, int cellCountX, int cellCountY);
	virtual ~Grid();

	void addObject(std::shared_ptr<Object> obj);
	void clearGarbage(std::shared_ptr<Object> obj);
	//void deleteObject(std::shared_ptr<Object> obj);
	void update(float deltaTime);
	void reset();
	void getCollisions(std::shared_ptr<Object>  target, std::vector<std::shared_ptr<Object>>& collisions);

	void renderDEBUG(Tmpl8::Surface& screen);
private:
	float x, y;
	int width, height;
	int cellCountX, cellCountY;
	int cellWidth, cellHeight;

	std::map<std::weak_ptr<Object>, std::vector<std::weak_ptr<Cell>>, std::owner_less<std::weak_ptr<Object>>> IdToCells;

	std::vector<std::shared_ptr<Cell>>cellPosToCell;

	//std::unordered_map<int, std::vector<std::weak_ptr<Object>>> cellPosToObjects;

	//std::vector<std::weak_ptr<Object>> objects;

	Tmpl8::vec2i getCellCoord(Tmpl8::vec2 pos);
	bool clampCellCoords(Tmpl8::vec2i& ul, Tmpl8::vec2i& br); //Clamp them to the screen and return false if failed
	std::pair<std::unordered_set<int>, std::unordered_set<int>> diff(const std::unordered_set<int>& newCells, const std::unordered_set<int>& oldCells);

	void generateGrid();
};