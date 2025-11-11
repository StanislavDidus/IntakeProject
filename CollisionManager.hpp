#pragma once

#include "Object.hpp"
#include <memory>
#include <vector>
#include <optional>
#include <set>

class GameManager;

//Pair in which order of values does no matter
// unordered_pair{1,2} and unordered_pair{2,1} would be equal
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
	CollisionManager();
	virtual ~CollisionManager();

	void checkCollision(std::shared_ptr<GameManager> gameManager, float deltaTime);
	void render(Tmpl8::Surface& screen);
	void renderDEBUG(std::shared_ptr<GameManager> gameManager, Tmpl8::Surface& screen);

	const std::vector<unordered_pair<std::shared_ptr<Object>>>& getCollisions() const;

private:
	bool PointRectangle(Tmpl8::vec2 target, std::shared_ptr<Object> col);
	bool PixelPerfectCheck(std::shared_ptr<Object>  target, std::shared_ptr<Object>  col, const Tmpl8::int4& overlap);
	bool PixelPerfectCheck(std::shared_ptr<Object>  target, std::shared_ptr<Object> col, const Tmpl8::vec4& overlap);
	void LineLineIntersection(Tmpl8::vec4 target, Tmpl8::vec4 col, std::vector<Tmpl8::vec2>& points);

	Tmpl8::vec4 getIntersection(std::shared_ptr<Object>  target, std::shared_ptr<Object>  col);

	void SendCollisionEvents(std::shared_ptr<Object>  A, std::shared_ptr<Object>  B, bool isCollision, float deltaTime);
	void checkForDestroyedObjects();

	std::vector<unordered_pair<std::shared_ptr<Object>>> collisions; // All collision in current frame
};

