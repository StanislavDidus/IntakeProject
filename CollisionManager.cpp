#include "CollisionManager.h"
#include "Functions.h"

CollisionManager::CollisionManager()
{
}

void CollisionManager::checkCollision()
{
	for (const auto& col : Object::objects)
	{
		for (const auto& col1 : Object::objects)
		{
			if (col == col1) continue;

			bool isCollision = false;

			if (SAT(col, col1))
			{
				Tmpl8::vec4 overlap = getIntersection(col, col1);

				if (PixelPerfectCheck(col, col1, overlap))
				{
					isCollision = true;
				}
			}

			CheckCollisionStatus(col, col1, isCollision);
		}
	}
}

void CollisionManager::renderDEBUG(Tmpl8::Surface& screen)
{
	for (const auto& col : Object::objects)
	{
		auto& objVerticies = col->getVerticies();
		screen.Line(objVerticies[0].x, objVerticies[0].y, objVerticies[1].x, objVerticies[1].y, Tmpl8::RedMask);
		screen.Line(objVerticies[1].x, objVerticies[1].y, objVerticies[2].x, objVerticies[2].y, Tmpl8::RedMask);
		screen.Line(objVerticies[2].x, objVerticies[2].y, objVerticies[3].x, objVerticies[3].y, Tmpl8::RedMask);
		screen.Line(objVerticies[3].x, objVerticies[3].y, objVerticies[0].x, objVerticies[0].y, Tmpl8::RedMask);
		for (const auto& col1 : Object::objects)
		{
			if (col == col1) continue;

			if (SAT(col, col1))
			{
				Tmpl8::vec4 overlap = getIntersection(col, col1);

				screen.Line(overlap.x, overlap.y, overlap.x + overlap.z, overlap.y, Tmpl8::GreenMask);
				screen.Line(overlap.x + overlap.z, overlap.y, overlap.x + overlap.z, overlap.y + overlap.w, Tmpl8::GreenMask);
				screen.Line(overlap.x + overlap.z, overlap.y + overlap.w, overlap.x, overlap.y + overlap.w, Tmpl8::GreenMask);
				screen.Line(overlap.x, overlap.y + overlap.w, overlap.x, overlap.y, Tmpl8::GreenMask);
			}
		}
	}
}

void CollisionManager::render(Tmpl8::Surface& screen)
{

}

bool CollisionManager::AABB(Object* target, Object* col)
{
	auto& pos1 = target->getPosition();
	auto& size1 = target->getSize();

	auto& pos2 = col->getPosition();
	auto& size2 = col->getSize();

	if (!(
		pos1.x + size1.x <= pos2.x ||
		pos2.x + size2.x <= pos1.x ||
		pos1.y + size1.y <= pos2.y ||
		pos2.y + size2.y <= pos1.y
		))
		return true;
	return false;
}

/*
Inspiration: https://dyn4j.org/2010/01/sat/#sat-axes
*/
bool CollisionManager::SAT(Object* target, Object* col)
{
	//Get separated axes for each collider
	std::vector<Tmpl8::vec2> axes;
	axes.reserve(2);

	std::vector<Tmpl8::vec2> objAxes = target->getAxes();
	std::vector<Tmpl8::vec2> colAxes = col->getAxes();

	axes.insert(axes.end(), objAxes.begin(), objAxes.end());
	axes.insert(axes.end(), colAxes.begin(), colAxes.end());

	//Project vertices to all axess
	for (const auto& axis : axes)
	{
		//Go through each of the verticies
		std::vector<Tmpl8::vec2> objVerticies = target->getVerticies();
		std::vector<Tmpl8::vec2> colVerticies = col->getVerticies();

		float min = objVerticies[0].dot(axis), max = min;
		for (int i = 1; i < 4; i++)
		{
			float proj = objVerticies[i].dot(axis);
			if (proj < min) min = proj;
			if (proj > max) max = proj;
		}

		float min1 = colVerticies[0].dot(axis), max1 = min1;
		for (int i = 1; i < 4; i++)
		{
			float proj = colVerticies[i].dot(axis);
			if (proj < min1) min1 = proj;
			if (proj > max1) max1 = proj;
		}

		//Check instersection
		if (max < min1 || max1 < min)
		{
			return false;
		}
	}

	//Return result
	return true;
}

bool CollisionManager::PointRectangle(Tmpl8::vec2 target, Object* col)
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

bool CollisionManager::PixelPerfectCheck(Object* target, Object* col, const Tmpl8::int4& overlap)
{
	for (int x = overlap.x; x < overlap.x + overlap.z; x++)
	{
		for (int y = overlap.y; y < overlap.y + overlap.w; y++)
		{
			Tmpl8::Pixel color1 = target->getPixelRotatedToPosition(x, y);
			Tmpl8::Pixel color2 = col->getPixelRotatedToPosition(x, y);

			if (color1 & 0xffffff && color2 & 0xffffff)
				return true;
		}
	}

	return false;
}

bool CollisionManager::PixelPerfectCheck(Object* target, Object* col, const Tmpl8::vec4& overlap)
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

Tmpl8::vec4 CollisionManager::getIntersection(Object* target, Object* col)
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
	for (const auto& tVerticy : target->getVerticies())
	{
		if (PointRectangle(tVerticy, col))
		{
			intersectionPoints.push_back(tVerticy);
		}
	}
	for (const auto& cVerticy : col->getVerticies())
	{
		if (PointRectangle(cVerticy, target))
		{
			intersectionPoints.push_back(cVerticy);
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

void CollisionManager::CheckCollisionStatus(Object* A, Object* B, bool isCollision)
{
	unordered_pair<Object*> pair{ A,B };
	auto it = std::find(collisions.begin(), collisions.end(), pair);

	if (isCollision)
	{
		if (it == collisions.end())
		{
			collisions.push_back(pair);

			//OnCollisionEnter
			A->onCollisionEnter(CollisionEvent{ B });
			B->onCollisionEnter(CollisionEvent{ A });
		}
		else if (it != collisions.end())
		{
			//OnCollisionStay
			A->onCollisionStay(CollisionEvent{ B });
			B->onCollisionStay(CollisionEvent{ A });
		}
	}
	else
	{
		if (it != collisions.end())
		{
			collisions.erase(it);

			//OnCollisionExit
			A->onCollisionExit(CollisionEvent{ B });
			B->onCollisionExit(CollisionEvent{ A });
		}
	}
}