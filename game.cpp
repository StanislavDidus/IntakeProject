#include "game.hpp"
#include "surface.h"
#include "CollisionHelper.hpp"
#include <cassert>
#include <sstream>
#include <fstream>
#include "Random.hpp"

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

	void Game::loadSprite(SpriteName spriteName, const std::string& path, int frames)
	{
		spriteMap[spriteName] = std::make_shared<Sprite>(new Surface(&path[0]), frames);
	}

	void Game::loadSound(SoundName soundName, const std::string& path, Audio::Sound::Type type)
	{
		soundMap[soundName] = Audio::Sound{ path, type };
	}

	void Game::initSprites()
	{
		//Ship
		loadSprite(SpriteName::SHIP, "assets/ship.png", 4);
		loadSprite(SpriteName::SHIP_ENGINE, "assets/engine.png", 1);
		loadSprite(SpriteName::SHIP_ENGINE_UPGRADED, "assets/engine1.png", 1);
		loadSprite(SpriteName::SHIP_ENGINE_EFFECT, "assets/engineEffect.png", 7);
		loadSprite(SpriteName::SHIP_ENGINE_UPGRADED_EFFECT, "assets/engineEffect1.png", 8);
		loadSprite(SpriteName::SHIP_EXPLOSION, "assets/explosion.png", 11);
		loadSprite(SpriteName::SHIP_WEAPON, "assets/weapon.png", 7);
		loadSprite(SpriteName::SHIP_WEAPON_UPGRADED, "assets/weapon1.png", 12);
		loadSprite(SpriteName::SHIP_HIT_EFFECT, "assets/hitEffect.png", 7);
		loadSprite(SpriteName::SHIP_BULLET, "assets/bullet.png", 4);
		loadSprite(SpriteName::SHIP_BULLET_UPGRADED, "assets/bullet1.png", 10);
		loadSprite(SpriteName::SHIP_BULLET_UPGRADED_TRAIL, "assets/bulletEffect.png", 4);

		// Objects
		loadSprite(SpriteName::SHEEP, "assets/sheep.png", 1);
		loadSprite(SpriteName::ASTEROID, "assets/asteroid.png", 3);
		loadSprite(SpriteName::ASTEROID_EXPLOSION, "assets/explosion1.png", 10);
		loadSprite(SpriteName::SPACE, "assets/space.png", 2);
		loadSprite(SpriteName::UPGRADE, "assets/upgrade.png", 1);

		// UI
		loadSprite(SpriteName::BUTTON_START, "assets/startButton.png", 3);
		loadSprite(SpriteName::BUTTON_SCORE, "assets/scoreButton.png", 3);
		loadSprite(SpriteName::BUTTON_EXIT, "assets/exitButton.png", 3);
		loadSprite(SpriteName::BUTTON_DELETE, "assets/deleteButton.png", 3);
		loadSprite(SpriteName::BUTTON_CANCEL, "assets/cancelButton.png", 3);
		loadSprite(SpriteName::UI_LOGO, "assets/logo.png", 1);
		loadSprite(SpriteName::UI_CLOCK, "assets/clock.png", 1);
		loadSprite(SpriteName::UI_SMILEYS, "assets/smileys.png", 20);
		loadSprite(SpriteName::UI_LABEL, "assets/label.png", 1);
	}

	void Game::initSounds()
	{
		loadSound(SoundName::MENU_MUSIC, "assets/Sounds/music.mp3", Audio::Sound::Type::Stream);
		auto& menuMusic = soundMap[SoundName::MENU_MUSIC];
		menuMusic.setLooping(true);
		menuMusic.replay();

		loadSound(SoundName::SHIP_SHOOT, "assets/Sounds/shoot.mp3");
		loadSound(SoundName::SHIP_SHOOT_UPGRADED, "assets/Sounds/shoot1.mp3");
		loadSound(SoundName::SHIP_CHARGE, "assets/Sounds/charge.mp3");
		loadSound(SoundName::SHIP_UPGRADE_WEAPON, "assets/Sounds/upgradeWeapon.mp3");
		loadSound(SoundName::SHIP_UPGRADE_ENGINE, "assets/Sounds/upgradeEngine.mp3");
		loadSound(SoundName::SHIP_DAMAGED, "assets/Sounds/shipDamaged.mp3");
		loadSound(SoundName::SHIP_DESTROYED, "assets/Sounds/shipDestroyed.mp3");
		loadSound(SoundName::BULLET_HIT, "assets/Sounds/hitEffect.mp3");
		loadSound(SoundName::ASTEROID_DESTROYED, "assets/Sounds/asteroidDestroyed.mp3");
		loadSound(SoundName::GAME_OVER, "assets/Sounds/gameOver.mp3");
		loadSound(SoundName::BUTTON_COVER, "assets/Sounds/buttonCover.mp3");
		loadSound(SoundName::BUTTON_UP, "assets/Sounds/buttonUp.mp3");

		Audio::Device::setMasterVolume(0.1f);
	}

	void Game::initGameManager()
	{
		gameManager = std::make_shared<GameManager>(spriteMap, soundMap);
	}

	void Game::initCollisionManager()
	{
		collisionManager = std::make_shared<CollisionManager>();
	}

	void Game::initAnimators()
	{
		animator = std::make_unique<Animator>();
		animator->addFrameAnimation(spriteMap[SpriteName::SPACE], 1.f, 0, spriteMap[SpriteName::SPACE]->Frames() - 1, []() {return true; });

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

		buttons.push_back(std::make_shared<Button>(spriteMap[SpriteName::BUTTON_START], soundMap, Tmpl8::vec2{ ScreenW / 2.f - sizeX / 2, ScreenH / 2 - sizeY / 2 + posY }, Tmpl8::vec2{ sizeX, sizeY }, [this] { setState(GameState::GAME); }));
		buttons.push_back(std::make_shared<Button>(spriteMap[SpriteName::BUTTON_SCORE], soundMap, Tmpl8::vec2{ ScreenW / 2 - sizeX / 2, ScreenH / 2 - sizeY / 2 + sizeY + posY }, Tmpl8::vec2{ sizeX, sizeY }, [this] { setState(GameState::SCORE);  }));
		buttons.push_back(std::make_shared<Button>(spriteMap[SpriteName::BUTTON_EXIT], soundMap, Tmpl8::vec2{ ScreenW / 2 - sizeX / 2, ScreenH / 2 - sizeY / 2 + sizeY * 2 + posY }, Tmpl8::vec2{ sizeX, sizeY }, [this] { Shutdown();  std::exit(0); }));

		// -- Score Menu -- //

		//Clear all player's data on click
		buttons.push_back(std::make_shared<Button>(spriteMap[SpriteName::BUTTON_DELETE], soundMap, Tmpl8::vec2{ ScreenW - 100.f - 50.f, ScreenH - sizeY - 25.f - sizeY }, Tmpl8::vec2{ sizeX, sizeY }, [this] { runDataVector.clear(); }));

		//Return to main menu
		buttons.push_back(std::make_shared<Button>(spriteMap[SpriteName::BUTTON_CANCEL], soundMap, Tmpl8::vec2{ ScreenW - 100.f - 50.f, ScreenH - sizeY - 25.f }, Tmpl8::vec2{ sizeX, sizeY }, [this] {setState(GameState::MENU); }));
	}

	// -- First init of the game -- //
	void Game::Init()
	{
		// -- Read player's data -- //
		loadData();

		//Initialize basic game components
		initSprites();
		initAnimators();
		initSounds();
		initButtons();

		//Init menu
		setState(GameState::MENU);

		initBgAsteroids();
	}

	void Game::Restart()
	{
		setState(GameState::MENU);

		soundMap[SoundName::MENU_MUSIC].replay();

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
		animator->update(deltaTime);

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

		animator->setAnimationFrame(spriteMap[SpriteName::SPACE]);

		spriteMap[SpriteName::SPACE]->DrawScaled(0, 0, ScreenWidth - 1, ScreenHeight - 1, screen);

		/*Tmpl8::vec2 p0 = { 0.f, 0.f };
		Tmpl8::vec2 p2 = {ScreenWidth, 0.f};
		Tmpl8::vec2 p1 = { ScreenWidth, ScreenHeight };
		Tmpl8::vec2 p = { 0.f, 0.f };
		Edge e = { p0, p1, p2, p };*/

		//for (int y = p.y; y < ScreenHeight; y++)
		//{
		//	for (int x = p.x; x < ScreenWidth; x++)
		//	{
		//		if (e.inside())
		//		{
		//			//screen.Plot<false>(x, y, Tmpl8::RedMask);
		//		}

		//		e.stepX();
		//	}

		//	e.stepY();
		//}

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
		else if (isKeyUp('u'))
		{
			auto player = gameManager->getPlayer();
			if (player) player->upgradeEngine();
		}
	}

	void Game::initBgAsteroids()
	{
		int bgAsteroidNumber = 1;

		for (int i = 0; i < bgAsteroidNumber; i++)
		{
			int width = Random::randomRange(64, 256);
			int height = width;

			vec2 position = Random::randomVector2(vec2{ 0.f }, vec2{ ScreenWidth, ScreenHeight });

			createBgAsteroid(position, vec2{ static_cast<float>(width), static_cast<float>(height) });
		}
	}

	void Game::spawnBgAsteroid()
	{
		int width = Random::randomRange(64, 256);
		int height = width;

		float x = static_cast<float>(Random::ff() ? -width : ScreenWidth - 1);
		float y = static_cast<float>(Random::ff() ? -height : ScreenHeight - 1);

		createBgAsteroid(Tmpl8::vec2{ x, y }, Tmpl8::vec2{ static_cast<float>(width), static_cast<float>(height) });
	}

	void Game::createBgAsteroid(const vec2& position, const vec2& size)
	{
		Tmpl8::vec2 randomTarget = { Random::randomVector2({100.f, 50.f}, {ScreenWidth - 100.f, ScreenHeight - 50.f}) };
		Tmpl8::vec2 direction = randomTarget - position;
		direction.normalize();

		int frame = Random::randomRange(0, 2);

		auto ast = std::make_shared<Object>
			(
				spriteMap[SpriteName::ASTEROID],
				position,
				size
			);

		ast->setDirection(direction);
		ast->setMaxSpeed(bgAsteroidMaxSpeed);
		ast->setAngle(Random::randomRange(0.f, 360.f));
		ast->setAcceleration(Tmpl8::vec2{ bgAsteroidAcceleration * direction.x, bgAsteroidAcceleration * direction.y });

		bgAsteroids[ast] = frame;
	}

	void Game::updateAsteroids(float deltaTime)
	{
		bgAsteroidsSpawnTimer -= deltaTime;
		if (bgAsteroidsSpawnTimer <= 0.f)
		{
			spawnBgAsteroid();
			bgAsteroidsSpawnTimer = bgAsteroidsSpawnTime;
		}

		for (auto it = bgAsteroids.begin(); it != bgAsteroids.end(); )
		{
			auto& ast = (*it).first;

			ast->move(deltaTime);
			ast->update(deltaTime);

			ast->setAngle(ast->getAngle() + bgAsteroidRotation * deltaTime);

			auto pos = ast->getPosition();
			auto size = ast->getSize();

			if (pos.x + size.x < 0.f ||
				pos.x >= ScreenWidth ||
				pos.y + size.y < 0.f ||
				pos.y >= ScreenHeight)
			{
				it = bgAsteroids.erase(it);
			}
			else
				++it;
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