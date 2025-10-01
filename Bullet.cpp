#include "Bullet.h"
#include "CollisionManager.h"

Bullet::Bullet(Tmpl8::Sprite* sprite, float x, float y, int width, int height, Tmpl8::vec2 velocity, Tmpl8::vec2 maxVelocity, Tmpl8::vec2 acceleration, Tmpl8::vec2 direction, float angle) :
	Object(sprite, x, y, width, height, velocity, maxVelocity, acceleration, direction, angle, "bullet")
{
	initAnimator();
}

void Bullet::initAnimator()
{
	animator = std::make_unique<Animator>();
	animator->addFrameAnimation(sprite, 0.1f, 0, sprite->Frames() - 1, []() {return true; });
}

void Bullet::update(float deltaTime)
{
	move(deltaTime);

	x += velocity.x * direction.x * deltaTime;
	y += velocity.y * direction.y * deltaTime;

	animator->update(deltaTime);
}

void Bullet::onCollisionEnter(std::shared_ptr<Object> object)
{
	if (object->getTag() == "asteroid")
	{
		destroy = true;
	}
}


