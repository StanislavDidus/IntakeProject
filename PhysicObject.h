#pragma once

#include "Object.h"

inline float clamp(float value, float min, float max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

class PhysicObject : public Object
{
public:
	PhysicObject
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
		float angle
	) : Object(sprite, x, y, width, height), velocity(velocity), maxVelocity(maxVelocity),  acceleration(acceleration), direction(direction), angle(angle) {}

	void render(Tmpl8::Surface* screen) override
	{
		sprite->DrawScaledRotated(x, y, width, height, angle, screen);
	}

	virtual void move(float deltaTime)
	{
		velocity += acceleration * deltaTime;

		velocity.x = clamp(velocity.x, -maxVelocity.x, maxVelocity.x);
		velocity.y = clamp(velocity.y, -maxVelocity.y, maxVelocity.y);
	}


	virtual void stop(float deltaTime)
	{
		velocity.x -= velocity.x * deltaTime;
		velocity.y -= velocity.y * deltaTime;

		if (std::abs(velocity.x) <= 0.01f) velocity.x = 0.f;
		if (std::abs(velocity.y) <= 0.01f) velocity.y = 0.f;
	}
protected:
	float angle;

	Tmpl8::vec2 velocity;
	Tmpl8::vec2 maxVelocity;
	Tmpl8::vec2 acceleration;
	Tmpl8::vec2 direction;
};