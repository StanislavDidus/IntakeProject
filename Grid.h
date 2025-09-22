#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "Object.h"

struct Cell
{
	Tmpl8::vec2 pos;
	Tmpl8::vec2 size;
};

class Grid
{
public:
	Grid(float x, float y, int width, int height, int cellCountX, int cellCountY);
	virtual ~Grid();

	void addObject(Object* obj);
	void destroyObject(Object* obj);
	void update(float deltaTime);
	void reset();
	void getCollisions(Object* target, std::vector<Object*>& collisions);

	void renderDEBUG(Tmpl8::Surface& screen);
private:
	float x, y;
	int width, height;
	int cellCountX, cellCountY;
	int cellWidth, cellHeight;

	std::unordered_map<int, std::unordered_set<Object*>> cellToObjects;
	std::unordered_map<int, Cell*> posToCell;

	std::set<Object*> objects;

	Tmpl8::vec2i getCellCoord(Tmpl8::vec2 pos);
	std::pair<std::unordered_set<int>, std::unordered_set<int>> diff(const std::unordered_set<int>& n, const std::unordered_set<int>& o);

	void generateGrid();
};