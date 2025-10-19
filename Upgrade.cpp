#include "Upgrade.hpp"

Upgrade::Upgrade(std::shared_ptr<Tmpl8::Sprite> sprite, Tmpl8::vec2 position, Tmpl8::vec2 size) :
	Object(sprite, position, size), rotationSpeed(50.f)
{
}

void Upgrade::update(float deltaTime)
{
	angle += rotationSpeed * deltaTime;
	angle = fmodf(angle, 360.f);
}
