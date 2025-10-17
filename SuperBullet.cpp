#include "SuperBullet.h"

SuperBullet::SuperBullet
(
	std::shared_ptr<Tmpl8::Sprite> sprite,
	Tmpl8::vec2 position,
	Tmpl8::vec2 size,
	float areaMultiplier
) :
	IBullet(nullptr, position, size), deathTime(3.f), deathTimer(deathTime), bulletSprite(sprite), bulletWidth(size.x / areaMultiplier), bulletHeight(size.y / areaMultiplier), areaMultiplier(areaMultiplier)
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
	//float posx = position.x + size.x / 2.f - bulletWidth / 2.f;
	//float posy = position.y + size.y / 2.f - bulletHeight / 2.f;
	
	//Set a width of the are to be a width of a bullet
	//To properly define axis
	//size.x = static_cast<int>(bulletWidth);
	//size.y = static_cast<int>(bulletHeight);
	
	//Render at 4 corners
	//renderAt(*bulletSprite.get(), { posx, posy }, screen);

	/*renderAt(*bulletSprite.get(), fmodf(posx + bulletWidth + ScreenWidth, ScreenWidth) - bulletWidth, fmodf(posy + bulletHeight + ScreenHeight, ScreenHeight) - bulletHeight, screen);
	renderAt(*bulletSprite.get(), fmodf(posx + bulletWidth + ScreenWidth, ScreenWidth) - bulletWidth, fmodf(posy + ScreenHeight, ScreenHeight), screen);
	renderAt(*bulletSprite.get(), fmodf(posx + ScreenWidth, ScreenWidth), fmodf(posy + bulletHeight + ScreenHeight, ScreenHeight) - bulletHeight, screen);*/

	//Return to area width
	//size.x = static_cast<int>(bulletWidth * areaMultiplier);
	//size.y = static_cast<int>(bulletHeight * areaMultiplier);
}

void SuperBullet::onCollisionEnter(std::shared_ptr<Object> object)
{
	if (object->getTag() == "asteroid")
	{
		
	}
}
