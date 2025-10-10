#pragma once

#include "template.h"
#include "surface.h"
#include <iostream>
#include <vector>
#include <memory>

#include "Functions.h"

enum Side
{
	LEFT, RIGHT, UP, DOWN
};

struct CollisionEvent;

class Object
{
public:
	Object
	(
		std::shared_ptr<Tmpl8::Sprite> sprite,
		float x,
		float y,
		int width,
		int height,
		Tmpl8::vec2 velocity,
		float maxSpeed,
		Tmpl8::vec2 acceleration,
		Tmpl8::vec2 direction,
		float angle,
		const std::string& tag
	);
	virtual ~Object();

	virtual void update(float deltaTime); // = 0; Pure virtual.
	virtual void render(Tmpl8::Surface& screen);
	void renderAt(Tmpl8::Sprite& sprite, float x, float y, Tmpl8::Surface& screen);

	const Tmpl8::vec2 getPosition() const;
	const Tmpl8::vec2 getSize() const;
	const Tmpl8::vec2 getDirection() const;

	virtual void move(float deltaTime);
	virtual void applyVelocity(float deltaTime);
	virtual void stop(float deltaTime);

	const std::vector<Tmpl8::vec2>        getAxes                  ()                                                     const;
	const std::vector<Tmpl8::vec4>        getEdges                 ()                                                     const;
	const std::vector<Tmpl8::vec2>        getVertices              (Tmpl8::vec2 pos)                                      const;
	const std::vector<Tmpl8::vec2>        getVertices              ()                                                     const;
	const Tmpl8::vec2                     getRotatedPoint          (Tmpl8::vec2 pos, float dir = 1.f)                     const;
	Tmpl8::Pixel                          getPixelAtRotatedPosition(int pixelX, int pixelY)                               const;

	float getAngle() const;
	std::shared_ptr<Tmpl8::Sprite> getSprite() const;
	const std::string& getTag() const;

	virtual void onCollisionEnter(std::shared_ptr<Object> object);
	virtual void onCollisionStay(std::shared_ptr<Object> object);
	virtual void onCollisionExit(std::shared_ptr<Object> object);

	bool destroy; // Destroy as soon as possible
protected:
	const Tmpl8::vec2                     getEdgeVector            (Side s)                                               const;
	const Tmpl8::vec4                     getEdge                  (Side s)                                               const;
	const Tmpl8::vec2                     getVertex                (Side v, Side h)                                       const;
	const Tmpl8::vec2                     getVertexAtPos           (Side v, Side h, Tmpl8::vec2 pos)                      const;
	const Tmpl8::vec2                     getRotatedPointWithCenter(Tmpl8::vec2 pos, Tmpl8::vec2 center, float dir = 1.f) const;

	std::shared_ptr<Tmpl8::Sprite> sprite;
	float x, y;
	int width, height;
	float angle;
	std::string tag;

	//Physics
	Tmpl8::vec2 velocity;
	float maxSpeed;
	Tmpl8::vec2 acceleration;
	Tmpl8::vec2 direction;
};

