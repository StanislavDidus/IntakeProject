#include "Object.h"
#include "CollisionManager.h"

Object::Object
(
	std::shared_ptr<Tmpl8::Sprite> sprite,
	float x,
	float y,
	int width,
	int height,
	Tmpl8::vec2 velocity,
	float maxSpeed,
	Tmpl8::vec2 acceleration,
	Tmpl8::vec2 direction,
	float angle,
	const std::string& tag
) : sprite(sprite), x(x), y(y), width(width), height(height), angle(angle),
	tag(tag), destroy(false), velocity(velocity), maxSpeed(maxSpeed),
	acceleration(acceleration), direction(direction)
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
	sprite->DrawScaledRotated(*this, screen);
}

void Object::renderAt(Tmpl8::Sprite& sprite, float x, float y, Tmpl8::Surface& screen)
{
	const auto& verticies = getVertices({x, y});
	sprite.DrawScaledRotated(verticies, x, y, width, height, angle, screen);
}

const Tmpl8::vec2 Object::getPosition() const
{
	return Tmpl8::vec2(x, y);
}

const Tmpl8::vec2 Object::getSize() const
{
	return Tmpl8::vec2(static_cast<float>(width), static_cast<float>(height));
}

const Tmpl8::vec2 Object::getDirection() const
{
	return direction;
}

void Object::move(float deltaTime)
{
	velocity += acceleration * deltaTime;

	float speed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
	if (speed > maxSpeed)
	{
		velocity.x = (velocity.x / speed) * maxSpeed;
		velocity.y = (velocity.y / speed) * maxSpeed;
	}
}

void Object::applyVelocity(float deltaTime)
{
	x += velocity.x * deltaTime;
	y += velocity.y * deltaTime;
}

void Object::stop(float deltaTime)
{
	velocity.x -= velocity.x * deltaTime;
	velocity.y -= velocity.y * deltaTime;

	if (std::abs(velocity.x) <= 0.01f) velocity.x = 0.f;
	if (std::abs(velocity.y) <= 0.01f) velocity.y = 0.f;
}

const std::vector<Tmpl8::vec2> Object::getAxes() const
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

const Tmpl8::vec2 Object::getEdgeVector(Side s) const
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

const std::vector<Tmpl8::vec4> Object::getEdges() const
{
	std::vector<Tmpl8::vec4> lines;
	lines.reserve(4);

	lines.push_back(getEdge(UP));
	lines.push_back(getEdge(RIGHT));
	lines.push_back(getEdge(DOWN));
	lines.push_back(getEdge(LEFT));

	return lines;
}

const Tmpl8::vec4 Object::getEdge(Side s) const
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

const std::vector<Tmpl8::vec2> Object::getVertices(Tmpl8::vec2 pos) const
{
	std::vector<Tmpl8::vec2> verticies;
	verticies.reserve(4);

	verticies.push_back(getVertexAtPos(UP, LEFT, pos));
	verticies.push_back(getVertexAtPos(UP, RIGHT, pos));
	verticies.push_back(getVertexAtPos(DOWN, RIGHT, pos));
	verticies.push_back(getVertexAtPos(DOWN, LEFT, pos));

	return verticies;
}

const std::vector<Tmpl8::vec2> Object::getVertices() const
{
	return getVertices({ x, y });
}

const Tmpl8::vec2 Object::getVertex(Side v, Side h) const
{
	float px = 0.f, py = 0.f;
	
	switch (v)
	{
	case UP:
		py = y;
		break;
	case DOWN:
		py = y + height;
		break;
	}

	switch (h)
	{
	case LEFT:
		px = x;
		break;
	case RIGHT:
		px = x + width;
	break;
	}

	return getRotatedPoint({ px, py });
}

const Tmpl8::vec2 Object::getVertexAtPos(Side v, Side h, Tmpl8::vec2 pos) const
{
	float px = 0.f, py = 0.f;

	switch (v)
	{
	case UP:
		py = pos.y;
		break;
	case DOWN:
		py = pos.y + height;
		break;
	}

	switch (h)
	{
	case LEFT:
		px = pos.x;
		break;
	case RIGHT:
		px = pos.x + width;
		break;
	}

	return getRotatedPointWithCenter({ px, py }, {pos.x, pos.y});
}

const Tmpl8::vec2 Object::getRotatedPoint(Tmpl8::vec2 pos, float dir) const
{
	float radians = angle * dir * Tmpl8::PI / 180.f;
	float sin = std::sin(radians);
	float cos = std::cos(radians);

	float cx = x + static_cast<float>(width / 2);
	float cy = y + static_cast<float>(height / 2);
	
	float sx = pos.x - cx;
	float sy = pos.y - cy;

	return { sx * cos - sy * sin + cx, sx * sin + sy * cos + cy };
}

const Tmpl8::vec2 Object::getRotatedPointWithCenter(Tmpl8::vec2 pos, Tmpl8::vec2 center, float dir) const
{
	float radians = angle * dir * Tmpl8::PI / 180.f;
	float sin = std::sin(radians);
	float cos = std::cos(radians);

	float cx = center.x + static_cast<float>(width / 2);
	float cy = center.y + static_cast<float>(height / 2);

	float sx = pos.x - cx;
	float sy = pos.y - cy;

	return { sx * cos - sy * sin + cx, sx * sin + sy * cos + cy };
}

Tmpl8::Pixel Object::getPixelAtRotatedPosition(int pixelX, int pixelY) const
{
	return sprite->getPixelAtRotatedPosition(static_cast<int>(x), static_cast<int>(y), pixelX, pixelY , width, height, angle);
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

void Object::onCollisionEnter(std::shared_ptr<Object> object)
{
	//Enter
}

void Object::onCollisionStay(std::shared_ptr<Object> object)
{
	//Stay
}

void Object::onCollisionExit(std::shared_ptr<Object> object)
{
	//Exit
}
