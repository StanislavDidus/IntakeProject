#pragma once

#include "Object.h"

class Sheep : public Object
{
public:
	Sheep
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
		float angle
	);

	virtual void update(float deltaTime);
	virtual void render(Tmpl8::Surface& screen);
private:
	float rotationSpeed;
};

