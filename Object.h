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

//Add seters
//Simplify the constructor
class Object
{
public:
	Object
	(
		std::shared_ptr<Tmpl8::Sprite> sprite,
		Tmpl8::vec2 position,
		Tmpl8::vec2 size
	);
	virtual ~Object();

	virtual void update(float deltaTime); // = 0; Pure virtual.
	virtual void render(Tmpl8::Surface& screen);
	void renderAt(Tmpl8::Sprite& sprite, Tmpl8::vec2 position, Tmpl8::Surface& screen);

	//Setters
	void setPosition(Tmpl8::vec2 position);
	void setSize(Tmpl8::vec2 size);
	void setVelocity(Tmpl8::vec2 velocity);
	void setMaxSpeed(float maxSpeed);
	void setAcceleration(Tmpl8::vec2 acceleration);
	void setDirection(Tmpl8::vec2 direction);
	void setAngle(float angle);
	void setTag(const std::string& tag);

	//Getters
	std::shared_ptr<Tmpl8::Sprite> getSprite() const;
	Tmpl8::vec2 getPosition() const;
	Tmpl8::vec2 getSize() const;
	Tmpl8::vec2 getVelocity() const;
	float getMaxSpeed() const;
	Tmpl8::vec2 getAcceleration() const;
	Tmpl8::vec2 getDirection() const;
	float getAngle() const;
	const std::string& getTag() const;

	virtual void move(float deltaTime);
	virtual void applyVelocity(float deltaTime);
	virtual void stop(float deltaTime);

	const std::vector<Tmpl8::vec2>        getAxes                  ()                                                     const;
	const std::vector<Tmpl8::vec4>        getEdges                 ()                                                     const;
	const std::vector<Vertex>             getVertices              (Tmpl8::vec2 pos)                                      const;
	const std::vector<Vertex>             getVertices              ()                                                     const;
	const Tmpl8::vec2                     getRotatedPoint          (Tmpl8::vec2 pos, float dir = 1.f)                     const;
	Tmpl8::Pixel                          getPixelAtRotatedPosition(int pixelX, int pixelY)                               const;

	virtual void onCollisionEnter(std::shared_ptr<Object> object);
	virtual void onCollisionStay(std::shared_ptr<Object> object, float deltaTime);
	virtual void onCollisionExit(std::shared_ptr<Object> object);

	bool destroy = false; // Destroy as soon as possible
	bool checkPixelPerfectCollision = true;
protected:
	const Tmpl8::vec2                     getEdgeVector            (Side s)                                               const;
	const Tmpl8::vec4                     getEdge                  (Side s)                                               const;
	const Tmpl8::vec2                     getVertex                (Side v, Side h)                                       const;
	const Tmpl8::vec2                     getVertexAtPos           (Side v, Side h, Tmpl8::vec2 pos)                      const;
	const Tmpl8::vec2                     getRotatedPointWithCenter(Tmpl8::vec2 pos, Tmpl8::vec2 center, float dir = 1.f) const;

	std::shared_ptr<Tmpl8::Sprite> sprite;
	Tmpl8::vec2 position{};
	Tmpl8::vec2 size{};
	Tmpl8::vec2 scale = {1.f, 1.f};
	float angle = 0.f;
	std::string tag = "default";

	//Physics
	Tmpl8::vec2 velocity{};
	float maxSpeed = 0.f;
	Tmpl8::vec2 acceleration{};
	Tmpl8::vec2 direction{};
};

