#pragma once

#include <memory>
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>

#include "Asteroid.h"
#include "Player.h"

class GameManager : public Listener
{
public:
	GameManager(Tmpl8::Sprite& bulletSprite, Tmpl8::Sprite& playerSprite);
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

	Tmpl8::Sprite bulletSprite;
	Tmpl8::Sprite playerSprite;

	std::vector<Tmpl8::Surface*> surfaces;
	std::vector<Tmpl8::Sprite*> sprites;
};

