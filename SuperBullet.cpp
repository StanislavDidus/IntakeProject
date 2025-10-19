#include "SuperBullet.hpp"
#include "CollisionHelper.hpp"

SuperBullet::SuperBullet
(
	std::shared_ptr<Tmpl8::Sprite> sprite,
	Tmpl8::vec2 position,
	Tmpl8::vec2 size,
	float areaMultiplier
) :
	IBullet(nullptr, position, size), deathTime(3.f), deathTimer(deathTime), bulletSprite(sprite), areaMultiplier(areaMultiplier)
{
	checkPixelPerfectCollision = false;

	initAnimator();
}

SuperBullet::~SuperBullet()
{
}

void SuperBullet::initAnimator()
{
	animator = std::make_unique<Animator>();
	animator->addFrameAnimation(bulletSprite, 0.1f, 0, bulletSprite->Frames() - 1, []() {return true; });
}


void SuperBullet::update(float deltaTime)
{
	deathTimer -= deltaTime;

	if (deathTimer <= 0.f) destroy = true;
	
	move(deltaTime);

	applyVelocity(deltaTime);

	//x = fmodf(x + ScreenWidth, ScreenWidth);
	//y = fmodf(y + ScreenHeight, ScreenHeight);

	stop(deltaTime);

	animator->update(deltaTime);
}

void SuperBullet::render(Tmpl8::Surface& screen)
{	
	resize(1.f / areaMultiplier);

	std::vector<Vertex> v = getVertices(bulletSprite, position);
	bulletSprite->DrawScaledRotated(v[0], v[1], v[2], v[3], screen);

	resize(areaMultiplier);
}

void SuperBullet::onCollisionEnter(std::shared_ptr<Object> object)
{
	if (object->getTag() == "asteroid")
	{
		
	}
}
