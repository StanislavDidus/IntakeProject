#include "game.h"
#include "surface.h"

#include <cstdio> //printf

namespace Tmpl8
{
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------

	std::bitset<256> Game::pressedButtons{};
	std::bitset<256> Game::releasedButtons{};
	std::bitset<256> Game::heldButtons{};

	bool Game::isKeyDown(int key)
	{
		return pressedButtons[key];
	}

	bool Game::isKeyHold(int key)
	{
		return heldButtons[key];
	}

	bool Game::isKeyUp(int key)
	{
		return releasedButtons[key];
	}

	void Game::initSprites()
	{
		//Ship
		sprites["ship"] = std::make_shared<Sprite>(new Surface("assets/ship.png"), 4);
		sprites["shipEngine"] = std::make_shared<Sprite>(new Surface("assets/engine.png"), 1);
		sprites["engineEffect"] = std::make_shared<Sprite>(new Surface("assets/engineEffect.png"), 7);
		sprites["weapon"] = std::make_shared<Sprite>(new Surface("assets/weapon.png"), 7);
		sprites["weapon1"] = std::make_shared<Sprite>(new Surface("assets/weapon1.png"), 12);
		sprites["bullet"] = std::make_shared<Sprite>(new Surface("assets/bullet.png"), 4);
		sprites["bullet1"] = std::make_shared<Sprite>(new Surface("assets/bullet1.png"), 10);
		//Object
		sprites["sheep"] = std::make_shared<Sprite>(new Surface("assets/sheep.png"), 1);
		sprites["space"] = std::make_shared<Sprite>(new Surface("assets/space.png"), 2);
		sprites["upgrade"] = std::make_shared<Sprite>(new Surface("assets/upgrade.png"), 1);
		//UI
		sprites["startButton"] = std::make_shared<Sprite>(new Surface("assets/startButton.png"), 3);
		sprites["scoreButton"] = std::make_shared<Sprite>(new Surface("assets/scoreButton.png"), 3);
		sprites["exitButton"] = std::make_shared<Sprite>(new Surface("assets/exitButton.png"), 3);
		//sprites["creditButton"] = std::make_shared<Sprite>(new Surface("assets/creditButton.png"), 3);
		sprites["logo"] = std::make_shared<Sprite>(new Surface("assets/logo.png"), 1);
	}

	void Game::initGameManager()
	{
		gameManager = std::make_shared<GameManager>(sprites, this);
	}

	void Game::initCollisionManager()
	{
		colManager = std::make_shared<CollisionManager>(gameManager);
	}

	void Game::initAnimators()
	{
		bgAnimator = std::make_unique<Animator>();
		bgAnimator->addFrameAnimation(sprites["space"], 1.f, 0, sprites["space"]->Frames() - 1, []() {return true; });
	}

	void Game::initButtons()
	{
		int sizeX = 150;
		int sizeY = 75;
		int posY = 25;

		buttons.push_back(std::make_shared<Button>(sprites["startButton"], ScreenWidth / 2 - sizeX / 2, ScreenHeight / 2 - sizeY / 2 + posY, sizeX, sizeY, [this] { initGame(); }));
		buttons.push_back(std::make_shared<Button>(sprites["scoreButton"], ScreenWidth / 2 - sizeX / 2, ScreenHeight / 2 - sizeY / 2 + sizeY + posY, sizeX, sizeY, [this] {}));
		buttons.push_back(std::make_shared<Button>(sprites["exitButton"], ScreenWidth / 2 - sizeX / 2, ScreenHeight / 2 - sizeY / 2 + sizeY * 2 + posY, sizeX, sizeY, [this] {std::exit(0); }));
		//buttons.push_back(std::make_shared<Button>(sprites["creditButton"], ScreenWidth / 2 - sizeX / 2 + 125, ScreenHeight / 2 - sizeY / 2 + sizeY * 2 + posY, 100, 75, [this] { }));
	}

	void Game::initGame()
	{
		state = GameState::GAME;
		
		initGameManager();
		initCollisionManager();
		initAnimators();

		
	}
	
	void Game::Init()
	{
		state = GameState::MENU;

		initSprites();
		initButtons();
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
		if(state == GameState::GAME)
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
		switch (state)
		{
		case GameState::MENU:
			for (const auto& button : buttons) button->CheckClick(mousePosition, wasMouseDown, wasMouseUp);
				break;
		case GameState::GAME:
			gameManager->update(deltaTime);

			colManager->checkCollision();

			bgAnimator->update(deltaTime);
			break;
		}
		
		//Update buttons
		wasMouseDown = false;
		wasMouseUp = false;

		/*for (const auto& key : downButtons)
		{
			holdButtons.insert(key);
		}
		downButtons.clear();
		upButtons.clear();*/

		pressedButtons = keys & ~heldButtons;
		releasedButtons = heldButtons & ~keys;
		heldButtons = keys;
	}

	void Game::render(Tmpl8::Surface& screen)
	{
		screen.Clear(0);

		sprites["space"]->DrawScaled(0, 0, ScreenWidth, ScreenHeight, screen);

		switch (state)
		{
		case GameState::MENU:
			for (const auto& button : buttons)
				button->render(screen);
			sprites["logo"]->DrawScaled(175, 10, ScreenWidth - 350, 225, screen);
			break;
		case GameState::GAME:
			gameManager->render(screen);
			break;
		}
	}
};