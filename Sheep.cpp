#include "Sheep.h"

Sheep::Sheep(std::shared_ptr<Tmpl8::Sprite> sprite, float x, float y, int width, int height, Tmpl8::vec2 velocity, float maxSpeed, Tmpl8::vec2 acceleration, Tmpl8::vec2 direction, float angle) :
	Object(sprite, x, y, width, height, velocity, maxSpeed, acceleration, direction, angle, "sheep"), rotationSpeed(50.f)
{

}

void Sheep::update(float deltaTime)
{
	angle -= rotationSpeed * deltaTime;
	this->angle = fmodf(this->angle, 360.f);

	move(deltaTime);

	applyVelocity(deltaTime);
}

void Sheep::render(Tmpl8::Surface& screen)
{
	sprite->DrawScaledRotated(*this, screen);
}
