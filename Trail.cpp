#include "Trail.hpp"
#include "CollisionHelper.hpp"

Trail::Trail(std::shared_ptr<Tmpl8::Sprite> sprite, Tmpl8::vec2 position, Tmpl8::vec2 size, std::shared_ptr<Object> asteroid) : Object(sprite, position, size), asteroid(asteroid)
{
	initAnimations();
}

Trail::~Trail()
{
}

void Trail::initAnimations()
{
	animator = std::make_unique<Animator>();

	animator->addFrameAnimation(sprite, 0.1f, 0, 3, [] {return true; });
}


void Trail::update(float deltaTime)
{
	animator->update(deltaTime);

	const auto& asteroidPosition = asteroid->getPosition();
	const auto& asteroidSize = asteroid->getSize();

	Tmpl8::vec2 asteroidCenterPosition = Tmpl8::vec2{ asteroidPosition.x + asteroidSize.x / 2.f, asteroidPosition.y + asteroidSize.y / 2.f };
	Tmpl8::vec2 bulletCenterPosition = Tmpl8::vec2{ position.x + size.x / 2.f, position.y + size.y / 2.f };

	auto direction = asteroidCenterPosition - bulletCenterPosition;
	float distance = direction.length();

	//Get a perpendicular to find 4 verticies
	Tmpl8::vec2 perp = (Tmpl8::vec2{ -direction.y , direction.x }).normalized();

	float halfThickness = size.x * 0.5f;

	Tmpl8::vec2 offset = perp * halfThickness;

	 p0 = asteroidCenterPosition - offset;
	 p1 = asteroidCenterPosition + offset;
	 p2 = bulletCenterPosition + offset;
	 p3 = bulletCenterPosition - offset;
}

void Trail::render(Tmpl8::Surface& screen)
{
	animator->setAnimationFrame(sprite);
	
	auto v = getVertices();

	//Substitute the position to a new one
	v[0].position = p1;
	v[1].position = p0;
	v[2].position = p3;
	v[3].position = p2;

	sprite->DrawScaledRotated(v[0], v[1], v[2], v[3], screen);
}

