#pragma once

#include "template.h"
#include "surface.h"
#include <iostream>
#include <vector>

class Object
{
public:
	Object(Tmpl8::Sprite* sprite, float x, float y, int width, int height, float angle) : sprite(sprite), x(x), y(y), width(width), height(height), angle(angle) {}

	virtual void update(float deltaTime); // = 0; Pure virtual.
	virtual void render(Tmpl8::Surface& screen);

	const Tmpl8::vec2 getPosition() const;
	const Tmpl8::vec2i getSize() const;

	const std::vector<Tmpl8::vec2>        getAxes     ()                                                          const;
	const Tmpl8::vec2                     getEdge     (Tmpl8::vec2 min, Tmpl8::vec2 max)                          const;
	const std::vector<Tmpl8::vec2>        getVerticies()                                                          const;
	inline const Tmpl8::vec2              getPoint    (float sin, float cos, float cx, float cy, Tmpl8::vec2 pos) const;
protected:
	//const Tmpl8::vec2              getCenterCoord()                                                         const;

	Tmpl8::Sprite* sprite;
	float x, y;
	int width, height;
	float angle;
};

