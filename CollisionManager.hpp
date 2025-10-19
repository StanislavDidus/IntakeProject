#pragma once

#include "Object.hpp"
#include <memory>
#include <vector>
#include <optional>
#include <set>

#include "GameManager.hpp"

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
	CollisionManager(std::shared_ptr<GameManager> gameManager);
	virtual ~CollisionManager();

	void checkCollision(float deltaTime);
	void render(Tmpl8::Surface& screen);
	void renderDEBUG(Tmpl8::Surface& screen);

	//void deleteObject(std::shared_ptr<Object> obj);

private:
	bool PointRectangle(Tmpl8::vec2 target, std::shared_ptr<Object> col);
	bool PixelPerfectCheck(std::shared_ptr<Object>  target, std::shared_ptr<Object>  col, const Tmpl8::int4& overlap);
	bool PixelPerfectCheck(std::shared_ptr<Object>  target, std::shared_ptr<Object> col, const Tmpl8::vec4& overlap);
	void LineLineIntersection(Tmpl8::vec4 target, Tmpl8::vec4 col, std::vector<Tmpl8::vec2>& points);

	Tmpl8::vec4 getIntersection(std::shared_ptr<Object>  target, std::shared_ptr<Object>  col);

	void SendCollisionEvents(std::shared_ptr<Object>  A, std::shared_ptr<Object>  B, bool isCollision, float deltaTime);
	void checkForDestroyedObjects();

	std::vector<unordered_pair<std::shared_ptr<Object>>> collisions; // All collision in current frame

	std::shared_ptr<GameManager> gameManager;

};

