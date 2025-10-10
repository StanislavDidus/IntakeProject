#include "SuperBullet.h"

SuperBullet::SuperBullet(std::shared_ptr<Tmpl8::Sprite> sprite, float x, float y, int width, int height, Tmpl8::vec2 velocity, float maxSpeed, Tmpl8::vec2 acceleration, Tmpl8::vec2 direction, float angle) :
	Bullet(sprite, x, y, width, height, velocity, maxSpeed, acceleration, direction, angle), deathTime(3.f), deathTimer(deathTime)
{
	tag = "superBullet";
}

SuperBullet::~SuperBullet()
{
}


void SuperBullet::update(float deltaTime)
{
	deathTimer -= deltaTime;

	if (deathTimer <= 0.f) destroy = true;
	
	move(deltaTime);

	applyVelocity(deltaTime);

	x = fmodf(x + ScreenWidth, ScreenWidth);
	y = fmodf(y + ScreenHeight, ScreenHeight);

	stop(deltaTime);

	animator->update(deltaTime);
}

void SuperBullet::render(Tmpl8::Surface& screen)
{
	renderAt(*sprite.get(), x, y, screen);

	renderAt(*sprite.get(), fmodf(x + width + ScreenWidth, ScreenWidth) - width, fmodf(y + height + ScreenHeight, ScreenHeight) - height, screen);
	renderAt(*sprite.get(), fmodf(x + width + ScreenWidth, ScreenWidth) - width, fmodf(y + ScreenHeight, ScreenHeight), screen);
	renderAt(*sprite.get(), fmodf(x + ScreenWidth, ScreenWidth), fmodf(y + height + ScreenHeight, ScreenHeight) - height, screen);
}

void SuperBullet::onCollisionEnter(std::shared_ptr<Object> object)
{
	if (object->getTag() == "asteroid")
	{
		
	}
}
