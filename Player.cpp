#include "Player.h"
#include <cmath>
#include <algorithm>
#include <iostream>

Player::Player
(
	Tmpl8::Sprite* sprite,
	Tmpl8::Sprite* bulletSprite,
	float x,
	float y,
	int width,
	int height,
	Tmpl8::vec2 velocity,
	Tmpl8::vec2 maxVelocity,
	Tmpl8::vec2 acceleration,
	Tmpl8::vec2 direction
) : PhysicObject(sprite, x, y, width, height, velocity, maxVelocity, acceleration, direction, 0.f, "player"),
	rotationSpeed(200.f),
	bulletSprite(bulletSprite),
	shootSpeed(0.2f),
	shootTimer(0.f) 
{

}

void Player::update(float deltaTime)
{
	shootTimer += deltaTime;
	
	x += velocity.x * direction.x * deltaTime;
	y += velocity.y * direction.y * deltaTime;

	/*if (x >= ScreenWidth) x = 0;
	if (y >= ScreenHeight) y = 0;

	if (x <= static_cast<float>(-width)) x = ScreenWidth - static_cast<float>(width);
	if (y <= static_cast<float>(-height)) y = ScreenWidth - static_cast<float>(height);*/

	x = fmodf(x + ScreenWidth, ScreenWidth);
	y = fmodf(y + ScreenHeight, ScreenHeight);

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

		if(bullet->destroy)
		{
			it = bullets.erase(it);
		}
		else
			++it;
	}
}

void Player::render(Tmpl8::Surface& screen)
{
	for (const auto& bullet : bullets)
		bullet->render(screen);
	
	sprite->DrawScaledRotated(x, y, width, height, angle, &screen); //Draw main ship

	sprite->DrawScaledRotated(fmodf(x + width + ScreenWidth, ScreenWidth) - width, fmodf(y + height + ScreenHeight, ScreenHeight) - height, width, height, angle, &screen);
	sprite->DrawScaledRotated(fmodf(x + width + ScreenWidth, ScreenWidth) - width, fmodf(y + ScreenHeight, ScreenHeight), width, height, angle, &screen);
	sprite->DrawScaledRotated(fmodf(x + ScreenWidth, ScreenWidth), fmodf(y + height + ScreenHeight, ScreenHeight) - height, width, height, angle, &screen);
}

void Player::rotate(float angle, float deltaTime)
{
	this->angle += angle * rotationSpeed * deltaTime;
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

void Player::onCollisionEnter(const CollisionEvent& event)
{
	//std::cout << "enter\n";
}

void Player::onCollisionStay(const CollisionEvent& event)
{
	//std::cout << "stay\n";
}

void Player::onCollisionExit(const CollisionEvent& event)
{
	//std::cout << "exit\n";
}
