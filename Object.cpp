#include "Object.h"

void Object::update(float deltaTime)
{
}

void Object::render(Tmpl8::Surface& screen)
{
	sprite->DrawScaledRotated(x, y, width, height, angle, &screen);
}

const Tmpl8::vec2 Object::getPosition() const
{
	return Tmpl8::vec2(x, y);
}

const Tmpl8::vec2i Object::getSize() const
{
	return Tmpl8::vec2i(width, height);
}

const std::vector<Tmpl8::vec2> Object::getAxes() const
{
	std::vector<Tmpl8::vec2> axes;
	axes.reserve(2);
	axes.push_back(getEdge({ x,y }, { x + width, y })); // Up-Side
	axes.push_back(getEdge({ x,y }, { x, y + height })); // Left-Side
	axes[0].normalize();
	axes[1].normalize();

	//Optional
	//Get an orthogonal of a vector
	std::swap(axes[0].x, axes[0].y);
	axes[0].x *= -1;

	std::swap(axes[1].x, axes[1].y);
	axes[1].x *= -1;

	return axes;
}

const Tmpl8::vec2 Object::getEdge(Tmpl8::vec2 min, Tmpl8::vec2 max) const
{
	float radians = angle * 3.1415f / 180.f;
	float sin = std::sin(radians);
	float cos = std::cos(radians);

	float cx = static_cast<float>(width / 2);
	float cy = static_cast<float>(height / 2);

	Tmpl8::vec2 A = getPoint(sin, cos, cx, cy, min);
	Tmpl8::vec2 B = getPoint(sin, cos, cx, cy, max);

	return Tmpl8::vec2{ A - B };
}

const std::vector<Tmpl8::vec2> Object::getVerticies() const
{
	std::vector<Tmpl8::vec2> verticies;
	verticies.reserve(4);

	float radians = angle * 3.1415f / 180.f;
	float sin = std::sin(radians);
	float cos = std::cos(radians);

	float cx = x + static_cast<float>(width / 2);
	float cy = y + static_cast<float>(height / 2);

	verticies.push_back(getPoint(sin, cos, cx, cy, { x, y }));
	verticies.push_back(getPoint(sin, cos, cx, cy, { x + width, y }));
	verticies.push_back(getPoint(sin, cos, cx, cy, { x + width , y + height }));
	verticies.push_back(getPoint(sin, cos, cx, cy, { x, y + height }));

	return verticies;
}

inline const Tmpl8::vec2 Object::getPoint(float sin, float cos, float cx, float cy, Tmpl8::vec2 pos) const
{
	float sx = pos.x - cx;
	float sy = pos.y - cy;

	return { sx * cos - sy * sin + cx, sx * sin + sy * cos + cy };
}

//const Tmpl8::vec2 Object::getCenterCoord() const
//{
//	float radians = angle * 3.1415f / 180.f;
//	float sin = std::sin(radians);
//	float cos = std::cos(radians);
//
//	float cx = x + static_cast<float>(width / 2);
//	float cy = y + static_cast<float>(height / 2);
//
//	return { cx, cy };
//}
