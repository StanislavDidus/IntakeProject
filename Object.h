#pragma once

#include "template.h"
#include "surface.h"
#include <iostream>
#include <vector>
#include <memory>

struct CollisionEvent;

class Object
{
public:
	Object(Tmpl8::Sprite* sprite, float x, float y, int width, int height, float angle, const std::string& tag);
	virtual ~Object();

	virtual void update(float deltaTime); // = 0; Pure virtual.
	virtual void render(Tmpl8::Surface& screen);

	const Tmpl8::vec2 getPosition() const;
	const Tmpl8::vec2 getSize() const;

	const std::vector<Tmpl8::vec2>        getAxes                  ()                                               const;
	const Tmpl8::vec2                     getEdgeVector            (Tmpl8::vec2 min, Tmpl8::vec2 max)               const;
	const std::vector<Tmpl8::vec4>        getEdges                 ()                                               const;
	const Tmpl8::vec4                     getEdge                  (Tmpl8::vec2 min, Tmpl8::vec2 max)               const;
	const std::vector<Tmpl8::vec2>        getVerticies             ()                                               const;
	const Tmpl8::vec2                     getRotatedPoint          (Tmpl8::vec2 pos, float dir = 1.f)               const;
	Tmpl8::Pixel                          getPixelRotatedToPosition(int pixelX, int pixelY)                         const;

	float getAngle() const;
	Tmpl8::Sprite* getSprite() const;
	const std::string& getTag() const;
	const Tmpl8::vec2 getLastPosition() const;

	virtual void onCollisionEnter(const CollisionEvent& event);
	virtual void onCollisionStay(const CollisionEvent& event);
	virtual void onCollisionExit(const CollisionEvent& event);

	bool destroy; // Destroy at the next frame
protected:
	Tmpl8::Sprite* sprite;
	float x, y;
	int width, height;
	float angle;
	std::string tag;
	Tmpl8::vec2 lastPosition;
};

