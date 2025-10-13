#include "game.h"
#include "surface.h"
#include <cassert>

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
		spriteMap["ship"] = std::make_shared<Sprite>(new Surface("assets/ship.png"), 4);
		spriteMap["shipEngine"] = std::make_shared<Sprite>(new Surface("assets/engine.png"), 1);
		spriteMap["engineEffect"] = std::make_shared<Sprite>(new Surface("assets/engineEffect.png"), 7);
		spriteMap["weapon"] = std::make_shared<Sprite>(new Surface("assets/weapon.png"), 7);
		spriteMap["weapon1"] = std::make_shared<Sprite>(new Surface("assets/weapon1.png"), 12);
		spriteMap["bullet"] = std::make_shared<Sprite>(new Surface("assets/bullet.png"), 4);
		spriteMap["bullet1"] = std::make_shared<Sprite>(new Surface("assets/bullet1.png"), 10);
		//Object
		spriteMap["sheep"] = std::make_shared<Sprite>(new Surface("assets/sheep.png"), 1);
		spriteMap["space"] = std::make_shared<Sprite>(new Surface("assets/space.png"), 2);
		spriteMap["upgrade"] = std::make_shared<Sprite>(new Surface("assets/upgrade.png"), 1);
		//UI
		spriteMap["startButton"] = std::make_shared<Sprite>(new Surface("assets/startButton.png"), 3);
		spriteMap["scoreButton"] = std::make_shared<Sprite>(new Surface("assets/scoreButton.png"), 3);
		spriteMap["exitButton"] = std::make_shared<Sprite>(new Surface("assets/exitButton.png"), 3);
		spriteMap["logo"] = std::make_shared<Sprite>(new Surface("assets/logo.png"), 1);
	}

	void Game::initSounds()
	{
		auto music = Audio::Sound{"assets/Sounds/music.mp3", Audio::Sound::Type::Stream};
		music.setLooping(true);
		music.replay();

		soundMap["music"] = music;
		soundMap["shoot"] = Audio::Sound{ "assets/Sounds/shoot.mp3" };
		soundMap["shoot1"] = Audio::Sound{ "assets/Sounds/shoot1.mp3" };
		soundMap["charge"] = Audio::Sound{ "assets/Sounds/charge.mp3" };
		soundMap["hit"] = Audio::Sound{ "assets/Sounds/hitEffect.mp3" };
		soundMap["charge"] = Audio::Sound{ "assets/Sounds/charge.mp3" };
		soundMap["upgrade"] = Audio::Sound{ "assets/Sounds/upgrade.mp3" };
		soundMap["shipDamaged"] = Audio::Sound{ "assets/Sounds/shipDamaged.mp3" };
		soundMap["asteroidDestroyed"] = Audio::Sound{ "assets/Sounds/asteroidDestroyed.mp3" };
		soundMap["gameOver"] = Audio::Sound{ "assets/Sounds/gameOver.mp3" };
		soundMap["buttonCover"] = Audio::Sound{ "assets/Sounds/buttonCover.mp3" };
		soundMap["buttonUp"] = Audio::Sound{ "assets/Sounds/buttonUp.mp3" };

		Audio::Device::setMasterVolume(0.05f);
	}

	void Game::initGameManager()
	{
		gameManager = std::make_shared<GameManager>(spriteMap, soundMap, this);
	}

	void Game::initCollisionManager()
	{
		collisionManager = std::make_shared<CollisionManager>(gameManager);
	}

	void Game::initAnimators()
	{
		backgroundAnimator = std::make_unique<Animator>();
		backgroundAnimator->addFrameAnimation(spriteMap["space"], 1.f, 0, spriteMap["space"]->Frames() - 1, []() {return true; });
	}

	void Game::initButtons()
	{
		int sizeX = 150;
		int sizeY = 75;
		int posY = 25;

		buttons.push_back(std::make_shared<Button>(spriteMap["startButton"], soundMap, ScreenWidth / 2 - sizeX / 2, ScreenHeight / 2 - sizeY / 2 + posY, sizeX, sizeY, [this] { initGame(); }));
		buttons.push_back(std::make_shared<Button>(spriteMap["scoreButton"], soundMap, ScreenWidth / 2 - sizeX / 2, ScreenHeight / 2 - sizeY / 2 + sizeY + posY, sizeX, sizeY, [this] {}));
		buttons.push_back(std::make_shared<Button>(spriteMap["exitButton"], soundMap, ScreenWidth / 2 - sizeX / 2, ScreenHeight / 2 - sizeY / 2 + sizeY * 2 + posY, sizeX, sizeY, [this] {std::exit(0); }));
		//buttons.push_back(std::make_shared<Button>(spriteMap["creditButton"], ScreenWidth / 2 - sizeX / 2 + 125, ScreenHeight / 2 - sizeY / 2 + sizeY * 2 + posY, 100, 75, [this] { }));
	}

	void Game::initGame()
	{
		currentGameState = GameState::GAME;
		
		initGameManager();
		initCollisionManager();
		initAnimators();

		
	}
	
	void Game::Init()
	{

		restart = false;
		
		currentGameState = GameState::MENU;

		initSprites();
		initSounds();
		initButtons();
	}

	void Game::Restart()
	{
		currentGameState = GameState::MENU;

		soundMap["music"].replay();

		initSprites();

		gameManager = nullptr;
		collisionManager = nullptr;
		backgroundAnimator = nullptr;
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

		if (restart) Restart(); restart = false;
#ifdef _DEBUG
		DebugContol(deltaTime);

		if(currentGameState == GameState::GAME)
			collisionManager->renderDEBUG(*screen);

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
		switch (currentGameState)
		{
		case GameState::MENU:
			for (const auto& button : buttons) button->CheckClick(mousePosition, wasMouseDown, wasMouseUp);
				break;
		case GameState::GAME:
			gameManager->update(deltaTime);

			collisionManager->checkCollision(deltaTime);

			backgroundAnimator->update(deltaTime);
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

		spriteMap["space"]->DrawScaled(0, 0, ScreenWidth, ScreenHeight, screen);

		switch (currentGameState)
		{
		case GameState::MENU:
			for (const auto& button : buttons)
				button->render(screen);
			spriteMap["logo"]->DrawScaled(175, 10, ScreenWidth - 350, 225, screen);
			break;
		case GameState::GAME:
			gameManager->render(screen);
			break;
		}
	}
	void Game::DebugContol(float deltaTime)
	{
		if (isKeyUp('r'))
		{
			Restart();
		}
		else if (isKeyUp('u'))
		{
			//Upgrade
		}
	}
};