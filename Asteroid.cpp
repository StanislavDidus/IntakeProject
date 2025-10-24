#include "Asteroid.hpp"
#include "CollisionManager.hpp"
#include "CollisionHelper.hpp"
#include <Audio/Device.hpp>
#include <sstream>

Asteroid::Asteroid
(
	std::shared_ptr<Tmpl8::Sprite> sprite,
	Tmpl8::vec2 position,
	Tmpl8::vec2 size,
	float maxHealth
) : Object(sprite, position, size), rotationSpeed(10.f), maxHealth(maxHealth), currentHealth(maxHealth), barWidth(75),
	barHeight(5), divide(false), turnToSheep(false)
{
	hpBar = std::make_unique<FillBar>(Tmpl8::GreenMask, position, Tmpl8::vec2{static_cast<float>(barWidth), 5.f}, 0.f, maxHealth);
}

void Asteroid::update(float deltaTime)
{
	angle += rotationSpeed * deltaTime;	

	move(deltaTime);

	applyVelocity(deltaTime);

	hpBar->setPosition({position.x + size.x / 2.f - static_cast<float>(barWidth) / 2.f, position.y - 5.f});

	//if (x >= ScreenWidth) x = 0;
	//if (y >= ScreenHeight) y = 0;

	//if (x <= static_cast<float>(-width)) x = ScreenWidth - static_cast<float>(width);
	//if (y <= static_cast<float>(-height)) y = ScreenHeight - static_cast<float>(height);
}

void Asteroid::render(Tmpl8::Surface& screen)
{
	//sprite->DrawScaledRotated(x, y, width, height, angle, &screen);
	std::vector<Vertex> v = getVertices();
	sprite->DrawScaledRotated(v[0], v[1], v[2], v[3], screen);

	hpBar->render(screen, currentHealth);

#ifdef _DEBUG
	std::stringstream ssPos, ssSize, ssHealth;
	ssPos << "Pos:" << static_cast<int>(position.x) << ", " << static_cast<int>(position.y);
	ssSize << "Size:" << static_cast<int>(size.x) << ", " << static_cast<int>(size.y);
	ssHealth << "Heath: " << currentHealth << "/" << maxHealth;

	screen.PrintScaled(&ssPos.str()[0], position.x, position.y - 60, 2, 2, 0xFFFFFF);
	screen.PrintScaled(&ssSize.str()[0], position.x, position.y - 40, 2, 2, 0xFFFFFF);
	screen.PrintScaled(&ssHealth.str()[0], position.x, position.y - 20, 2, 2, 0xFFFFFF);
#endif 

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

void Asteroid::onCollisionExit(std::shared_ptr<Object> object)
{
	
}
