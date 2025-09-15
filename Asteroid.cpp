#include "Asteroid.h"

void Asteroid::update(float deltaTime)
{
	angle += rotationSpeed * deltaTime;

	move(deltaTime);

	x += velocity.x * direction.x * deltaTime;
	y += velocity.y * direction.y * deltaTime;
}
