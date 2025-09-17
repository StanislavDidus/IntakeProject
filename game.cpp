#include "game.h"
#include "surface.h"

#include <cstdio> //printf

namespace Tmpl8
{
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------

	void Game::initSprites()
	{
		sprites["ship"] = new Sprite(new Surface("assets/airship.png"), 1);
		sprites["space"] = new Sprite(new Surface("assets/space.png"), 1);
		sprites["bullet"] = new Sprite(new Surface("assets/bullet.png"), 1);
		sprites["asteroid"] = new Sprite(new Surface("assets/asteroid1.png"), 3);
	}

	void Game::initGameManager()
	{
		gameManager = std::make_shared<GameManager>();
	}

	void Game::initCollisionManager()
	{
		colManager = std::make_unique<CollisionManager>();
	}

	void Game::initPlayer()
	{
		player = std::make_shared<Player>
			(
				sprites["ship"],
				sprites["bullet"],
				static_cast<float>(ScreenWidth) / 2.f - 32.f / 2.f,
				static_cast<float>(ScreenHeight) / 2.f - 32.f / 2.f,
				32,
				32,
				Tmpl8::vec2{0.f, 0.f},
				Tmpl8::vec2{150.f, 150.f},
				Tmpl8::vec2{550.f, 550.f},
				Tmpl8::vec2{0.f, -1.f}
			);
	}
	
	void Game::Init()
	{
		initSprites();
		initGameManager();
		initCollisionManager();
		initPlayer();
	}
	
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
		for (auto& [name, sprite] : sprites)
		{
			delete sprite;
		}
		sprites.clear();
	}

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		deltaTime /= 1000.f;

		update(deltaTime);
		render(*screen);
	}

	void Game::update(float deltaTime)
	{
		updateControls(deltaTime);

		player->update(deltaTime);

		gameManager->update(deltaTime);

		// Check collisions.
		colManager->checkCollision();

		//Update buttons
		for (const auto& key : downButtons)
		{
			holdButtons.insert(key);
		}
		downButtons.clear();
		upButtons.clear();
	}

	void Game::render(Tmpl8::Surface& screen)
	{
		screen.Clear(0);

		sprites["space"]->Draw(&screen, 0, 0);
		player->render(screen);

		gameManager->render(screen);

		std::vector<std::shared_ptr<PhysicObject>> physicObg(gameManager->asteroids.begin(), gameManager->asteroids.end());
		colManager->render(player, physicObg, screen);
	}

	void Game::updateControls(float deltaTime)
	{
		//Movement
		if (isKeyHold('w'))
		{
			player->move(deltaTime);
		}
		
		//Rotation
		if(isKeyHold('a'))
			player->rotate(-1.f);
			
		if(isKeyHold('d'))
			player->rotate(1.f);

		//Shoot
		if (isKeyHold('e') || isKeyHold(' '))
			player->shoot();
			
		//Stop
		if (!isKeyHold('w'))
			player->stop(deltaTime);

		/*if (isKeyDown('t')) std::cout << "down\n";
		if (isKeyHold('t')) std::cout << "hold\n";
		if (isKeyUp('t')) std::cout << "up\n";*/
		
	}
	bool Game::isKeyDown(int key)
	{
		return downButtons.find(key) != downButtons.end();
	}

	bool Game::isKeyHold(int key)
	{
		return holdButtons.find(key) != holdButtons.end();
	}

	bool Game::isKeyUp(int key)
	{
		return upButtons.find(key) != upButtons.end();
	}
};