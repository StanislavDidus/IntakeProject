#pragma once

#include "Object.h"
#include <vector>

class PhysicObject : public Object
{
public:
	PhysicObject
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
		float angle,
		const std::string& tag
	);

	virtual ~PhysicObject();

	void render(Tmpl8::Surface& screen) override;

	virtual void move(float deltaTime);

	virtual void stop(float deltaTime);

protected:
	Tmpl8::vec2 velocity;
	Tmpl8::vec2 maxVelocity;
	Tmpl8::vec2 acceleration;
	Tmpl8::vec2 direction;
};