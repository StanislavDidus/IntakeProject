#pragma once

#include "Object.hpp"
#include "FillBar.hpp"

class Asteroid : public Object
{
public:
	Asteroid
	(
		std::shared_ptr<Tmpl8::Sprite> sprite,
		Tmpl8::vec2 position,
		Tmpl8::vec2 size,
		float maxHealth,
		int frameIndex
	);
	~Asteroid() override;

	void update(float deltaTime) override;
	void render(Tmpl8::Surface& screen) override;

	void onCollisionEnter(std::shared_ptr<Object> object) override;
	void onCollisionStay(std::shared_ptr<Object> object, float deltaTime) override;
	void onCollisionExit(std::shared_ptr<Object> object) override;

	bool divide = false;
	bool turnToSheep = false;
private:
	int frameIndex = 0;

	std::unique_ptr<FillBar> hpBar;
	int barWidth = 75;
	int barHeight = 5;

	float rotationSpeed = 10.f;
	float maxHealth = 4;
	float currentHealth = 4;
};

