#include "game.hpp"
#include "surface.h"
#include "CollisionHelper.hpp"
#include <cassert>
#include <sstream>
#include <fstream>

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
		spriteMap["explosion"] = std::make_shared<Sprite>(new Surface("assets/explosion.png"), 11);
		spriteMap["explosion1"] = std::make_shared<Sprite>(new Surface("assets/explosion1.png"), 10);
		spriteMap["weapon"] = std::make_shared<Sprite>(new Surface("assets/weapon.png"), 7);
		spriteMap["weapon1"] = std::make_shared<Sprite>(new Surface("assets/weapon1.png"), 12);
		spriteMap["hitEffect"] = std::make_shared<Sprite>(new Surface("assets/hitEffect.png"), 7);
		spriteMap["bullet"] = std::make_shared<Sprite>(new Surface("assets/bullet.png"), 4);
		spriteMap["bullet1"] = std::make_shared<Sprite>(new Surface("assets/bullet1.png"), 10);
		spriteMap["trail"] = std::make_shared<Sprite>(new Surface("assets/bulletEffect.png"), 4);
		//Object
		spriteMap["sheep"] = std::make_shared<Sprite>(new Surface("assets/sheep.png"), 1);
		spriteMap["space"] = std::make_shared<Sprite>(new Surface("assets/space.png"), 2);
		spriteMap["upgrade"] = std::make_shared<Sprite>(new Surface("assets/upgrade.png"), 1);	
		//UI
		spriteMap["startButton"] = std::make_shared<Sprite>(new Surface("assets/startButton.png"), 3);
		spriteMap["scoreButton"] = std::make_shared<Sprite>(new Surface("assets/scoreButton.png"), 3);
		spriteMap["exitButton"] = std::make_shared<Sprite>(new Surface("assets/exitButton.png"), 3);
		spriteMap["deleteButton"] = std::make_shared<Sprite>(new Surface("assets/deleteButton.png"), 3);
		spriteMap["cancelButton"] = std::make_shared<Sprite>(new Surface("assets/cancelButton.png"), 3);
		spriteMap["logo"] = std::make_shared<Sprite>(new Surface("assets/logo.png"), 1);
		spriteMap["clock"] = std::make_shared<Sprite>(new Surface("assets/clock.png"), 1);
		spriteMap["smileys"] = std::make_shared<Sprite>(new Surface("assets/smileys.png"), 20);
		spriteMap["label"] = std::make_shared<Sprite>(new Surface("assets/label.png"), 1);
	}

	void Game::initSounds()
	{
		auto music = Audio::Sound{ "assets/Sounds/music.mp3", Audio::Sound::Type::Stream };
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
		soundMap["shipDestroyed"] = Audio::Sound{ "assets/Sounds/shipDestroyed.mp3" };
		soundMap["asteroidDestroyed"] = Audio::Sound{ "assets/Sounds/asteroidDestroyed.mp3" };
		soundMap["gameOver"] = Audio::Sound{ "assets/Sounds/gameOver.mp3" };
		soundMap["buttonCover"] = Audio::Sound{ "assets/Sounds/buttonCover.mp3" };
		soundMap["buttonUp"] = Audio::Sound{ "assets/Sounds/buttonUp.mp3" };

		Audio::Device::setMasterVolume(0.1f);
	}

	void Game::initGameManager()
	{
		gameManager = std::make_shared<GameManager>(collisionManager, spriteMap, soundMap);
	}

	void Game::initCollisionManager()
	{
		collisionManager = std::make_shared<CollisionManager>();
	}

	void Game::initAnimators()
	{
		animator = std::make_unique<Animator>();
		animator->addFrameAnimation(spriteMap["space"], 1.f, 0, spriteMap["space"]->Frames() - 1, []() {return true; });
		
		//animator->addFrameAnimation(spriteMap["hitEffect"], 0.1f, 0, 6, [] {return true; });
		//animator->addFrameCycledAnimation(spriteMap["explosion1"], 0.1f, 0, 9, "Explosion1");
	}

	void Game::initButtons()
	{
		float sizeX = 150;
		float sizeY = 75;
		float posY = 25;

		float ScreenW = static_cast<float>(ScreenWidth);
		float ScreenH = static_cast<float>(ScreenHeight);

		buttons.push_back(std::make_shared<Button>(spriteMap["startButton"], soundMap, Tmpl8::vec2{ ScreenW / 2.f - sizeX / 2, ScreenH / 2 - sizeY / 2 + posY }, Tmpl8::vec2{ sizeX, sizeY }, [this] { setState(GameState::GAME); }));
		buttons.push_back(std::make_shared<Button>(spriteMap["scoreButton"], soundMap, Tmpl8::vec2{ ScreenW / 2 - sizeX / 2, ScreenH / 2 - sizeY / 2 + sizeY + posY }, Tmpl8::vec2{ sizeX, sizeY }, [this] { setState(GameState::SCORE);  }));
		buttons.push_back(std::make_shared<Button>(spriteMap["exitButton"], soundMap, Tmpl8::vec2{ ScreenW / 2 - sizeX / 2, ScreenH / 2 - sizeY / 2 + sizeY * 2 + posY }, Tmpl8::vec2{ sizeX, sizeY }, [this] { Shutdown();  std::exit(0); }));
		
		// -- Score Menu -- //
		
		//Clear all player's data on click
		buttons.push_back(std::make_shared<Button>(spriteMap["deleteButton"], soundMap, Tmpl8::vec2{ ScreenW - 100.f - 50.f, ScreenH - sizeY - 25.f - sizeY }, Tmpl8::vec2{sizeX, sizeY}, [this] { runDataVector.clear(); }));

		//Return to main menu
		buttons.push_back(std::make_shared<Button>(spriteMap["cancelButton"], soundMap, Tmpl8::vec2{ ScreenW - 100.f - 50.f, ScreenH - sizeY - 25.f}, Tmpl8::vec2{ sizeX, sizeY }, [this] {setState(GameState::MENU); }));
	}

	// -- First init of the game -- //
	void Game::Init()
	{
		// -- Read player's data -- //
		loadData();
		
		//Initialize basic game components
		initSprites();
		initSounds();
		initButtons();
		
		//Init menu
		setState(GameState::MENU);
	}

	void Game::Restart()
	{
		setState(GameState::MENU);

		soundMap["music"].replay();

		initSprites();
	}

	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
		// -- Save player's data -- //
		saveData();
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

		if (currentState == GameState::GAME)
			collisionManager->renderDEBUG(gameManager, *screen);

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
		switch (currentState)
		{
		case GameState::MENU:
			updateMenu(deltaTime);
			break;
		case GameState::GAME:
			updateGame(deltaTime);
			break;
		case GameState::SCORE:
			updateScore(deltaTime);
			break;
		}

		EventBus::Get().process();	

		//Update buttons
		wasMouseDown = false;
		wasMouseUp = false;

		pressedButtons = keys & ~heldButtons;
		releasedButtons = heldButtons & ~keys;
		heldButtons = keys;
	}

	void Game::render(Tmpl8::Surface& screen)
	{
		screen.Clear(0);

		spriteMap["space"]->DrawScaled(0, 0, ScreenWidth - 1, ScreenHeight - 1, screen);

		switch (currentState)
		{
		case GameState::MENU:
			renderMenu(screen);
			break;
		case GameState::GAME:
			renderGame(screen);
			break;
		case GameState::SCORE:
			renderScore(screen);
			break;
		}
	}
	
	void Game::DebugContol(float deltaTime)
	{
		if (isKeyUp('r'))
		{
			Restart();
		}
		else if (isKeyUp('k'))
		{
			EventBus::Get().push<EventType::KILL_ALL>();
		}
	}

	void Game::loadData()
	{
		std::ifstream data("data.bin", std::ios::binary);
		if (!data)
		{
			std::cerr << "ERROR: Could not open file for reading\n";
			return;
		}

		size_t numEntries = 0;

		data.read(reinterpret_cast<char*>(&numEntries), sizeof(numEntries));

		runDataVector.resize(numEntries);

		data.read(reinterpret_cast<char*>(runDataVector.data()), numEntries * sizeof(RunData));

		if (data.bad())
		{
			std::cerr << "ERROR: Could not open file for reading";
			return;
		}

		data.close();
	}

	void Game::saveData()
	{
		std::ofstream data("data.bin", std::ios::binary);
		if (!data)
		{
			std::cerr << "ERROR: Could not open file for writing\n";
			return;
		}

		auto numberOfData = std::size(runDataVector);

		data.write(reinterpret_cast<const char*>(&numberOfData), sizeof(numberOfData));
		data.write(reinterpret_cast<const char*>(runDataVector.data()), numberOfData * sizeof(RunData));

		if (data.bad())
		{
			std::cerr << "ERROR: Could not open file for writing\n";
			return;
		}

		data.close();
	}
};