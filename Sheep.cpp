#include "Sheep.hpp"
#include "CollisionHelper.hpp"

Sheep::Sheep(std::shared_ptr<Tmpl8::Sprite> sprite, Tmpl8::vec2 position, Tmpl8::vec2 size) :
	Object(sprite, position, size), rotationSpeed(50.f)
{

}

void Sheep::update(float deltaTime)
{
	angle -= rotationSpeed * deltaTime;
	this->angle = fmodf(this->angle, 360.f);

	move(deltaTime);

	applyVelocity(deltaTime);
}

void Sheep::render(Tmpl8::Surface& screen)
{
	std::vector<Vertex> v = getVertices();
	sprite->DrawScaledRotated(v[0], v[1], v[2], v[3], screen);
}
