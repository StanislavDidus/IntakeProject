#pragma once

#include <memory>
#include "template.h"
#include "Animator.hpp"

class Particle
{
public:
	Particle(std::shared_ptr<Tmpl8::Sprite> sprite, const Tmpl8::vec2& position, const Tmpl8::vec2& size, float lastTime);
	virtual ~Particle();

	void update(float deltaTime);
	void render(Tmpl8::Surface& screen);

	void resize(float scale);

	bool destroy = false;
private:
	void initAnimator();

	std::shared_ptr<Tmpl8::Sprite> sprite;
	Tmpl8::vec2 position{};
	Tmpl8::vec2 size{};

	std::unique_ptr<Animator> animator;

	float lastTime = 0.f;
};

