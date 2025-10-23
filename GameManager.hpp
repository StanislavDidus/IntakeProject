#pragma once

#include <memory>
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <unordered_map>

#include "Asteroid.hpp"
#include "Player.hpp"
#include "Sheep.hpp"
#include "EventBus.hpp"
#include "TimerManager.hpp"
#include "CollisionManager.hpp"

namespace Tmpl8
{
	class Game;
}

class GameManager
{
public:
	GameManager(std::shared_ptr<CollisionManager> collisionManager, const std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>>& spriteMap, const std::unordered_map<std::string, Audio::Sound>& soundMap);
	virtual ~GameManager();

	void update(float deltaTime);
	void render(Tmpl8::Surface& screen );

	//Getters
	int getNumberOfSheep() const;

	std::shared_ptr<Player> getPlayer() const;

	std::vector<std::shared_ptr<Object>> getObjects() const;
private:
	void initTimerManager();

	void updatePlayer(float deltaTime);
	void updateObjects(float deltaTime);

	void spawnAsteroid();
	void spawnUpgrade();
	void spawnSheep(Tmpl8::vec2 pos, Tmpl8::vec2 size, Tmpl8::vec2 direction, float angle);
		
	float spawnRate, spawnTimer;

	std::unique_ptr<TimerManager> timerManager;

	//Particles
	std::vector<std::pair<std::shared_ptr<Object>, float>> particles;


	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<Object>> objects;

	//Temp object vector
	std::vector<std::shared_ptr<Object>> tempObjects;

	std::shared_ptr<CollisionManager> collisionManager;

	std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>> spriteMap;
	std::unordered_map<std::string, Audio::Sound> soundMap;

	std::vector<std::shared_ptr<Tmpl8::Sprite>> asteroidSprites;

	//Particles
	float particleSpawnTime = 0.3f;

	float upgradeSpawnTime, upgradeSpawnTimer;
	bool isUpgradeOnMap;
	bool isUpgradeUsed;

	int sheepCounter = 0;
};

