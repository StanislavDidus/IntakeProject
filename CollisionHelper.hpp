#pragma once

#include "Object.hpp"

struct Vertex
{
	Tmpl8::vec2 position;
	Tmpl8::vec2 textureCoordinate;
};

struct CollisionHelper
{
	static bool AABBCheck(std::shared_ptr<Object>  target, std::shared_ptr<Object>  col)
	{
		auto pos1 = target->getPosition();
		auto size1 = target->getSize();

		auto pos2 = col->getPosition();
		auto size2 = col->getSize();

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
	static bool SATCheck(std::shared_ptr<Object>  target, std::shared_ptr<Object>  col) //Separating Axis Theorem
	{
		//Get separated axes for each collider
		std::vector<Tmpl8::vec2> axes;
		axes.reserve(4);

		std::vector<Tmpl8::vec2> objAxes = target->getAxes();
		std::vector<Tmpl8::vec2> colAxes = col->getAxes();

		axes.insert(axes.end(), objAxes.begin(), objAxes.end());
		axes.insert(axes.end(), colAxes.begin(), colAxes.end());

		//Project vertices to all axess
		for (const auto& axis : axes)
		{
			//Go through each of the verticies
			auto objVerticies = target->getVerticesPosition();
			auto colVerticies = col->getVerticesPosition();

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
				//Early exit
				return false;
			}
		}

		//Return result
		return true;
	}

};

//Area formula 
// ||(B - A) * (C - A)|| / 2

struct Edge
{
	Edge(const Tmpl8::vec2& p0, const Tmpl8::vec2& p1, const Tmpl8::vec2& p2) : p0(p0), p1(p1), p2(p2) 
	{ 
		//Calculate an area of main triangle
		const Tmpl8::vec2 a1 = p1 - p0;
		const Tmpl8::vec2 a2 = p0 - p2;
		float cross = a1.x * a2.y - a1.y * a2.x;
		area = abs(cross) / 2.f;
	}

	bool intersect(const Tmpl8::vec2& p) const
	{
		//Calculate w
		const Tmpl8::vec2 w1 = p1 - p0;
		const Tmpl8::vec2 w2 = p - p0;
		float wcross = w1.x * w2.y - w1.y * w2.x;
		float warea = abs(wcross) / 2.f;

		//Calculate v
		const Tmpl8::vec2 v1 = p0 - p2;
		const Tmpl8::vec2 v2 = p - p2;
		float vcross = v1.x * v2.y - v1.y * v2.x;
		float varea = abs(vcross) / 2.f;

		//Calculate u
		const Tmpl8::vec2 u1 = p2 - p1;
		const Tmpl8::vec2 u2 = p - p1;
		float ucross = u1.x * u2.y - u1.y * u2.x;
		float uarea = abs(ucross) / 2.f;

		float areasum = warea + varea + uarea;

		if (abs(area - areasum) < 0.01f) return true;
		return false;
	}

	//Implementation: https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/barycentric-coordinates.html
	Tmpl8::vec3 barycentric(const Tmpl8::vec2& p) const
	{
		//Calculate w
		const Tmpl8::vec2 w1 = p1 - p0;
		const Tmpl8::vec2 w2 = p - p0;
		float wcross = w1.x * w2.y - w1.y * w2.x;
		float w = abs(wcross) / 2.f / area;

		//Calculate v
		const Tmpl8::vec2 v1 = p0 - p2;
		const Tmpl8::vec2 v2 = p - p2;
		float vcross = v1.x * v2.y - v1.y * v2.x;
		float v = abs(vcross) / 2.f / area;

		//Calculate u
		//w + u + v = 1
		float u = 1.f - w - v;

		return Tmpl8::vec3{u ,v ,w};
	}

	float area;
	Tmpl8::vec2 p0, p1, p2;
};

struct AABB
{
	AABB(float x, float y, float width, float height) : x(x), y(y), width(width), height(height)
	{

	}

	AABB(const Tmpl8::vec2& p0, const Tmpl8::vec2& p1, const Tmpl8::vec2& p2, const Tmpl8::vec2& p3)
	{
		constexpr float inf = std::numeric_limits<float>().infinity();
		float minX = inf, minY = inf, maxX = -inf, maxY = -inf;
		
		if (p0.x < minX) minX = p0.x;
		if (p0.y < minY) minY = p0.y;
		if (p0.x > maxX) maxX = p0.x;
		if (p0.y > maxY) maxY = p0.y;

		if (p1.x < minX) minX = p1.x;
		if (p1.y < minY) minY = p1.y;
		if (p1.x > maxX) maxX = p1.x;
		if (p1.y > maxY) maxY = p1.y;

		if (p2.x < minX) minX = p2.x;
		if (p2.y < minY) minY = p2.y;
		if (p2.x > maxX) maxX = p2.x;
		if (p2.y > maxY) maxY = p2.y;

		if (p3.x < minX) minX = p3.x;
		if (p3.y < minY) minY = p3.y;
		if (p3.x > maxX) maxX = p3.x;
		if (p3.y > maxY) maxY = p3.y;

		x = minX, y = minY, width = maxX - minX, height = maxY - minY;
	}

	bool intersects(const AABB& col)
	{
		return !(x + width <= col.x ||
			col.x + col.width <= x ||
			y + height <= col.y ||
			col.y + col.height <= y);
	}

	void clamp(const AABB& col)
	{
		float minX = 0.f, minY = 0.f, maxX = 0.f, maxY = 0.f;
		
		minX = std::max(x, col.x);
		minY = std::max(y, col.y);

		maxX = std::min(x + width, col.x + col.width);
		maxY = std::min(y + height, col.y + col.height);

		x = minX, y = minY, width = maxX - minX, height = maxY - minY;
	}

	float x, y, width, height;
};