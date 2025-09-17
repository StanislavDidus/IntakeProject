#pragma once

#include "PhysicObject.h"
#include <memory>
#include <vector>

class CollisionManager
{
public:
	CollisionManager();

	void checkCollision();
	void render(std::shared_ptr<PhysicObject> obj, const std::vector<std::shared_ptr<PhysicObject>>& colliders, Tmpl8::Surface& screen);
private:
	bool AABB(PhysicObject* target, PhysicObject* col);
	bool SAT(PhysicObject* target, PhysicObject* col);
};

