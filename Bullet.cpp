#include "Bullet.hpp"
#include "CollisionManager.hpp"
#include "Audio/Device.hpp"
#include "CollisionHelper.hpp"

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
void Bullet::render(Tmpl8::Surface& screen)
{
	sprite->SetFrame(animator->getAnimationFrame(sprite));

	auto v = getVertices();
	sprite->DrawScaledRotated(v[0], v[1], v[2], v[3], screen);
}

void Bullet::onCollisionEnter(std::shared_ptr<Object> object)
{
	if (object->getTag() == "asteroid")
	{
		Audio::Device::playSound("assets/Sounds/hitEffect.mp3");
		
		destroy = true;
	}
}


