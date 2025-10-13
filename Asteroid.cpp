#include "Asteroid.h"
#include "CollisionManager.h"
#include <Audio/Device.hpp>

Asteroid::Asteroid
(
	std::shared_ptr<Tmpl8::Sprite> sprite,
	float x,
	float y,
	int width,
	int height,
	Tmpl8::vec2 velocity,
	float maxSpeed,
	Tmpl8::vec2 acceleration,
	Tmpl8::vec2 direction,
	float maxHealth
) : Object(sprite, x, y, width, height, velocity, maxSpeed, acceleration, direction, 0.f, "asteroid"), rotationSpeed(10.f), maxHealth(maxHealth), currentHealth(maxHealth), barWidth(75),
	barHeight(height), divide(false), turnToSheep(false)
{
	hpBar = std::make_unique<FillBar>(Tmpl8::GreenMask, x , y, barWidth, 5, 0.f, maxHealth);
}

void Asteroid::update(float deltaTime)
{
	angle += rotationSpeed * deltaTime;	

	move(deltaTime);

	applyVelocity(deltaTime);

	hpBar->setPosition({x + static_cast<float>(width) / 2.f - static_cast<float>(barWidth) / 2.f, y - 5.f});

	//if (x >= ScreenWidth) x = 0;
	//if (y >= ScreenHeight) y = 0;

	//if (x <= static_cast<float>(-width)) x = ScreenWidth - static_cast<float>(width);
	//if (y <= static_cast<float>(-height)) y = ScreenHeight - static_cast<float>(height);
}

void Asteroid::render(Tmpl8::Surface& screen)
{
	//sprite->DrawScaledRotated(x, y, width, height, angle, &screen);
	sprite->DrawScaledRotated(*this, screen);

	hpBar->render(screen, currentHealth);

	//sprite->DrawScaledRotated(fmodf(x + width + ScreenWidth, ScreenWidth) - width, fmodf(y + height + ScreenHeight, ScreenHeight) - height, width, height, angle, &screen);
	//sprite->DrawScaledRotated(fmodf(x + width + ScreenWidth, ScreenWidth) - width, fmodf(y + ScreenHeight, ScreenHeight), width, height, angle, &screen);
	//sprite->DrawScaledRotated(fmodf(x + ScreenWidth, ScreenWidth), fmodf(y + height + ScreenHeight, ScreenHeight) - height, width, height, angle, &screen);
}

void Asteroid::onCollisionEnter(std::shared_ptr<Object> object)
{
	if (object->getTag() == "bullet")
	{
		currentHealth -= 1.f;

		if (currentHealth <= 0.f)
		{
			destroy = true;
			divide = true;

			Audio::Device::playSound("assets/Sounds/asteroidDestroyed.mp3");
		}
	}
	else if (object->getTag() == "sheep")
	{
		destroy = true;

		Audio::Device::playSound("assets/Sounds/asteroidDestroyed.mp3");

		turnToSheep = true;
	}
}

void Asteroid::onCollisionStay(std::shared_ptr<Object> object, float deltaTime)
{
	if (object->getTag() == "superBullet")
	{
		currentHealth -= 0.75f * deltaTime;

		
		

		if (currentHealth <= 0.f)
		{
			destroy = true;

			Audio::Device::playSound("assets/Sounds/asteroidDestroyed.mp3");

			turnToSheep = true;
		}
	}
}
