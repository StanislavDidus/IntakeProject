#include "CollisionManager.h"
#include "Functions.h"
#include "CollisionHelper.h"

CollisionManager::CollisionManager(std::shared_ptr<GameManager> gameManager) : gameManager(gameManager)
{
	
}

CollisionManager::~CollisionManager()
{
	
}

void CollisionManager::checkCollision(float deltaTime)
{
	//Reset grid
	std::vector<std::shared_ptr<Object>> objects = gameManager->getObjects();
	for (const auto& col : objects)
	{
		for (const auto& col1 : objects)
		{
			if (col == col1) continue;

			bool isCollision = false;

			if (CollisionHelper::SATCheck(col, col1))
			{
				//Objects require pixel perfect collision then check it
				if (col->checkPixelPerfectCollision && col1->checkPixelPerfectCollision)
				{
					Tmpl8::vec4 overlap = getIntersection(col, col1);

					if (PixelPerfectCheck(col, col1, overlap))
					{
						isCollision = true;
					}
				}
				//Else the collision is already occured between two bounding boxes, so we return true
				else
				{
					isCollision = true;
				}
			}

			SendCollisionEvents(col, col1, isCollision, deltaTime);
		}
	}
}

void CollisionManager::renderDEBUG(Tmpl8::Surface& screen)
{
	/*for (const auto& col : gameManager->getObjects())
	{
		auto& objVerticies = col->getVertices();
		screen.Line(objVerticies[0].x, objVerticies[0].y, objVerticies[1].x, objVerticies[1].y, Tmpl8::RedMask);
		screen.Line(objVerticies[1].x, objVerticies[1].y, objVerticies[2].x, objVerticies[2].y, Tmpl8::RedMask);
		screen.Line(objVerticies[2].x, objVerticies[2].y, objVerticies[3].x, objVerticies[3].y, Tmpl8::RedMask);
		screen.Line(objVerticies[3].x, objVerticies[3].y, objVerticies[0].x, objVerticies[0].y, Tmpl8::RedMask);
		for (const auto& col1 : gameManager->getObjects())
		{
			if (col == col1) continue;

			if (CollisionHelper::SATCheck(col, col1))
			{
				Tmpl8::vec4 overlap = getIntersection(col, col1);

				screen.Line(overlap.x, overlap.y, overlap.x + overlap.z, overlap.y, Tmpl8::GreenMask);
				screen.Line(overlap.x + overlap.z, overlap.y, overlap.x + overlap.z, overlap.y + overlap.w, Tmpl8::GreenMask);
				screen.Line(overlap.x + overlap.z, overlap.y + overlap.w, overlap.x, overlap.y + overlap.w, Tmpl8::GreenMask);
				screen.Line(overlap.x, overlap.y + overlap.w, overlap.x, overlap.y, Tmpl8::GreenMask);
			}
		}
	}*/
}

void CollisionManager::render(Tmpl8::Surface& screen)
{

}

bool CollisionManager::PointRectangle(Tmpl8::vec2 target, std::shared_ptr<Object>  col)
{
	Tmpl8::vec2 pos = col->getPosition();
	Tmpl8::vec2 size = col->getSize();

	Tmpl8::vec2 cTarget = col->getRotatedPoint(target, -1.f);

	if (cTarget.x >= pos.x &&
		cTarget.x <= pos.x + size.x &&
		cTarget.y >= pos.y &&
		cTarget.y <= pos.y + size.y)
		return true;

	return false;
}

bool CollisionManager::PixelPerfectCheck(std::shared_ptr<Object> target, std::shared_ptr<Object>  col, const Tmpl8::int4& overlap)
{
	for (int x = overlap.x; x < overlap.x + overlap.z; x++)
	{
		for (int y = overlap.y; y < overlap.y + overlap.w; y++)
		{
			Tmpl8::Pixel color1 = target->getPixelAtRotatedPosition(x, y);
			Tmpl8::Pixel color2 = col->getPixelAtRotatedPosition(x, y);

			if (color1 & 0xffffff && color2 & 0xffffff)
				return true;
		}
	}

	return false;
}

