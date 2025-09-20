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
	);

	void update(float deltaTime) override;
	void render(Tmpl8::Surface& screen) override;

	void rotate(float angle, float deltaTime);

	void shoot();

	void onCollisionEnter(const CollisionEvent& event) override;
	void onCollisionStay(const CollisionEvent& event) override;
	void onCollisionExit(const CollisionEvent& event) override;
private:
	void updateBullets(float deltaTime);

	Tmpl8::Sprite* bulletSprite;

	float shootSpeed;
	float shootTimer;
	float rotationSpeed;

	std::vector<std::shared_ptr<Bullet>> bullets;
};

