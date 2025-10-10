#include "Upgrade.h"

Upgrade::Upgrade(std::shared_ptr<Tmpl8::Sprite> sprite, float x, float y, int width, int height, Tmpl8::vec2 velocity, float maxSpeed, Tmpl8::vec2 acceleration, Tmpl8::vec2 direction, float angle) :
	Object(sprite, x, y, width, height, velocity, maxSpeed, acceleration, direction, angle, "upgrade"), rotationSpeed(50.f)
{
}

void Upgrade::update(float deltaTime)
{
	angle += rotationSpeed * deltaTime;
	angle = fmodf(angle, 360.f);
}
