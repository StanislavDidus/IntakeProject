#pragma once

#include "template.h"
#include "surface.h"
#include <iostream>

class Object
{
public:
	Object(Tmpl8::Sprite* sprite, float x, float y, int width, int height) : sprite(sprite), x(x), y(y), width(width), height(height) {}

	virtual void update(float deltaTime) {}
	virtual void render(Tmpl8::Surface* screen) { sprite->DrawScaled(static_cast<int>(x), static_cast<int>(y), width, height, screen); }
protected:
	Tmpl8::Sprite* sprite;
	float x, y;
	int width, height;
};

