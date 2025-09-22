#include "Object.h"
#include "CollisionManager.h"

Object::Object(Tmpl8::Sprite* sprite, float x, float y, int width, int height, float angle, const std::string& tag) : sprite(sprite), x(x), y(y), width(width), height(height), angle(angle), tag(tag), destroy(false), lastPosition(x, y)
{
	
}

Object::~Object()
{
	
}

void Object::update(float deltaTime)
{
	lastPosition = { x, y };
	
	//Move

	
}

void Object::render(Tmpl8::Surface& screen)
{
	sprite->DrawScaledRotated(x, y, width, height, angle, &screen);
}

const Tmpl8::vec2 Object::getPosition() const
{
	return Tmpl8::vec2(x, y);
}

const Tmpl8::vec2 Object::getSize() const
{
	return Tmpl8::vec2(static_cast<float>(width), static_cast<float>(height));
}

const std::vector<Tmpl8::vec2> Object::getAxes() const
{
	std::vector<Tmpl8::vec2> axes;
	axes.reserve(2);
	axes.push_back(getEdgeVector({ x,y }, { x + width, y })); // Up-Side
	axes.push_back(getEdgeVector({ x,y }, { x, y + height })); // Left-Side
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

const Tmpl8::vec2 Object::getEdgeVector(Tmpl8::vec2 min, Tmpl8::vec2 max) const
{
	Tmpl8::vec2 A = getRotatedPoint(min);
	Tmpl8::vec2 B = getRotatedPoint(max);

	return Tmpl8::vec2{ A - B };
}

const std::vector<Tmpl8::vec4> Object::getEdges() const
{
	std::vector<Tmpl8::vec4> lines;
	lines.reserve(4);

	lines.push_back(getEdge({x, y}, {x + width, y}));
	lines.push_back(getEdge({ x + width, y }, { x + width, y + height }));
	lines.push_back(getEdge({ x + width, y + height}, { x, y + height }));
	lines.push_back(getEdge({ x , y + height}, { x, y }));

	return lines;
}

const Tmpl8::vec4 Object::getEdge(Tmpl8::vec2 min, Tmpl8::vec2 max) const
{
	Tmpl8::vec2 A = getRotatedPoint(min);
	Tmpl8::vec2 B = getRotatedPoint(max);

	return Tmpl8::vec4{ A.x, A.y, B.x, B.y };
}

const std::vector<Tmpl8::vec2> Object::getVerticies() const
{
	std::vector<Tmpl8::vec2> verticies;
	verticies.reserve(4);

	verticies.push_back(getRotatedPoint({ x, y }));
	verticies.push_back(getRotatedPoint({ x + width, y }));
	verticies.push_back(getRotatedPoint({ x + width , y + height }));
	verticies.push_back(getRotatedPoint({ x, y + height }));

	return verticies;
}

const Tmpl8::vec2 Object::getRotatedPoint(Tmpl8::vec2 pos, float dir) const
{
	float radians = angle * dir * 3.1415f / 180.f;
	float sin = std::sin(radians);
	float cos = std::cos(radians);

	float cx = x + static_cast<float>(width / 2);
	float cy = y + static_cast<float>(height / 2);
	
	float sx = pos.x - cx;
	float sy = pos.y - cy;

	return { sx * cos - sy * sin + cx, sx * sin + sy * cos + cy };
}

Tmpl8::Pixel Object::getPixelRotatedToPosition(int pixelX, int pixelY) const
{
	return sprite->getPixelRotatedToPosition(static_cast<int>(x), static_cast<int>(y), pixelX, pixelY , width, height, angle);
}

float Object::getAngle() const
{
	return angle;
}

Tmpl8::Sprite* Object::getSprite() const
{
	return sprite;
}

const std::string& Object::getTag() const
{
	return tag;
}

const Tmpl8::vec2 Object::getLastPosition() const
{
	return lastPosition;
}

void Object::onCollisionEnter(const CollisionEvent& event)
{
	
}

void Object::onCollisionStay(const CollisionEvent& event)
{
}

void Object::onCollisionExit(const CollisionEvent& event)
{
}
