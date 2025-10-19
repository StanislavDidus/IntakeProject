#include "Animator.hpp"

FrameAnimation::FrameAnimation(std::shared_ptr<Tmpl8::Sprite> sprite, float animationSpeed, int firstFrame, int lastFrame, const std::function<bool()>& condition) :
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

FrameCycledAnimation::FrameCycledAnimation(std::shared_ptr<Tmpl8::Sprite> sprite, float animationSpeed, int firstFrame, int lastFrame, bool returnToBaseFrame) :
	sprite(sprite), animationSpeed(animationSpeed), firstFrame(firstFrame), lastFrame(lastFrame), currentFrame(firstFrame), active(false), timer(0.f), returnToBaseFrame(returnToBaseFrame)

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

			if(returnToBaseFrame)
				sprite->SetFrame(0);

			return;
		}
	}
}

void FrameCycledAnimation::stop()
{
	active = false;

	if(returnToBaseFrame)
		sprite->SetFrame(0);
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

void Animator::stopAnimation(const std::string& name)
{
	frameCycledAnimations[name]->stop();
}

void Animator::addFrameAnimation(std::shared_ptr<Tmpl8::Sprite> sprite, float animationSpeed, int firstFrame, int lastFrame, const std::function<bool()>& condition)
{
	frameAnimations.push_back(std::make_shared<FrameAnimation>(sprite, animationSpeed, firstFrame, lastFrame, condition));
}

void Animator::addFrameCycledAnimation(std::shared_ptr<Tmpl8::Sprite> sprite, float animationSpeed, int firstFrame, int lastFrame, const std::string& name, bool returnToBaseFrame)
{
	frameCycledAnimations[name] = std::make_shared<FrameCycledAnimation>(sprite, animationSpeed, firstFrame, lastFrame, returnToBaseFrame);
}
