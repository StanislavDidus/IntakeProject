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
	sprite->SetFrame(animator->getAnimationFrame(sprite));
	
	Tmpl8::vec2i posi = { static_cast<int>(position.x), static_cast<int>(position.y) };
	Tmpl8::vec2i sizei = { static_cast<int>(size.x), static_cast<int>(size.y) };
		
	sprite->DrawScaled(posi.x, posi.y, sizei.x, sizei.y, screen);
}

void Particle::initAnimator()
{
	unsigned int frames = sprite->Frames();
	float animationSpeed = lastTime / static_cast<float>(frames);
	
	animator = std::make_unique<Animator>();

	animator->addFrameCycledAnimation(sprite, animationSpeed, 0, frames - 1, "Particle");

	animator->playAnimation("Particle");
}
