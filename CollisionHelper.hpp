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

constexpr bool isTopLeft(float ax, float ay, float bx, float by) noexcept
{
	return (ay > by) || (ay == by && ax < bx);  // TODO: Check correctness of top-left rule.
}

constexpr bool isTopLeft(const Tmpl8::vec2& a, const Tmpl8::vec2& b) noexcept
{
	return isTopLeft(a.x, a.y, b.x, b.y);
}

constexpr float orient2D(float ax, float ay, float bx, float by, float cx, float cy) noexcept
{
	return (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
}

constexpr int orient2D(const Tmpl8::vec2& a, const Tmpl8::vec2& b, const Tmpl8::vec2& c) noexcept
{
	return orient2D(a.x, a.y, b.x, b.y, c.x, c.y);
}

struct Edge
{
	Edge(const Tmpl8::vec2& p0, const Tmpl8::vec2& p1, const Tmpl8::vec2& p2, const Tmpl8::vec2& p) : dX{ p1.x - p0.x, p2.x - p1.x, p0.x - p2.x }, dY{ p0.y - p1.y, p1.y - p2.y, p2.y - p0.y }
	{ 
		area = orient2D(p0, p1, p2);

		float bias0 = isTopLeft(p1, p2) ? 0.f : -1.f;
		float bias1 = isTopLeft(p2, p0) ? 0.f : -1.f;
		float bias2 = isTopLeft(p0, p1) ? 0.f : -1.f;

		w0.x = orient2D(p1, p2, p) + bias0;
		w0.y = orient2D(p2, p0, p) + bias1;
		w0.z = orient2D(p0, p1, p) + bias2;

		w = w0;
	}

	bool inside() const
	{
		return (static_cast<int>(w.x) | static_cast<int>(w.y) | static_cast<int>(w.z)) >= 0;  // Check if all weights are non-negative.
	}

	 void stepX()
    {
        w[0] += dY[1];
        w[1] += dY[2];
        w[2] += dY[0];
    }

    void stepY()
    {
        w0[0] += dX[1];
        w0[1] += dX[2];
        w0[2] += dX[0];

        w = w0;
    }


	Tmpl8::vec3 barycentric() const
	{
		float u = w.x / area;
		float v = w.y / area;

		return { u, v, 1.0f - (u + v) };
	}

	float area;
	Tmpl8::vec2 p0, p1, p2;

	Tmpl8::vec3 dX, dY, w0, w;
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