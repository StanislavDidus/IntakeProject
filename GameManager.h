#pragma once

#include <memory>
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <unordered_map>

#include "Asteroid.h"
#include "Player.h"

class GameManager
{
public:
	GameManager(std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>>& sprites);
	virtual ~GameManager();

	void update(float deltaTime);
	void render(Tmpl8::Surface& screen );

	std::shared_ptr<Player> getPlayer() const;

	std::vector<std::shared_ptr<Object>> getObjects() const;
private:
	void updatePlayer(float deltaTime);
	void updateObjects(float deltaTime);

	void spawnAsteroid();
		
	float spawnRate, spawnTimer;

	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<Object>> objects;

	std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>> sprites;

	std::vector<std::shared_ptr<Tmpl8::Sprite>> asteroidSprites;
};

