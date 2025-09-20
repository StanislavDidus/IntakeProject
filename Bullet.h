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
	) : PhysicObject(sprite, x, y, width, height, velocity, maxVelocity, acceleration, direction, angle, "bullet") {
	}

	void update(float deltaTime) override
	{
		move(deltaTime);

		x += velocity.x * direction.x * deltaTime;
		y += velocity.y * direction.y * deltaTime;

		if (x + static_cast<float>(width) < 0.f || x >= 800.f || y + static_cast<float>(height) < 0.f || y >= 512.f)
			destroy = true;

	}

	void onCollisionEnter(const CollisionEvent& event) override;

private:
	
};
	
