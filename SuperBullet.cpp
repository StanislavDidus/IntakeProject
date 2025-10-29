#include "SuperBullet.hpp"
#include "CollisionHelper.hpp"

SuperBullet::SuperBullet
(
	std::shared_ptr<Tmpl8::Sprite> sprite,
	std::shared_ptr<Tmpl8::Sprite> trailSprite,
	Tmpl8::vec2 position,
	Tmpl8::vec2 size,
	float areaMultiplier
) :
	IBullet(nullptr, position, size), trailSprite(trailSprite), deathTime(3.f), deathTimer(deathTime), bulletSprite(sprite), areaMultiplier(areaMultiplier)
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

	for (const auto& trail : trails)
	{
		resize(1.f / areaMultiplier);

		trail->setPosition(position);

		resize(areaMultiplier);
		
		trail->update(deltaTime);
	}
}

void SuperBullet::render(Tmpl8::Surface& screen)
{	
	bulletSprite->SetFrame(animator->getAnimationFrame(bulletSprite));
	
	resize(1.f / areaMultiplier);

	center = { size.x / 2.f, size.y / 2.f };

	std::vector<Vertex> v = getVertices(bulletSprite, position);
	bulletSprite->DrawScaledRotated(v[0], v[1], v[2], v[3], screen);

	resize(areaMultiplier);

	center = { size.x / 2.f, size.y / 2.f };

	for (const auto& trail : trails)
	{
		trail->render(screen);
	}
}

void SuperBullet::onCollisionEnter(std::shared_ptr<Object> object)
{
	if (object->getTag() == "asteroid")
	{
		resize(1.f / areaMultiplier);

		std::shared_ptr<Trail> trail = std::make_shared<Trail>(trailSprite, position, size, object);

		resize(areaMultiplier);

		trails.push_back(trail);
	}
}

void SuperBullet::onCollisionExit(std::shared_ptr<Object> object)
{
	if (object->getTag() == "asteroid")
	{
		for (auto it = trails.begin(); it != trails.end();)
		{
			auto& trail = *it;
			if (object == trail->asteroid)
			{
				it = trails.erase(it);
			}
			else
				++it;
		}
	}
}
