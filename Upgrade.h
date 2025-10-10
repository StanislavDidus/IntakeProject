#pragma once

#include "Object.h"

class Upgrade : public Object
{
public:
	Upgrade(std::shared_ptr<Tmpl8::Sprite> sprite,
		float x,
		float y,
		int width,
		int height,
		Tmpl8::vec2 velocity,
		float maxSpeed,
		Tmpl8::vec2 acceleration,
		Tmpl8::vec2 direction,
		float angle = 0.f
	);

	void update(float deltaTime) override;
private:
	float rotationSpeed;
};

