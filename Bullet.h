#pragma once

#include "PhysicObject.h"

class Bullet : public PhysicObject
{
public:
	Bullet
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
	) : PhysicObject(sprite, x, y, width, height, velocity, maxVelocity, acceleration, direction, angle) {
	}

	void update(float deltaTime) override
	{
		move(deltaTime);

		x += velocity.x * direction.x * deltaTime;
		y += velocity.y * direction.y * deltaTime;
	}

private:
	
};
	
