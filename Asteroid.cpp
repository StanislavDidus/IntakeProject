#include "Asteroid.h"
#include "CollisionManager.h"

Asteroid::Asteroid
(
	Tmpl8::Sprite* sprite,
	float x,
	float y,
	int width,
	int height,
	Tmpl8::vec2 velocity,
	Tmpl8::vec2 maxVelocity,
	Tmpl8::vec2 acceleration,
	Tmpl8::vec2 direction,
	int maxHealth
) : Object(sprite, x, y, width, height, velocity, maxVelocity, acceleration, direction, 0.f, "asteroid"), rotationSpeed(10.f), maxHealth(maxHealth), currentHealth(maxHealth), barWidth(75), barHeight(height)
{
	hpBar = std::make_unique<FillBar>(Tmpl8::GreenMask, x , y, barWidth, 5, 0, maxHealth);
}

void Asteroid::update(float deltaTime)
{
	angle += rotationSpeed * deltaTime;

	move(deltaTime);

	x += velocity.x * direction.x * deltaTime;
	y += velocity.y * direction.y * deltaTime;

	hpBar->setPosition({x + static_cast<float>(width) / 2.f - static_cast<float>(barWidth) / 2.f, y - 5.f});

	//if (x >= ScreenWidth) x = 0;
	//if (y >= ScreenHeight) y = 0;

	//if (x <= static_cast<float>(-width)) x = ScreenWidth - static_cast<float>(width);
	//if (y <= static_cast<float>(-height)) y = ScreenHeight - static_cast<float>(height);
}

void Asteroid::render(Tmpl8::Surface& screen)
{
	sprite->DrawScaledRotated(x, y, width, height, angle, &screen);
	hpBar->render(screen, currentHealth);

	//sprite->DrawScaledRotated(fmodf(x + width + ScreenWidth, ScreenWidth) - width, fmodf(y + height + ScreenHeight, ScreenHeight) - height, width, height, angle, &screen);
	//sprite->DrawScaledRotated(fmodf(x + width + ScreenWidth, ScreenWidth) - width, fmodf(y + ScreenHeight, ScreenHeight), width, height, angle, &screen);
	//sprite->DrawScaledRotated(fmodf(x + ScreenWidth, ScreenWidth), fmodf(y + height + ScreenHeight, ScreenHeight) - height, width, height, angle, &screen);
}

void Asteroid::onCollisionEnter(std::shared_ptr<Object> object)
{
	if (object->getTag() == "bullet")
	{
		currentHealth--;

		if (currentHealth <= 0)
		{
			destroy = true;
		}
	}
}
