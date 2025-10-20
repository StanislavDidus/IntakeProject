#include "Trail.hpp"
#include "CollisionHelper.hpp"

Trail::Trail(std::shared_ptr<Tmpl8::Sprite> sprite, Tmpl8::vec2 position, Tmpl8::vec2 size, std::shared_ptr<Object> asteroid, Tmpl8::vec2 bulletSize) : Object(sprite, position, size), asteroid(asteroid),
bulletSize(bulletSize)
{
	center = { size.x / 2.f, size.y };
}

Trail::~Trail()
{
}

void Trail::update(float deltaTime)
{
	Tmpl8::vec2 asteroidPosition = asteroid->getPosition();
	Tmpl8::vec2 asteroidSize = asteroid->getSize();
	Tmpl8::vec2 centerPosition = { asteroidPosition.x + asteroidSize.x / 2.f, asteroidPosition.y + asteroidSize.y / 2.f };
	
	Tmpl8::vec2 direction = centerPosition - Tmpl8::vec2{ position.x + bulletSize.x / 2.f, position.y + bulletSize.y / 2.f };
	float distance = direction.length();

	size.y = distance;

	direction.normalize();

	center = { size.x / 2.f, size.y };

	float centerX = (position + center).x;
	float centerY = (position + center).y;

	float radians = std::asin((direction.x - centerX) / (direction.y - centerY));
	//float radians = std::atan2(direction.x - centerX, direction.y - centerY);
	angle = radians * 180.f / Tmpl8::PI;
}

void Trail::render(Tmpl8::Surface& screen)
{
	auto v = getVertices();
	sprite->DrawScaledRotated(v[0], v[1], v[2], v[3], screen);
}
