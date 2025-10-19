#pragma once

#include "Object.hpp"

class Upgrade : public Object
{
public:
	Upgrade(std::shared_ptr<Tmpl8::Sprite> sprite,
		Tmpl8::vec2 position,
		Tmpl8::vec2 size
	);

	void update(float deltaTime) override;
private:
	float rotationSpeed;
};

