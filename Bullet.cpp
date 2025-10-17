#include "Bullet.h"
#include "CollisionManager.h"
#include "Audio/Device.hpp"

Bullet::Bullet(std::shared_ptr<Tmpl8::Sprite> sprite, Tmpl8::vec2 position, Tmpl8::vec2 size) :
	IBullet(sprite, position, size)
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

	applyVelocity(deltaTime);

	animator->update(deltaTime);
}	

void Bullet::onCollisionEnter(std::shared_ptr<Object> object)
{
	if (object->getTag() == "asteroid")
	{
		Audio::Device::playSound("assets/Sounds/hitEffect.mp3");
		
		destroy = true;
	}
}


