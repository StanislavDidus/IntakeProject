#pragma once

#include "template.h"
#include "surface.h"
#include <iostream>
#include <vector>

class UIElement
{
public:
	UIElement(std::shared_ptr<Tmpl8::Sprite> sprite, float x, float y, int width, int height) : sprite(sprite), position({ x, y }), size({ width, height }) {}
	virtual ~UIElement() {}

	virtual void update(float deltaTime) {}
	virtual void render(Tmpl8::Surface& screen) {}

	void setPosition(Tmpl8::vec2 pos) { position = pos; }
protected:
	std::shared_ptr<Tmpl8::Sprite> sprite;
	Tmpl8::vec2 position;
	Tmpl8::vec2i size;
};