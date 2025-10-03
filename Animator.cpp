#include "Animator.h"

FrameAnimation::FrameAnimation(Tmpl8::Sprite* sprite, float animationSpeed, int firstFrame, int lastFrame, const std::function<bool()>& condition) :
	 sprite(sprite), animationSpeed(animationSpeed), firstFrame(firstFrame), lastFrame(lastFrame), condition(condition), currentFrame(firstFrame), timer(0.f)
{
}

void FrameAnimation::play(float deltaTime)
{
	timer += deltaTime;

	if (timer >= animationSpeed)
	{
		timer = 0.f;

		int spriteFrame = sprite->getCurrentFrame();
		if (spriteFrame < firstFrame || spriteFrame > lastFrame) currentFrame = firstFrame;
		else currentFrame = spriteFrame;

		currentFrame++;
		if (currentFrame > lastFrame) currentFrame = firstFrame;
		sprite->SetFrame(currentFrame);
	}
}
const std::function<bool()>& FrameAnimation::getCondition() const
{
	return condition;
}

FrameCycledAnimation::FrameCycledAnimation(Tmpl8::Sprite* sprite, float animationSpeed, int firstFrame, int lastFrame) :
	sprite(sprite), animationSpeed(animationSpeed), firstFrame(firstFrame), lastFrame(lastFrame), currentFrame(firstFrame), active(false), timer(0.f)

{

}

void FrameCycledAnimation::play(float deltaTime)
{
	timer += deltaTime;

	if (timer >= animationSpeed)
	{
		timer = 0.f;

		sprite->SetFrame(currentFrame);

		//std::cout << currentFrame << "\n";

		currentFrame++;

		if (currentFrame > lastFrame)
		{
			active = false; 
			sprite->SetFrame(0);
			return;
		}
	}
}

void FrameCycledAnimation::setBaseFrame()
{
	currentFrame = firstFrame;
}


Animator::Animator()
{

}

Animator::~Animator()
{

}

void Animator::update(float deltaTime)
{
	for (const auto& animation : frameAnimations)
	{
		if (animation->getCondition()())
		{
			animation->play(deltaTime);
		}
	}

	for (const auto& [name, animation] : frameCycledAnimations)
	{
		if(animation->active) animation->play(deltaTime);
	}
}

void Animator::playAnimation(const std::string& name)
{
	frameCycledAnimations[name]->setBaseFrame();
	frameCycledAnimations[name]->active = true;
}

void Animator::addFrameAnimation(Tmpl8::Sprite* sprite, float animationSpeed, int firstFrame, int lastFrame, const std::function<bool()>& condition)
{
	frameAnimations.push_back(std::make_shared<FrameAnimation>(sprite, animationSpeed, firstFrame, lastFrame, condition));
}

void Animator::addFrameCycledAnimation(Tmpl8::Sprite* sprite, float animationSpeed, int firstFrame, int lastFrame, const std::string& name)
{
	frameCycledAnimations[name] = std::make_shared<FrameCycledAnimation>(sprite, animationSpeed, firstFrame, lastFrame);
}