bool CollisionManager::PixelPerfectCheck(std::shared_ptr<Object>  target, std::shared_ptr<Object> col, const Tmpl8::vec4& overlap)
{
	Tmpl8::int4 iOverlap = { static_cast<int>(overlap.x), static_cast<int>(overlap.y),  static_cast<int>(overlap.z),  static_cast<int>(overlap.w) };
	return PixelPerfectCheck(target, col, iOverlap);
}

//Formulas: https://www.jeffreythompson.org/collision-detection/line-line.php
void CollisionManager::LineLineIntersection(Tmpl8::vec4 target, Tmpl8::vec4 col, std::vector<Tmpl8::vec2>& points)
{
	float x1 = target.x;
	float y1 = target.y;

	float x2 = target.z;
	float y2 = target.w;

	float x3 = col.x;
	float y3 = col.y;

	float x4 = col.z;
	float y4 = col.w;

	float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
	{
		//Intersection occured
		float intersectionX = x1 + (uA * (x2 - x1));
		float intersectionY = y1 + (uA * (y2 - y1));

		points.push_back({ intersectionX, intersectionY });
	}
}

Tmpl8::vec4 CollisionManager::getIntersection(std::shared_ptr<Object>  target, std::shared_ptr<Object>  col)
{
	std::vector<Tmpl8::vec2> intersectionPoints;

	//Get intersection points from intersected rectangels
	//Separate ractangels into 4 lines
	for (const auto& tLine : target->getEdges())
	{
		for (const auto& cLine : col->getEdges())
		{
			LineLineIntersection(tLine, cLine, intersectionPoints);
		}
	}

	//Add verticies that are inside of a rectangle
	for (const auto& tVerticy : target->getVertices())
	{
		if (PointRectangle(tVerticy.position, col))
		{
			intersectionPoints.push_back(tVerticy.position);
		}
	}
	for (const auto& cVerticy : col->getVertices())
	{
		if (PointRectangle(cVerticy.position, target))
		{
			intersectionPoints.push_back(cVerticy.position);
		}
	}

	Tmpl8::vec2 min{};
	Tmpl8::vec2 max{};
	if (intersectionPoints.size() > 0)
	{
		min = { intersectionPoints[0] };
		max = min;
		for (int i = 1; i < intersectionPoints.size(); i++)
		{
			auto& intersection = intersectionPoints[i];
			if (intersection.x < min.x) min.x = intersection.x;
			if (intersection.y < min.y) min.y = intersection.y;

			if (intersection.x > max.x) max.x = intersection.x;
			if (intersection.y > max.y) max.y = intersection.y;
		}
	}

	min = clampVec2(min, { 0.f, 0.f }, { static_cast<float>(ScreenWidth) - 1.f, static_cast<float>(ScreenHeight) - 1.f });
	max = clampVec2(max, { 0.f, 0.f }, { static_cast<float>(ScreenWidth) - 1.f, static_cast<float>(ScreenHeight) - 1.f });

	float width = max.x - min.x, height = max.y - min.y;

	return { min.x, min.y, width, height };
}

void CollisionManager::SendCollisionEvents(std::shared_ptr<Object>  A, std::shared_ptr<Object>  B, bool isCollision, float deltaTime)
{
	unordered_pair<std::shared_ptr<Object> > pair{ A,B };
	auto it = std::find(collisions.begin(), collisions.end(), pair);

	if (isCollision)
	{
		if (it == collisions.end())
		{
			collisions.push_back(pair);

			//OnCollisionEnter
			A->onCollisionEnter(B);
			B->onCollisionEnter(A);
		}
		else if (it != collisions.end())
		{
			//OnCollisionStay
			A->onCollisionStay(B, deltaTime);
			B->onCollisionStay(A, deltaTime);
		}
	}
	else if(!isCollision)
	{
		if (it != collisions.end())
		{
			collisions.erase(it);

			//OnCollisionExit
			A->onCollisionExit(B);
			B->onCollisionExit(A);
		}
	}
}