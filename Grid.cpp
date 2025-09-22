#include "Grid.h"
#include "CollisionHelper.h"
#include "functions.h"

Grid::Grid(float x, float y, int width, int height, int cellCountX, int cellCountY) : x(x), y(y), width(width), height(height), cellCountX(cellCountX), cellCountY(cellCountY)
{
	generateGrid();
}

Grid::~Grid()
{
	for (const auto& [pos, cell] : posToCell)
	{
		delete cell;
	}
}

void Grid::addObject(Object* obj)
{
	auto& pos = obj->getPosition();
	auto& lastPos = obj->getLastPosition();
	auto& size = obj->getSize();
	
	if (objects.find(obj) == objects.end())
	{
		objects.insert(obj);

		Tmpl8::vec2i ulc = getCellCoord(pos); // Up-Left-Verticy
		Tmpl8::vec2i drc = getCellCoord({ pos + size });  // Down-Right-Verticy

		//Find new place for an object
		for (int i = ulc.x; i < drc.x + 1; i++)
		{
			for (int j = ulc.y; j < drc.y + 1; j++)
			{
				int cellIndex = i + j * cellCountX;
				cellToObjects[cellIndex].insert(obj);
			}
		}
	}
	else
	{
		Tmpl8::vec2i ulc = getCellCoord(pos); // Up-Left-Verticy
		Tmpl8::vec2i drc = getCellCoord({ pos + size });  // Down-Right-Verticy

		Tmpl8::vec2i pulc = getCellCoord(lastPos); // Previous Up-Left-Verticy
		Tmpl8::vec2i pdrc = getCellCoord({lastPos + size}); // Previous Down-Right-Verticy

		if (pulc == ulc && pdrc == drc) return;

		std::unordered_set<int> intersectCells;
		std::unordered_set<int> previousIntersectCells;

		for (int i = ulc.x; i < drc.x + 1; i++)
		{
			for (int j = ulc.y; j < drc.y + 1; j++)
			{
				int cellIndex = i + j * cellCountX;
				intersectCells.insert(cellIndex);
			}
		}

		for (int i = pulc.x; i < pdrc.x + 1; i++)
		{
			for (int j = pulc.y; j < pdrc.y + 1; j++)
			{
				int cellIndex = i + j * cellCountX;
				previousIntersectCells.insert(cellIndex);
			}
		}

		auto diffPair = diff(intersectCells, previousIntersectCells);

		for (const auto& i : diffPair.first)
		{
			cellToObjects[i].insert(obj);
		}

		for (const auto& i : diffPair.second)
		{
			cellToObjects[i].erase(obj);
		}

		/*for (const auto& i : previousIntersectCells)
		{
			cellToObjects[i].erase(obj);
		}

		for (const auto& i : intersectCells)
		{
			cellToObjects[i].insert(obj);
		}*/
	}
}

void Grid::destroyObject(Object* obj)
{
	

	for (int i = 0; i < cellCountX; i++)
	{
		for (int j = 0; j < cellCountY; j++)
		{
			int cellIndex = i + j * cellCountX;
			cellToObjects[cellIndex].erase(obj);
		}
	}

	objects.erase(obj);
}

void Grid::update(float deltaTime)
{

}

void Grid::reset()
{
	
}

void Grid::getCollisions(Object* target, std::vector<Object*>& collisions)
{
	auto& pos = target->getPosition();
	auto& size = target->getSize();

	Tmpl8::vec2i ulc{ static_cast<int>(static_cast<int>(pos.x) / cellWidth), static_cast<int>(static_cast<int>(pos.y) / cellHeight) };
	Tmpl8::vec2i drc{ static_cast<int>(static_cast<int>(pos.x + size.x) / cellWidth), static_cast<int>(static_cast<int>(pos.y + size.y) / cellHeight) };

	for (int i = ulc.x; i < drc.x + 1; i++)
	{
		for (int j = ulc.y; j < drc.y + 1; j++)
		{
			int cellIndex = i + j * cellCountX;
			collisions.reserve(cellToObjects[cellIndex].size());
			collisions.insert(collisions.end(), cellToObjects[cellIndex].begin(), cellToObjects[cellIndex].end());
		}
	}
}

void Grid::renderDEBUG(Tmpl8::Surface& screen)
{
	for (const auto& pair : cellToObjects)
	{
		if (pair.second.size() <= 0) continue;
		auto& cell = posToCell[pair.first];
		screen.Line(cell->pos.x, cell->pos.y, cell->pos.x + cell->size.x, cell->pos.y, Tmpl8::BlueMask);
		screen.Line(cell->pos.x + cell->size.x, cell->pos.y, cell->pos.x + cell->size.x, cell->pos.y + cell->size.y, Tmpl8::BlueMask);
		screen.Line(cell->pos.x + cell->size.x, cell->pos.y + cell->size.y, cell->pos.x, cell->pos.y + cell->size.y, Tmpl8::BlueMask);
		screen.Line(cell->pos.x, cell->pos.y + cell->size.y, cell->pos.x, cell->pos.y, Tmpl8::BlueMask);
	}
}

Tmpl8::vec2i Grid::getCellCoord(Tmpl8::vec2 pos)
{
	Tmpl8::vec2i vec = Tmpl8::vec2i{ static_cast<int>(static_cast<int>(pos.x) / cellWidth), static_cast<int>(static_cast<int>(pos.y) / cellHeight) };
	vec = clampVec2i(vec, { 0,0 }, { cellCountX - 1, cellCountY - 1 });
	return vec;
}

std::pair<std::unordered_set<int>, std::unordered_set<int>>Grid::diff(const std::unordered_set<int>& n, const std::unordered_set<int>& o) // new-old
{
	std::pair<std::unordered_set<int>, std::unordered_set<int>> pair;

	for (const auto& i: o)
	{
		//If element from the old set is in new
		//Insert to add set
		if (n.find(i) != n.end())
		{
			pair.first.insert(i);
		}
		//else add to the remove set
		else
		{
			pair.second.insert(i);
		}
	}

	for (const auto& i : n)
	{
		pair.first.insert(i);
	}

	return pair;
}

void Grid::generateGrid()
{
	float offSetX = static_cast<float>(ScreenWidth) / static_cast<float>(cellCountX);
	float offSetY = static_cast<float>(ScreenHeight) / static_cast<float>(cellCountY);

	cellWidth = width / cellCountX;
	cellHeight = height / cellCountY;


	for (int x = 0; x < cellCountX; x++)
	{
		for (int y = 0; y < cellCountY; y++)
		{
			auto cell = new Cell({ offSetX * static_cast<float>(x), offSetY * static_cast<float>(y) }, { offSetX, offSetY });
			posToCell[x + y * cellCountX] = cell;
		}
	}
}
