#include "Object.hpp"
#include "CollisionManager.hpp"
#include "CollisionHelper.hpp"

Object::Object
(
	std::shared_ptr<Tmpl8::Sprite> sprite,
	Tmpl8::vec2 position,
	Tmpl8::vec2 size
) : sprite(sprite), position(position), size(size), center(size.x / 2.f, size.y / 2.f)
{	
	
}

Object::~Object()
{
	
}

void Object::update(float deltaTime)
{
	applyVelocity(deltaTime);
}

void Object::render(Tmpl8::Surface& screen)
{
	std::vector<Vertex> v = getVertices();
	sprite->DrawScaledRotated(v[0], v[1], v[2], v[3], screen);
}

void Object::setPosition(Tmpl8::vec2 position)
{
	this->position = position;
}

void Object::setSize(Tmpl8::vec2 size)
{
	this->size = size;
}

void Object::setVelocity(Tmpl8::vec2 velocity)
{
	this->velocity = velocity;
}

void Object::setMaxSpeed(float maxSpeed)
{
	this->maxSpeed = maxSpeed;
}

void Object::setAcceleration(Tmpl8::vec2 acceleration)
{
	this->acceleration = acceleration;
}

void Object::setDirection(Tmpl8::vec2 direction)
{
	this->direction = direction;
}

void Object::setAngle(float angle)
{
	this->angle = angle;
}

void Object::setTag(const std::string& tag)
{
	this->tag = tag;
}

Tmpl8::vec2 Object::getPosition() const
{
	return position;
}

Tmpl8::vec2 Object::getSize() const
{
	return size;
}

Tmpl8::vec2 Object::getVelocity() const
{
	return velocity;
}

float Object::getMaxSpeed() const
{
	return maxSpeed;
}

Tmpl8::vec2 Object::getAcceleration() const
{
	return acceleration;
}

Tmpl8::vec2 Object::getDirection() const
{
	return direction;
}

float Object::getAngle() const
{
	return angle;
}

std::shared_ptr<Tmpl8::Sprite> Object::getSprite() const
{
	return sprite;
}

const std::string& Object::getTag() const
{
	return tag;
}

void Object::move(float deltaTime)
{
	velocity += acceleration * deltaTime;

	float speed = velocity.length();
	if (speed > maxSpeed)
	{
		velocity.x = (velocity.x / speed) * maxSpeed;
		velocity.y = (velocity.y / speed) * maxSpeed;
	}
}

void Object::applyVelocity(float deltaTime)
{
	position += velocity * deltaTime;
}

void Object::stop(float deltaTime)
{
	velocity.x -= velocity.x * deltaTime;
	velocity.y -= velocity.y * deltaTime;

	if (std::abs(velocity.x) <= 0.01f) velocity.x = 0.f;
	if (std::abs(velocity.y) <= 0.01f) velocity.y = 0.f;
}

void Object::resize(float scale)
{
	Tmpl8::vec2 center = Tmpl8::vec2{ position.x + size.x / 2.f, position.y + size.y / 2.f };

	size = size * scale;

	position = Tmpl8::vec2{center.x - size.x / 2.f, center.y - size.y / 2.f};
}

