#pragma once

#include "Object.h"
#include "FillBar.h"

class Asteroid : public Object
{
public:
	Asteroid
	(
		std::shared_ptr<Tmpl8::Sprite> sprite,
		Tmpl8::vec2 position,
		Tmpl8::vec2 size,
		float maxHealth
	);

	void update(float deltaTime) override;
	void render(Tmpl8::Surface& screen) override;

	void onCollisionEnter(std::shared_ptr<Object> object) override;
	void onCollisionStay(std::shared_ptr<Object> object, float deltaTime) override;

	bool divide;
	bool turnToSheep;
private:
	std::unique_ptr<FillBar> hpBar;
	int barWidth, barHeight;

	float rotationSpeed;
	float maxHealth, currentHealth;
};

