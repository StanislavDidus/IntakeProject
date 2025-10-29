#pragma once

#include "Object.hpp"
#include "Animator.hpp"

class IBullet : public Object
{
public:
	IBullet
	(
		std::shared_ptr<Tmpl8::Sprite> sprite,
		Tmpl8::vec2 position,
		Tmpl8::vec2 size
	) : Object(sprite, position, size)
	{

	}

	void update(float deltaTime) override
	{
		move(deltaTime);

		applyVelocity(deltaTime);
	}

protected:
	std::unique_ptr<Animator> animator;
};

class Bullet : public IBullet
{
public:
	Bullet
	(
		std::shared_ptr<Tmpl8::Sprite> sprite,
		Tmpl8::vec2 position,
		Tmpl8::vec2 size
	);

	void update(float deltaTime) override;
	void render(Tmpl8::Surface& screen) override;

	void onCollisionEnter(std::shared_ptr<Object> object) override;

protected:
	void initAnimator();
};
	
