#pragma once

#include "template.h"
#include "surface.h"
#include <iostream>
#include <vector>
#include <functional>

struct Animation {};

class FrameAnimation : public Animation
{
public:
	FrameAnimation(std::shared_ptr<Tmpl8::Sprite> sprite, float animationSpeed, int firstFrame, int lastFrame,const std::function<bool()>& condition);

	void play(float deltaTime);

	const std::function<bool()>& getCondition() const;
private:
	std::shared_ptr<Tmpl8::Sprite> sprite;
	float animationSpeed;
	int firstFrame, lastFrame, currentFrame;
	std::function<bool()> condition;
	float timer;
};

class FrameCycledAnimation : public Animation
{
public:
	FrameCycledAnimation(std::shared_ptr<Tmpl8::Sprite> sprite, float animationSpeed, int firstFrame, int lastFrame, bool returnToBaseFrame);

	void play(float deltaTime);
	void stop();
	void setBaseFrame();

	bool active;
private:
	std::shared_ptr<Tmpl8::Sprite> sprite;
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

	bool isAnimationActive(const std::string& name);

	void addFrameAnimation(std::shared_ptr<Tmpl8::Sprite>sprite, float animationSpeed, int firstFrame, int lastFrame, const std::function<bool()>& condition);
	void addFrameCycledAnimation(std::shared_ptr<Tmpl8::Sprite> sprite, float animationSpeed, int firstFrame, int lastFrame, const std::string& name, bool returnToBaseFrame = true);
private:
	std::vector<std::shared_ptr<FrameAnimation>> frameAnimations;
	std::unordered_map<std::string, std::shared_ptr<FrameCycledAnimation>> frameCycledAnimations;
};

