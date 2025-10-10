#pragma once

#include "Bullet.h"

class SuperBullet : public Bullet
{
public:
	SuperBullet
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
		float angle
	);

	virtual ~SuperBullet();

	void update(float deltaTime) override;
	void render(Tmpl8::Surface& screen) override;

	void onCollisionEnter(std::shared_ptr<Object> object) override;
protected:
	float deathTime, deathTimer;
};

