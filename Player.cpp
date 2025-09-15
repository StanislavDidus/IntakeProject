#include "Player.h"
#include <cmath>
#include <algorithm>
#include <iostream>

void Player::update(float deltaTime)
{
	shootTimer += deltaTime;
	
	x += velocity.x * direction.x * deltaTime;
	y += velocity.y * direction.y * deltaTime;

	if (x >= 800) x = 0;
	if (y >= 512) y = 0;

	if (x <= static_cast<float>(-width)) x = 800 - static_cast<float>(width);
	if (y <= static_cast<float>(-height)) y = 512 - static_cast<float>(height);

	updateBullets(deltaTime);
}

void Player::updateBullets(float deltaTime)
{
	for (const auto& bullet : bullets)
	{
		bullet->update(deltaTime);
	}

	for (auto it = bullets.begin(); it != bullets.end(); )
	{
		auto& bullet = *it;

		auto& pos = bullet->getPosition();
		auto& size = bullet->getSize();
		
		if (pos.x + size.x < 0.f || pos.x >= 800.f || pos.y + size.y < 0.f || pos.y >= 512.f)
		{
			it = bullets.erase(it);
		}
		else
			++it;
	}
}

void Player::render(Tmpl8::Surface* screen)
{
	for (const auto& bullet : bullets)
		bullet->render(screen);
	
	sprite->DrawScaledRotated(x, y, width, height, angle, screen);
	sprite->DrawScaledRotated(fmodf(x + ScreenWidth, ScreenWidth), fmodf(y + ScreenHeight, ScreenHeight), width, height, angle, screen);
	sprite->DrawScaledRotated(fmodf(x + width + ScreenWidth, ScreenWidth) - width, fmodf(y + height + ScreenHeight, ScreenHeight) - height, width, height, angle, screen);
}
void Player::rotate(float angle)
{
	this->angle += angle * rotationSpeed;
	this->angle = fmodf(this->angle, 360.f);

	float radians = this->angle * 3.1415f / 180.f;
	float sin = std::sin(radians);
	float cos = std::cos(radians);

	direction.x = sin;
	direction.y = -cos;

	direction.normalize();
}

void Player::shoot()
{
	if (shootTimer < shootSpeed)
		return;
	shootTimer = 0.f;

	auto bullet = std::make_shared<Bullet>
		(
			bulletSprite,
			x,
			y,
			width,
			height,
			Tmpl8::vec2{ 0.f, 0.f },
			Tmpl8::vec2{ 1500.f, 1500.f },
			Tmpl8::vec2{ 5000.f, 5000.f },
			direction,
			angle
		);

	bullets.push_back(bullet);
}