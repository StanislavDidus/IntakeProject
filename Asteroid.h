#pragma once

#include "PhysicObject.h"

class Asteroid : public PhysicObject
{
public:
	Asteroid
	(
		Tmpl8::Sprite* sprite,
		float x,
		float y,
		int width,
		int height,
		Tmpl8::vec2 velocity,
		Tmpl8::vec2 maxVelocity,
		Tmpl8::vec2 acceleration,
		Tmpl8::vec2 direction
	) : PhysicObject(sprite, x, y, width, height, velocity, maxVelocity, acceleration, direction, 0.f, "asteroid"), rotationSpeed(10.f) {
	}

	void update(float deltaTime) override;
	void render(Tmpl8::Surface& screen) override;
private:
	float rotationSpeed;
};

