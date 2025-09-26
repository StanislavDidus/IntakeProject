#include "Grid.h"
#include "CollisionHelper.h"
#include "functions.h"
#include <algorithm>

Grid::Grid(float x, float y, int width, int height, int cellCountX, int cellCountY) : x(x), y(y), width(width), height(height), cellCountX(cellCountX), cellCountY(cellCountY)
{
	generateGrid();
}

Grid::~Grid()
{
	
}

void Grid::addObject(std::shared_ptr<Object> obj)
{
	auto& pos = obj->getPosition();
	auto& lastPos = obj->getLastPosition();
	auto& size = obj->getSize();
	
	if (IdToCells.find(obj) == IdToCells.end())
	{
		Tmpl8::vec2i ulc = getCellCoord(pos); // Up-Left-Verticy
		Tmpl8::vec2i brc = getCellCoord({ pos + size });  // Bottom-Right-Verticy

		if (!clampCellCoords(ulc, brc)) return;

		//Find new place for an object
		for (int i = ulc.x; i < brc.x + 1; i++)
		{
			for (int j = ulc.y; j < brc.y + 1; j++)
			{
				int cellIndex = i + j * cellCountX;

				auto& cell = cellPosToCell[cellIndex];
				cell->addObject(obj);
				IdToCells[obj].push_back(cell);
			}
		}
	}
	else
	{
		Tmpl8::vec2i ulc = getCellCoord(pos); // Up-Left-Verticy
		Tmpl8::vec2i brc = getCellCoord({ pos + size });  // Bottom-Right-Verticy

		if (!clampCellCoords(ulc, brc)) return;

		Tmpl8::vec2i pulc = getCellCoord(lastPos); // Previous Up-Left-Verticy
		Tmpl8::vec2i pbrc = getCellCoord({lastPos + size}); // Previous Bottom-Right-Verticy

		if (!clampCellCoords(pulc, pbrc)) return;

		if (pulc == ulc && pbrc == brc) return;

		std::unordered_set<int> intersectCells;
		std::unordered_set<int> previousIntersectCells;

		for (int i = ulc.x; i < brc.x + 1; i++)
		{
			for (int j = ulc.y; j < brc.y + 1; j++)
			{
				int cellIndex = i + j * cellCountX;
				intersectCells.insert(cellIndex);
			}
		}

		for (int i = pulc.x; i < pbrc.x + 1; i++)
		{
			for (int j = pulc.y; j < pbrc.y + 1; j++)
			{
				int cellIndex = i + j * cellCountX;
				previousIntersectCells.insert(cellIndex);
			}
		}

		auto diffPair = diff(intersectCells, previousIntersectCells);

		for (const auto& i : diffPair.first)
		{
			//cellPosToObjects[i].push_back(obj);

			auto& cell = cellPosToCell[i];
			cell->addObject(obj);
			IdToCells[obj].push_back(cell);
		}

		for (const auto& i : diffPair.second)
		{
			//cellToObjects[i].erase(obj);

			//std::erase(cellToObjects[i], obj);

			auto& cell = cellPosToCell[i];
			cell->removeObject(obj);
			//IdToCells[obj].push_back(cell);
			std::remove_if(IdToCells[obj].begin(), IdToCells[obj].end(), [cell](std::weak_ptr<Cell> c) {if (auto s = c.lock()) return s == cell; return false; });
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

void Grid::clearGarbage(std::shared_ptr<Object> obj)
{
	for (const auto& cell : IdToCells[obj])
	{
		if (auto s = cell.lock())
		{
			s->removeObject(obj);
		}
	}

	IdToCells.erase(obj);
}

//void Grid::deleteObject(std::shared_ptr<Object> obj)
//{
//}

void Grid::update(float deltaTime)
{

}

void Grid::reset()
{
	
}

void Grid::getCollisions(std::shared_ptr<Object> target, std::vector<std::shared_ptr<Object>>& collisions)
{
	auto& pos = target->getPosition();
	auto& size = target->getSize();

	Tmpl8::vec2i ulc = getCellCoord(pos);
	Tmpl8::vec2i brc = getCellCoord(pos + size);

	if (!clampCellCoords(ulc, brc)) return;

	for (int i = ulc.x; i < brc.x + 1; i++)
	{
		for (int j = ulc.y; j < brc.y + 1; j++)
		{
			int cellIndex = i + j * cellCountX;
			collisions.reserve(cellPosToCell[cellIndex]->getObjects().size());
			//collisions.insert(collisions.end(), cellToObjects[cellIndex].begin(), cellToObjects[cellIndex].end());
			
			//std::copy_if(cellToObjects[cellIndex], collisions.end(), [](std::weak_ptr<Object> obj) { return !obj.expired(); });

			//std::copy_if(cellToObjects[cellIndex].begin(), cellToObjects[cellIndex].end(), collisions.end(), [](std::weak_ptr<Object> obj) { return !obj.expired(); });

			for (const auto& obj : cellPosToCell[cellIndex]->getObjects())
			{
				if (auto s =  obj.lock())
				{
					collisions.push_back(s);
				}
			}
		}
	}
}

void Grid::renderDEBUG(Tmpl8::Surface& screen)
{
	for (int i = 0; i < cellPosToCell.size(); i++)
	{
		//if (cellPosToCell[i]->getObjects().size() <= 0) continue;

		bool exp = false;
		for (const auto& c : cellPosToCell[i]->getObjects())
		{
			if (c.expired())
				exp = true;
		}
		if (exp) continue;
		auto& cell = cellPosToCell[i];
		screen.Line(cell->pos.x, cell->pos.y, cell->pos.x + cell->size.x, cell->pos.y, Tmpl8::BlueMask);
		screen.Line(cell->pos.x + cell->size.x, cell->pos.y, cell->pos.x + cell->size.x, cell->pos.y + cell->size.y, Tmpl8::BlueMask);
		screen.Line(cell->pos.x + cell->size.x, cell->pos.y + cell->size.y, cell->pos.x, cell->pos.y + cell->size.y, Tmpl8::BlueMask);
		screen.Line(cell->pos.x, cell->pos.y + cell->size.y, cell->pos.x, cell->pos.y, Tmpl8::BlueMask);
	}
}

Tmpl8::vec2i Grid::getCellCoord(Tmpl8::vec2 pos)
{
	Tmpl8::vec2i vec = Tmpl8::vec2i{ static_cast<int>(static_cast<int>(pos.x) / cellWidth), static_cast<int>(static_cast<int>(pos.y) / cellHeight) };

	return vec;
}

bool Grid::clampCellCoords(Tmpl8::vec2i& ul, Tmpl8::vec2i& br)
{
	//if((ul.x < 0 || ul.x >= cellCountX || ul.y < 0 || ul.y >= cellCountY) && (br.x < 0 || br.x >= cellCountX || br.y < 0 || br.y >= cellCountY))
		//return false;
	
	ul = clampVec2i(ul, { 0,0 }, { cellCountX - 1, cellCountY - 1 });
	br = clampVec2i(br, { 0,0 }, { cellCountX - 1, cellCountY - 1 });
	
	return true;
}

std::pair<std::unordered_set<int>, std::unordered_set<int>>Grid::diff(const std::unordered_set<int>& newCells, const std::unordered_set<int>& oldCells) // new-old
{
	std::pair<std::unordered_set<int>, std::unordered_set<int>> pair;

	for (const auto& i: oldCells)
	{
		//If element from the old set is in new
		//Insert to add set
		if (newCells.find(i) == newCells.end())
		{
			pair.second.insert(i);
		}
	}

	for (const auto& i : newCells)
	{
		if (oldCells.find(i) == oldCells.end())
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

	for (int y = 0; y < cellCountX; y++)
	{
		for (int x = 0; x < cellCountY; x++)
		{
			auto cell = std::make_shared<Cell>(Tmpl8::vec2{ offSetX * static_cast<float>(x), offSetY * static_cast<float>(y) }, Tmpl8::vec2{ offSetX, offSetY });
			cellPosToCell.push_back(cell);
		}
	}
}
