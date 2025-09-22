#pragma once

#include "Asteroid.h"
#include <memory>
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>

#include "Player.h"
#include "CollisionManager.h"

class GameManager : public Listener
{
public:
	GameManager(std::shared_ptr<CollisionManager> colManager, Tmpl8::Sprite& bulletSprite);
	virtual ~GameManager();

	void update(float deltaTime);
	void render(Tmpl8::Surface& screen );

	std::vector<std::shared_ptr<Asteroid>> asteroids;
private:
	void updateBullets(float deltaTime);
	void updateAsteroids(float deltaTime);

	void spawnAsteroid();

	float spawnRate, spawnTimer;

	std::shared_ptr<CollisionManager> colManager;

	std::vector<std::shared_ptr<Bullet>> bullets;
	Tmpl8::Sprite bulletSprite;

	std::vector<Tmpl8::Surface*> surfaces;
	std::vector<Tmpl8::Sprite*> sprites;
};

