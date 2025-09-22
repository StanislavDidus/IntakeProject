#include "PhysicObject.h"
#include "Functions.h"

std::vector<PhysicObject*> PhysicObject::physicObjects;

PhysicObject::PhysicObject
(
	Tmpl8::Sprite* sprite,
	float x,
	float y,
	int width,
	int height,
	Tmpl8::vec2 velocity,
	Tmpl8::vec2 maxVelocity,
	Tmpl8::vec2 acceleration,
	Tmpl8::vec2 direction,
	float angle,
	const std::string& tag
) : Object(sprite, x, y, width, height, angle, tag),
	velocity(velocity),
	maxVelocity(maxVelocity),
	acceleration(acceleration),
	direction(direction)
{
	physicObjects.push_back(this);
}

PhysicObject::~PhysicObject()
{
	physicObjects.erase(std::remove(physicObjects.begin(), physicObjects.end(), this), physicObjects.end());
}

void PhysicObject::render(Tmpl8::Surface& screen)
{
	sprite->DrawScaledRotated(x, y, width, height, angle, &screen);
}

void PhysicObject::move(float deltaTime)
{
	velocity += acceleration * deltaTime;

	velocity.x = clamp(velocity.x, -maxVelocity.x, maxVelocity.x);
	velocity.y = clamp(velocity.y, -maxVelocity.y, maxVelocity.y);
}

void PhysicObject::stop(float deltaTime)
{
	velocity.x -= velocity.x * deltaTime;
	velocity.y -= velocity.y * deltaTime;

	if (std::abs(velocity.x) <= 0.01f) velocity.x = 0.f;
	if (std::abs(velocity.y) <= 0.01f) velocity.y = 0.f;
}
