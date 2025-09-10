#include "Player.h"
#include <cmath>

void Player::update(float deltaTime)
{
	x += velocity.x * deltaTime;
	y += velocity.y * deltaTime;

	x = fmodf(x + ScreenWidth, ScreenWidth);
	y = fmodf(y + ScreenHeight, ScreenHeight);
	
}

void Player::render(Tmpl8::Surface* screen)
{
	sprite->DrawScaledRotated(x, y, width, height, angle ,screen);
//	sprite->DrawScaledRotated(fmodf( x + ScreenWidth, ScreenWidth ), fmodf(y + ScreenHeight, ScreenHeight), width, height, angle ,screen);
}

void Player::move(float deltaTime)
{
	velocity.x += direction.x * acceleration.x * deltaTime;
	velocity.y += direction.y * acceleration.y * deltaTime;

	velocity.x = std::min(std::max(-maxVelocity.x, velocity.x), maxVelocity.x);
	velocity.y = std::min(std::max(-maxVelocity.y, velocity.y), maxVelocity.y);
}

void Player::stop(float deltaTime)
{
	velocity.x -= velocity.x * deltaTime;
	velocity.y -= velocity.y * deltaTime;

	if (std::abs(velocity.x) <= 0.01f) velocity.x = 0.f;
	if (std::abs(velocity.y) <= 0.01f) velocity.y = 0.f;
}

void Player::rotate(float angle)
{
	this->angle += angle * rotationSpeed;

	float radians = angle * 3.1415f / 180.f;
	float sin = std::sin(radians);
	float cos = std::cos(radians);

	float oldX = direction.x;
	float oldY = direction.y;

	direction.x = oldX * cos - oldY * sin;
	direction.y = oldX * sin + oldY * cos;
}
