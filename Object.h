#pragma once

#include "template.h"
#include "surface.h"
#include <iostream>

class Object
{
public:
	Object(Tmpl8::Sprite* sprite, float x, float y, int width, int height) : sprite(sprite), x(x), y(y), width(width), height(height) {}

	virtual void update(float deltaTime) = 0; // Pure virtual.
	virtual void render(Tmpl8::Surface& screen) { sprite->DrawScaled(static_cast<int>(x), static_cast<int>(y), width, height, &screen); }

	const Tmpl8::vec2 getPosition() const
	{
		return { x, y };
	}

	const Tmpl8::vec2i getSize() const
	{
		return { width, height };
	}

protected:
	Tmpl8::Sprite* sprite;
	float x, y;
	int width, height;
};

