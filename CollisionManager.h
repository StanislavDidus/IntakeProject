#pragma once

#include "PhysicObject.h"
#include <memory>
#include <vector>
#include <optional>
#include <set>

#include "Grid.h"

struct CollisionEvent
{
	Object* collider;
	//other data
};

template<typename T>
struct unordered_pair
{
	unordered_pair(T x, T y) : a(x), b(y) {}

	bool operator==(const unordered_pair<T>& other) const { return (a == other.a && b == other.b) || (a == other.b && b == other.a); }
	bool operator<(const T& other) const { return a < other.a && b < other.b; }

	T a, b;
};

class CollisionManager
{
public:
	CollisionManager(std::shared_ptr<Grid> grid);

	void checkCollision();
	void destroyObject(Object* object);
	void render(Tmpl8::Surface& screen);
	void renderDEBUG(Tmpl8::Surface& screen);

private:
	bool PointRectangle(Tmpl8::vec2 target, Object* col);
	bool PixelPerfectCheck(Object* target, Object* col, const Tmpl8::int4& overlap);
	bool PixelPerfectCheck(Object* target, Object* col, const Tmpl8::vec4& overlap);
	void LineLineIntersection(Tmpl8::vec4 target, Tmpl8::vec4 col, std::vector<Tmpl8::vec2>& points);

	Tmpl8::vec4 getIntersection(Object* target, Object* col);

	void CheckCollisionStatus(Object* A, Object* B, bool isCollision);

	std::vector<unordered_pair<Object*>> collisions; // All collision in current frame

	std::shared_ptr<Grid> grid;
};

