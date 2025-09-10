#pragma once

#include "template.h"
#include "surface.h"
#include <iostream>
#include <algorithm>

#include "Object.h"

class Player : public Object
{
public:
	Player(Tmpl8::Sprite* sprite, float x, float y, int width, int height) : Object(sprite, x, y, width, height),
		angle(0.f), rotationSpeed(1.f), direction{ 0.f, -1.f }, velocity{ 0.f, 0.f }, maxVelocity{ 150.f, 150.f }, acceleration{550.f, 550.f} {
	}

	void update(float deltaTime) override;
	void render(Tmpl8::Surface* screen) override;

	void move(float deltaTime);
	void stop(float deltaTime);
	void rotate(float angle);
private:
	float angle;
	float rotationSpeed;

	Tmpl8::vec2 velocity;
	Tmpl8::vec2 maxVelocity;
	Tmpl8::vec2 acceleration;
	//Tmpl8::vec2 friction;
	Tmpl8::vec2 direction;
};