std::vector<Tmpl8::vec2> Object::getAxes() const
{
	std::vector<Tmpl8::vec2> axes;
	axes.reserve(2);
	axes.push_back(getEdgeVector(UP));
	axes.push_back(getEdgeVector(LEFT)); 
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

Tmpl8::vec2 Object::getEdgeVector(Side s) const
{
	Tmpl8::vec2 A;
	Tmpl8::vec2 B;

	switch (s)
	{
	case Side::LEFT:
		A = getVertex(UP, LEFT);
		B = getVertex(DOWN, LEFT);
		break;
	case Side::RIGHT:
		A = getVertex(UP, RIGHT);
		B = getVertex(DOWN, RIGHT);
		break;
	case Side::UP:
		A = getVertex(UP, LEFT);
		B = getVertex(UP, RIGHT);
		break;
	case Side::DOWN:
		A = getVertex(DOWN, LEFT);
		B = getVertex(DOWN, RIGHT);
		break;
	}

	return Tmpl8::vec2{ A - B };
}

std::vector<Tmpl8::vec4> Object::getEdges() const
{
	std::vector<Tmpl8::vec4> lines;
	lines.reserve(4);

	lines.push_back(getEdge(UP));
	lines.push_back(getEdge(RIGHT));
	lines.push_back(getEdge(DOWN));
	lines.push_back(getEdge(LEFT));

	return lines;
}

Tmpl8::vec4 Object::getEdge(Side s) const
{
	Tmpl8::vec2 A;
	Tmpl8::vec2 B;

	switch (s)
	{
	case Side::LEFT:
		A = getVertex(UP, LEFT);
		B = getVertex(DOWN, LEFT);
		break;
	case Side::RIGHT:
		A = getVertex(UP, RIGHT);
		B = getVertex(DOWN, RIGHT);
		break;
	case Side::UP:
		A = getVertex(UP, LEFT);
		B = getVertex(UP, RIGHT);
		break;
	case Side::DOWN:
		A = getVertex(DOWN, LEFT);
		B = getVertex(DOWN, RIGHT);
		break;
	}

	return Tmpl8::vec4{ A.x, A.y, B.x, B.y };
}


std::vector<Tmpl8::vec2> Object::getVerticesPosition() const
{
	std::vector<Tmpl8::vec2> verticies;
	verticies.reserve(4);

	verticies.emplace_back(getVertex(UP, LEFT));
	verticies.emplace_back(getVertex(UP, RIGHT));
	verticies.emplace_back(getVertex(DOWN, RIGHT));
	verticies.emplace_back(getVertex(DOWN, LEFT));

	return verticies;
}

std::vector<Vertex> Object::getVertices(std::shared_ptr<Tmpl8::Sprite> sprite, const Tmpl8::vec2& pos) const
{
	std::vector<Vertex> verticies;
	verticies.reserve(4);

	float w = static_cast<float>(sprite->GetWidth());
	float h = static_cast<float>(sprite->GetHeight());

	
	
	verticies.emplace_back(getVertexAtPos(UP, RIGHT, pos), Tmpl8::vec2{ w, 0.f });
	verticies.emplace_back(getVertexAtPos(UP, LEFT, pos), Tmpl8::vec2{ 0.f, 0.f });
	verticies.emplace_back(getVertexAtPos(DOWN, LEFT, pos), Tmpl8::vec2{ 0.f, h });
	verticies.emplace_back(getVertexAtPos(DOWN, RIGHT, pos), Tmpl8::vec2{ w, h });
	
	
	

	return verticies;
}

std::vector<Vertex> Object::getVertices() const
{
	return getVertices(sprite, position);
}

Tmpl8::vec2 Object::getVertex(Side v, Side h) const
{
	float px = 0.f, py = 0.f;
	
	switch (v)
	{
	case UP:
		py = position.y;
		break;
	case DOWN:
		py = position.y + size.y;
		break;
	}

	switch (h)
	{
	case LEFT:
		px = position.x;
		break;
	case RIGHT:
		px = position.x + size.x;
	break;
	}

	return getRotatedPoint({ px, py }, position);
}

Tmpl8::vec2 Object::getVertexAtPos(Side v, Side h, const Tmpl8::vec2& pos) const
{
	float px = 0.f, py = 0.f;

	switch (v)
	{
	case UP:
		py = pos.y;
		break;
	case DOWN:
		py = pos.y + size.y;
		break;
	}

	switch (h)
	{
	case LEFT:
		px = pos.x;
		break;
	case RIGHT:
		px = pos.x + size.x;
		break;
	}

	return getRotatedPoint({ px, py }, pos);
}

//point - is a point we want to rotate
//pos - is coordinate of an upper-left corner of the object
Tmpl8::vec2 Object::getRotatedPoint(const Tmpl8::vec2& point, const Tmpl8::vec2& pos, float dir) const
{
	float radians = angle * dir * Tmpl8::PI / 180.f;
	float sin = std::sin(radians);
	float cos = std::cos(radians);

	Tmpl8::vec2 centerPos = pos + center;
	
	float sx = point.x - centerPos.x;
	float sy = point.y - centerPos.y;
	
	return { sx * cos - sy * sin + centerPos.x, sx * sin + sy * cos + centerPos.y};
}

//Tmpl8::vec2 Object::getRotatedPointWithCenter(const Tmpl8::vec2& pos, float dir) const
//{
//	float radians = angle * dir * Tmpl8::PI / 180.f;
//	float sin = std::sin(radians);
//	float cos = std::cos(radians);
//
//	Tmpl8::vec2 centerPos = { position.x + center.x, position.y + center.y};
//
//	float sx = pos.x - centerPos.x;
//	float sy = pos.y - centerPos.y;
//
//	return { sx * cos - sy * sin + centerPos.x, sx * sin + sy * cos + centerPos.y};
//}

Tmpl8::Pixel Object::getPixelAtRotatedPosition(int pixelX, int pixelY) const
{
	Tmpl8::vec2 c = center;

	return sprite->getPixelAtRotatedPosition(static_cast<int>(position.x), static_cast<int>(position.y), pixelX, pixelY , static_cast<int>(size.x), static_cast<int>(size.y), static_cast<int>(c.x), static_cast<int>(c.y), angle);
}

void Object::onCollisionEnter(std::shared_ptr<Object> object)
{
	//Enter
}

void Object::onCollisionStay(std::shared_ptr<Object> object, float deltaTime)
{
	//Stay
}

void Object::onCollisionExit(std::shared_ptr<Object> object)
{
	//Exit
}
