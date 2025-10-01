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
	FrameAnimation(Tmpl8::Sprite* sprite, float animationSpeed, int firstFrame, int lastFrame,const std::function<bool()>& condition);

	void play(float deltaTime);

	const std::function<bool()>& getCondition() const;
private:
	Tmpl8::Sprite* sprite;
	float animationSpeed;
	int firstFrame, lastFrame, currentFrame;
	std::function<bool()> condition;
	float timer;
};

class FrameCycledAnimation : public Animation
{
public:
	FrameCycledAnimation(Tmpl8::Sprite* sprite, float animationSpeed, int firstFrame, int lastFrame);

	void play(float deltaTime);
	void setBaseFrame();

	bool active;
private:
	Tmpl8::Sprite* sprite;
	float animationSpeed;
	int firstFrame, lastFrame, currentFrame;
	float timer;
};

class Animator
{
public:
	Animator();
	virtual ~Animator();

	void update(float deltaTime);

	void playAnimation(const std::string& name);

	void addFrameAnimation(Tmpl8::Sprite* sprite, float animationSpeed, int firstFrame, int lastFrame, const std::function<bool()>& condition);
	void addFrameCycledAnimation(Tmpl8::Sprite* sprite, float animationSpeed, int firstFrame, int lastFrame, const std::string& name);
private:
	std::vector<std::shared_ptr<FrameAnimation>> frameAnimations;
	std::unordered_map<std::string, std::shared_ptr<FrameCycledAnimation>> frameCycledAnimations;
};

