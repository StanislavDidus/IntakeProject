#pragma once

#include "Object.h"
#include "FillBar.h"

class Asteroid : public Object
{
public:
	Asteroid
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
	);

	void update(float deltaTime) override;
	void render(Tmpl8::Surface& screen) override;

	void onCollisionEnter(std::shared_ptr<Object> object) override;
private:
	std::unique_ptr<FillBar> hpBar;
	int barWidth, barHeight;

	float rotationSpeed;
	int maxHealth, currentHealth;
};

