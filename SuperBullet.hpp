#pragma once

#include "Bullet.hpp"
#include "Trail.hpp"

class SuperBullet : public IBullet
{
public:
	SuperBullet
	(
		std::shared_ptr<Tmpl8::Sprite> sprite,
		std::shared_ptr<Tmpl8::Sprite> trailSprite,
		Tmpl8::vec2 position,
		Tmpl8::vec2 size,
		float areaMultiplier
	);

	virtual ~SuperBullet();

	void update(float deltaTime) override;
	void render(Tmpl8::Surface& screen) override;

	void onCollisionEnter(std::shared_ptr<Object> object) override;
	void onCollisionExit(std::shared_ptr<Object> object) override;
protected:
	void initAnimator();

	std::vector<std::shared_ptr<Trail>> trails;

	std::shared_ptr<Tmpl8::Sprite> bulletSprite;
	std::shared_ptr<Tmpl8::Sprite> trailSprite;
	float areaMultiplier;
	float deathTime, deathTimer;
};

