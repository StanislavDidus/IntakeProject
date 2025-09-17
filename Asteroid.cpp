#include "Asteroid.h"

void Asteroid::update(float deltaTime)
{
	angle += rotationSpeed * deltaTime;

	move(deltaTime);

	x += velocity.x * direction.x * deltaTime;
	y += velocity.y * direction.y * deltaTime;

	//if (x >= ScreenWidth) x = 0;
	//if (y >= ScreenHeight) y = 0;

	//if (x <= static_cast<float>(-width)) x = ScreenWidth - static_cast<float>(width);
	//if (y <= static_cast<float>(-height)) y = ScreenHeight - static_cast<float>(height);
}

void Asteroid::render(Tmpl8::Surface& screen)
{
	sprite->DrawScaledRotated(x, y, width, height, angle, &screen);

	//sprite->DrawScaledRotated(fmodf(x + width + ScreenWidth, ScreenWidth) - width, fmodf(y + height + ScreenHeight, ScreenHeight) - height, width, height, angle, &screen);
	//sprite->DrawScaledRotated(fmodf(x + width + ScreenWidth, ScreenWidth) - width, fmodf(y + ScreenHeight, ScreenHeight), width, height, angle, &screen);
	//sprite->DrawScaledRotated(fmodf(x + ScreenWidth, ScreenWidth), fmodf(y + height + ScreenHeight, ScreenHeight) - height, width, height, angle, &screen);
}
