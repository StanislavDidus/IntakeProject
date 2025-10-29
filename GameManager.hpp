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
#include "Particle.hpp"

namespace Tmpl8
{
	class Game;
}

class GameManager : public Listener
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

	void updateObjects(float deltaTime);

	void spawnAsteroid();
	void spawnUpgrade();
	void spawnSheep(Tmpl8::vec2 pos, Tmpl8::vec2 size, Tmpl8::vec2 direction, float angle);
		
	float spawnRate = 2.f;
	float spawnTimer = 2.f;

	std::unique_ptr<TimerManager> timerManager;

	//Particles
	std::vector<std::shared_ptr<Particle>> particles;


	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<Object>> objects;

	//Temp object vector
	std::vector<std::shared_ptr<Object>> tempObjects;

	std::shared_ptr<CollisionManager> collisionManager;

	std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>> spriteMap;
	std::unordered_map<std::string, Audio::Sound> soundMap;

	std::vector<std::shared_ptr<Tmpl8::Sprite>> asteroidSprites;

	//Asteroids
	float asteroidAcceleration = 500.f;
	float asteroidMaxSpeed = 100.f;

	//Particles
	//float particleSpawnTime = 0.3f;

	float upgradeSpawnTime = 5.f;
	float upgradeSpawnTimer = 5.f;
	bool isUpgradeOnMap = false;
	bool isUpgradeUsed = false; 

	int sheepCounter = 0;
};

