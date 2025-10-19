#pragma once

#include "Object.hpp"

class Sheep : public Object
{
public:
	Sheep
	(
		std::shared_ptr<Tmpl8::Sprite> sprite,
		Tmpl8::vec2 position,
		Tmpl8::vec2 size
	);

	virtual void update(float deltaTime);
	virtual void render(Tmpl8::Surface& screen);
private:
	float rotationSpeed;
};

