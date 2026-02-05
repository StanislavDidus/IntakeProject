#include "Animator.hpp"

FrameAnimation::FrameAnimation(std::shared_ptr<Tmpl8::Sprite> sprite, float animationSpeed, int firstFrame, int lastFrame, const std::function<bool()>& condition) :
	 Animation(sprite), animationSpeed(animationSpeed), firstFrame(firstFrame), lastFrame(lastFrame), condition(condition), currentFrame(firstFrame), timer(0.f)
{
}

int FrameAnimation::play(float deltaTime)
{
	timer += deltaTime;

	if (timer >= animationSpeed)
	{
		timer = 0.f;

		currentFrame++;
		if (currentFrame > lastFrame) currentFrame = firstFrame;
		
	}
	return currentFrame;
}
const std::function<bool()>& FrameAnimation::getCondition() const
{
	return condition;
}

FrameCycledAnimation::FrameCycledAnimation(std::shared_ptr<Tmpl8::Sprite> sprite, float animationSpeed, int firstFrame, int lastFrame, bool returnToBaseFrame) :
	Animation(sprite), animationSpeed(animationSpeed), firstFrame(firstFrame), lastFrame(lastFrame), currentFrame(firstFrame), active(false), timer(0.f), returnToBaseFrame(returnToBaseFrame)

{

}

int FrameCycledAnimation::play(float deltaTime)
{
	//int frame = currentFrame;
	
	timer += deltaTime;

	if (timer >= animationSpeed)
	{
		timer = 0.f;

		currentFrame++;

		if (currentFrame > lastFrame)
		{
			currentFrame = lastFrame;

			active = false; 

			if (returnToBaseFrame)
				return 0;
		}
	}

	return currentFrame;
}

int FrameCycledAnimation::stop()
{
	active = false;

	if (returnToBaseFrame)
	{
		return 0;
	}
	else
	{
		return currentFrame;
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
			//if(auto s = animation->getSprite().lock())
			spriteAnimationFrames[animation->getSprite()] = animation->play(deltaTime);
		}
	}

	for (const auto& [name, animation] : frameCycledAnimations)
	{
		if (animation->active)
		{
			//if (auto s = animation->getSprite().lock())
			spriteAnimationFrames[animation->getSprite()] = animation->play(deltaTime);
		}
	}
}

void Animator::playAnimation(const std::string& name)
{
	frameCycledAnimations[name]->setBaseFrame();
	frameCycledAnimations[name]->active = true;
}

void Animator::stopAnimation(const std::string& name)
{
	//frameCycledAnimations[name]->stop();

	auto& animation = frameCycledAnimations[name];
	auto sprite = animation->getSprite();

	//if (auto s = sprite.lock())
	//{
	spriteAnimationFrames[sprite] = animation->stop();
	//}
}

void Animator::setAnimationFrame(std::shared_ptr<Tmpl8::Sprite> sprite)
{
	auto it = spriteAnimationFrames.find(sprite);
	if (it != spriteAnimationFrames.end())
	{
		sprite->SetFrame(it->second);
	}
}

bool Animator::isAnimationActive(const std::string& name)
{
	return frameCycledAnimations[name]->active;
}

void Animator::addFrameAnimation(std::shared_ptr<Tmpl8::Sprite> sprite, float animationSpeed, int firstFrame, int lastFrame, const std::function<bool()>& condition)
{
	frameAnimations.push_back(std::make_shared<FrameAnimation>(sprite, animationSpeed, firstFrame, lastFrame, condition));
}

void Animator::addFrameCycledAnimation(std::shared_ptr<Tmpl8::Sprite> sprite, float animationSpeed, int firstFrame, int lastFrame, const std::string& name, bool returnToBaseFrame)
{
	frameCycledAnimations[name] = std::make_shared<FrameCycledAnimation>(sprite, animationSpeed, firstFrame, lastFrame, returnToBaseFrame);
}
