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
		//Ship
		sprites["ship"] = std::make_shared<Sprite>(new Surface("assets/airship.png"), 1);
		sprites["shipEngine"] = std::make_shared<Sprite>(new Surface("assets/engine.png"), 1);
		sprites["engineEffect"] = std::make_shared<Sprite>(new Surface("assets/engineEffect.png"), 7);
		sprites["weapon"] = std::make_shared<Sprite>(new Surface("assets/weapon.png"), 7);
		sprites["bullet"] = std::make_shared<Sprite>(new Surface("assets/bullet.png"), 4);

		sprites["space"] = std::make_shared<Sprite>(new Surface("assets/space.png"), 2);
		//sprites["asteroid"] = new Sprite(new Surface("assets/asteroid.png"), 3);
	}

	void Game::initGameManager()
	{
		gameManager = std::make_shared<GameManager>(sprites);
	}

	void Game::initCollisionManager()
	{
		colManager = std::make_shared<CollisionManager>(gameManager);
	}

	void Game::initAnimators()
	{
		bgAnimator = std::make_unique<Animator>();
		bgAnimator->addFrameAnimation(sprites["space"].get(), 1.f, 0, sprites["space"]->Frames() - 1, []() {return true; });
	}

	/*void Game::initPlayer()
	{
		
	}*/
	
	void Game::Init()
	{
		initSprites();
		initGameManager();
		initCollisionManager();
		initAnimators();
		//initPlayer();
	}
	
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		deltaTime /= 1000.f;

		update(deltaTime);
		render(*screen);
#ifdef _DEBUG
		colManager->renderDEBUG(*screen);

		//FPS Counter
		std::stringstream ss;
		ss << "FPS: " + std::to_string(1.f / deltaTime);
		screen->Print(&ss.str()[0], 0, 100, 0xffffff);
#else 
		std::stringstream ss;
		ss << "FPS: " + std::to_string(1.f / deltaTime);
		screen->Print(&ss.str()[0], 0, 100, 0xffffff);
#endif
	}

	void Game::update(float deltaTime)
	{
		updateControls(deltaTime);

		gameManager->update(deltaTime);

		colManager->checkCollision();

		bgAnimator->update(deltaTime);

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

		sprites["space"]->DrawScaled(0, 0, ScreenWidth, ScreenHeight, &screen);

		gameManager->render(screen);
	}

	void Game::updateControls(float deltaTime)
	{
		//Movement
		if (isKeyHold('w'))
		{
			gameManager->getPlayer()->move(deltaTime);
		}
		
		//Rotation
		if(isKeyHold('a'))
			gameManager->getPlayer()->rotate(-1.f, deltaTime);
			
		if(isKeyHold('d'))
			gameManager->getPlayer()->rotate(1.f, deltaTime);

		//Shoot
		if (isKeyHold('e') || isKeyHold(' '))
			gameManager->getPlayer()->shoot();
			
		//Stop
		if (!isKeyHold('w'))
			gameManager->getPlayer()->stop(deltaTime);

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