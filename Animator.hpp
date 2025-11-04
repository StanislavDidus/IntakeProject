#pragma once

#include "template.h"
#include "surface.h"
#include <iostream>
#include <vector>
#include <functional>
#include <map>

class Animation 
{
public:
	Animation(std::shared_ptr<Tmpl8::Sprite> sprite) : sprite(sprite) {}
	virtual ~Animation() {}

	virtual int play(float deltaTime) = 0;

	std::weak_ptr<Tmpl8::Sprite> getSprite() const
	{
		return sprite;
	}
protected:
	std::weak_ptr<Tmpl8::Sprite> sprite;
};

class FrameAnimation : public Animation
{
public:
	FrameAnimation(std::shared_ptr<Tmpl8::Sprite> sprite, float animationSpeed, int firstFrame, int lastFrame,const std::function<bool()>& condition);

	int play(float deltaTime) override;

	const std::function<bool()>& getCondition() const;
private:
	float animationSpeed;
	int firstFrame, lastFrame, currentFrame;
	std::function<bool()> condition;
	float timer;
};

class FrameCycledAnimation : public Animation
{
public:
	FrameCycledAnimation(std::shared_ptr<Tmpl8::Sprite> sprite, float animationSpeed, int firstFrame, int lastFrame, bool returnToBaseFrame);

	int play(float deltaTime) override;
	int stop();
	void setBaseFrame();

	bool active;
private:
	float animationSpeed;
	int firstFrame, lastFrame, currentFrame;
	float timer;
	bool returnToBaseFrame;
};

class Animator
{
public:
	Animator();
	virtual ~Animator();

	void update(float deltaTime);

	void playAnimation(const std::string& name);
	void stopAnimation(const std::string& name);

	//Get animation frame based on a sprite
	void setAnimationFrame(std::shared_ptr<Tmpl8::Sprite> sprite); 

	bool isAnimationActive(const std::string& name);

	void addFrameAnimation(std::shared_ptr<Tmpl8::Sprite>sprite, float animationSpeed, int firstFrame, int lastFrame, const std::function<bool()>& condition);
	void addFrameCycledAnimation(std::shared_ptr<Tmpl8::Sprite> sprite, float animationSpeed, int firstFrame, int lastFrame, const std::string& name, bool returnToBaseFrame = true);
private:
	std::vector<std::shared_ptr<FrameAnimation>> frameAnimations;

	//String is used to identify them by names
	std::unordered_map<std::string, std::shared_ptr<FrameCycledAnimation>> frameCycledAnimations;

	//stores an index of a current frame for each sprite
	//map is necessary when one animator is managing multiple sprites at once
	//!!! Should use std::pair<std::weak_ptr>, int> instead
	std::map<std::shared_ptr<Tmpl8::Sprite>, int> spriteAnimationFrames;
};

