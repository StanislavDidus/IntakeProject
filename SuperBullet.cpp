#include "SuperBullet.h"

SuperBullet::SuperBullet
(
	std::shared_ptr<Tmpl8::Sprite> sprite,
	float x,
	float y,
	int width,
	int height,
	float areaMultiplier,
	Tmpl8::vec2 velocity,
	float maxSpeed,
	Tmpl8::vec2 acceleration,
	Tmpl8::vec2 direction,
	float angle
) :
	IBullet(nullptr, x, y, width, height, velocity, maxSpeed, acceleration, direction, angle, "superBullet"), deathTime(3.f), deathTimer(deathTime), bulletSprite(sprite), bulletWidth(width / areaMultiplier), bulletHeight(height / areaMultiplier), areaMultiplier(areaMultiplier)
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
	//Find a middle position inside a 'damage' area to spawn a bullet
	float posx = x + width / 2.f - bulletWidth / 2.f;
	float posy = y + height / 2.f - bulletHeight / 2.f;
	
	//Set a width of the are to be a width of a bullet
	//To properly define axis
	width = static_cast<int>(bulletWidth);
	height = static_cast<int>(bulletHeight);
	
	//Render at 4 corners
	renderAt(*bulletSprite.get(), posx, posy, screen);

	/*renderAt(*bulletSprite.get(), fmodf(posx + bulletWidth + ScreenWidth, ScreenWidth) - bulletWidth, fmodf(posy + bulletHeight + ScreenHeight, ScreenHeight) - bulletHeight, screen);
	renderAt(*bulletSprite.get(), fmodf(posx + bulletWidth + ScreenWidth, ScreenWidth) - bulletWidth, fmodf(posy + ScreenHeight, ScreenHeight), screen);
	renderAt(*bulletSprite.get(), fmodf(posx + ScreenWidth, ScreenWidth), fmodf(posy + bulletHeight + ScreenHeight, ScreenHeight) - bulletHeight, screen);*/

	//Return to area width
	width = static_cast<int>(bulletWidth * areaMultiplier);
	height = static_cast<int>(bulletHeight * areaMultiplier);
}

void SuperBullet::onCollisionEnter(std::shared_ptr<Object> object)
{
	if (object->getTag() == "asteroid")
	{
		
	}
}
