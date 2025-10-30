#include "Particle.hpp"

Particle::Particle(std::shared_ptr<Tmpl8::Sprite> sprite, const Tmpl8::vec2& position, const Tmpl8::vec2& size, float lastTime) : sprite(sprite), position(position), size(size), lastTime(lastTime)
{
	initAnimator();
}

Particle::~Particle()
{
}

void Particle::update(float deltaTime)
{
	animator->update(deltaTime);

	lastTime -= deltaTime;

	if (lastTime <= 0.f) destroy = true;
}

void Particle::render(Tmpl8::Surface& screen)
{
	animator->setAnimationFrame(sprite);
	
	Tmpl8::vec2i posi = { static_cast<int>(position.x), static_cast<int>(position.y) };
	Tmpl8::vec2i sizei = { static_cast<int>(size.x), static_cast<int>(size.y) };
		
	sprite->DrawScaled(posi.x, posi.y, sizei.x, sizei.y, screen);
}

void Particle::resize(float scale)
{
	Tmpl8::vec2 center = Tmpl8::vec2{ position.x + size.x / 2.f, position.y + size.y / 2.f };

	size = size * scale;

	position = Tmpl8::vec2{ center.x - size.x / 2.f, center.y - size.y / 2.f };
}

void Particle::initAnimator()
{
	unsigned int frames = sprite->Frames();
	float animationSpeed = lastTime / static_cast<float>(frames);
	
	animator = std::make_unique<Animator>();

	animator->addFrameCycledAnimation(sprite, animationSpeed, 0, frames - 1, "Particle");

	animator->playAnimation("Particle");
}
