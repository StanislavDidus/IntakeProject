#pragma once

#include "template.h"
#include "surface.h"
#include <iostream>
#include <algorithm>
#include <vector>

#include "PhysicObject.h"
#include "Bullet.h"

class Player : public PhysicObject
{
public:
	Player
	(
		Tmpl8::Sprite* sprite,
		Tmpl8::Sprite* bulletSprite,
		float x,
		float y,
		int width,
		int height,
		Tmpl8::vec2 velocity,
		Tmpl8::vec2 maxVelocity,
		Tmpl8::vec2 acceleration,
		Tmpl8::vec2 direction
		) : PhysicObject(sprite, x, y, width, height, velocity, maxVelocity, acceleration, direction, 0.f), rotationSpeed(1.f), bulletSprite(bulletSprite), shootSpeed(0.2f), shootTimer(0.f) {
	}

	void update(float deltaTime) override;
	void render(Tmpl8::Surface* screen) override;

	void rotate(float angle);

	void shoot();
private:
	void updateBullets(float deltaTime);

	Tmpl8::Sprite* bulletSprite;

	float shootSpeed;
	float shootTimer;
	float rotationSpeed;

	std::vector<std::shared_ptr<Bullet>> bullets;
};

