#pragma once

#include "template.h"
#include "surface.h"
#include <iostream>
#include <algorithm>
#include <vector>

#include "Object.h"
#include "Animator.h"
#include "Bullet.h"

class Player : public Object
{
public:
	Player
	(
		Tmpl8::Sprite* playerSprite,
		Tmpl8::Sprite* engineSprite,
		Tmpl8::Sprite* engineEffectSprite,
		Tmpl8::Sprite* weaponSprite,
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

	const std::vector<std::shared_ptr<Bullet>>& getBullets() const;

	void rotate(float angle, float deltaTime);

	void shoot();

	void onCollisionEnter(std::shared_ptr<Object> object) override;
	void onCollisionStay(std::shared_ptr<Object> object) override;
	void onCollisionExit(std::shared_ptr<Object> object) override;
private:
	void initAnimator();

	void updateBullets(float deltaTime);

	void renderShipPart(Tmpl8::Sprite& sprite, Tmpl8::Surface& screen);

	std::vector<std::shared_ptr<Bullet>> bullets;

	Tmpl8::Sprite* engineSprite;
	Tmpl8::Sprite* engineEffectSprite;
	Tmpl8::Sprite* weaponSprite;

	Tmpl8::Sprite* bulletSprite;

	std::unique_ptr<Animator> animator;

	float shootSpeed;
	float shootTimer;
	bool shootLeft;

	float rotationSpeed;
};

