#pragma once

#include "Bullet.hpp"

class SuperBullet : public IBullet
{
public:
	SuperBullet
	(
		std::shared_ptr<Tmpl8::Sprite> sprite,
		Tmpl8::vec2 position,
		Tmpl8::vec2 size,
		float areaMultiplier
	);

	virtual ~SuperBullet();

	void update(float deltaTime) override;
	void render(Tmpl8::Surface& screen) override;

	void onCollisionEnter(std::shared_ptr<Object> object) override;
protected:
	void initAnimator();

	std::shared_ptr<Tmpl8::Sprite> bulletSprite;
	float areaMultiplier;
	float deathTime, deathTimer;
};

