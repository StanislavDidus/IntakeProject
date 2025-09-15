#pragma once

#include "Asteroid.h"
#include <memory>
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "Player.h"

class GameManager
{
public:
	GameManager();

	void update(float deltaTime);
	void render(Tmpl8::Surface& screen );
private:
	void spawnAsteroid();

	float spawnRate, spawnTimer;

	std::vector<std::shared_ptr<Asteroid>> asteroids;

	std::vector<Tmpl8::Surface*> surfaces;
	std::vector<Tmpl8::Sprite*> sprites;
};

