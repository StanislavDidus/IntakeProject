#pragma once

#include "Object.h"
#include "Animator.h"

class Bullet : public Object
{
public:
	Bullet
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

	void update(float deltaTime) override;

	void onCollisionEnter(std::shared_ptr<Object> object) override;

protected:
	void initAnimator();

	std::unique_ptr<Animator> animator;
};
	
