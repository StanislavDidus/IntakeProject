#include "game.hpp"
#include "surface.h"
#include "CollisionHelper.hpp"
#include <cassert>
#include <sstream>
#include <fstream>
#include "Random.hpp"

#include <input/Input.hpp>

using namespace input;

namespace Tmpl8
{
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	 
	// -- Initializers -- //
	void Game::initAssets()
	{
		assetManager = std::make_shared<AssetManager>();
		assetManager->loadSprites();
		assetManager->loadSounds();
	}

	void Game::initGameManager()
	{
		gameManager = std::make_shared<GameManager>(assetManager);
	}

	void Game::initCollisionManager()
	{
		collisionManager = std::make_shared<CollisionManager>();
	}

	void Game::initAnimators()
	{
		animator = std::make_unique<Animator>();
		animator->addFrameAnimation(assetManager->getSprite(SpriteName::SPACE), 1.f, 0, assetManager->getSprite(SpriteName::SPACE)->Frames() - 1, []() {return true; });
	}

	void Game::initInput()
	{
		//Debug keys
		//Restart
		Input::addButtonCallback("Restart", [](std::span<const GamepadStateTracker>, const KeyboardStateTracker& keyboard, const MouseStateTracker&)
			{
				bool r = keyboard.isKeyPressed(input::Keyboard::Key::R);
				return r;
			});

		//Upgrade ship
		Input::addButtonCallback("Upgrade", [](std::span<const GamepadStateTracker>, const KeyboardStateTracker& keyboard, const MouseStateTracker&)
			{
				bool u = keyboard.isKeyPressed(input::Keyboard::Key::U);
				return u;
			});

		//Kill all objects
		Input::addButtonCallback("Kill_All", [](std::span<const GamepadStateTracker>, const KeyboardStateTracker& keyboard, const MouseStateTracker&)
			{
				bool k = keyboard.isKeyPressed(input::Keyboard::Key::K);
				return k;
			});

		//Mouse wheel
		//Kill all objects
		Input::addAxisCallback("Mouse_Wheel", [](std::span<const GamepadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouse)
			{
				auto m = static_cast<float>(mouse.scrollWheelDelta);
				return std::clamp(m, -1.f, 1.f);
			});

		Input::addAxisCallback("Scroll", [](std::span<const GamepadStateTracker>, const KeyboardStateTracker& keyboard, const MouseStateTracker&)
			{
				const auto& w = keyboard.getLastState().W ? -1.f : 0.f;
				const auto& s = keyboard.getLastState().S ? 1.f : 0.f;
				return std::clamp(w + s, -1.f, 1.f);
			});

		//Left mouse button down
		Input::addButtonCallback("Mouse_Down", [](std::span<const GamepadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouse)
			{
				bool m = mouse.leftButton == input::ButtonState::Pressed;
				return m;
			});

		//Left mouse button up
		Input::addButtonCallback("Mouse_Up", [](std::span<const GamepadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouse)
			{
				bool m = mouse.leftButton == input::ButtonState::Released;
				return m;
			});
	}

	void Game::initButtons()
	{
		float sizeX = 150;
		float sizeY = 75;
		float posY = 25;

		float ScreenW = static_cast<float>(ScreenWidth);
		float ScreenH = static_cast<float>(ScreenHeight);

		buttons.push_back(std::make_shared<Button>(assetManager->getSprite(SpriteName::BUTTON_START), assetManager, Tmpl8::vec2{ ScreenW / 2.f - sizeX / 2, ScreenH / 2 - sizeY / 2 + posY }, Tmpl8::vec2{ sizeX, sizeY }, [this] { setState(GameState::GAME); }));
		buttons.push_back(std::make_shared<Button>(assetManager->getSprite(SpriteName::BUTTON_SCORE), assetManager, Tmpl8::vec2{ ScreenW / 2 - sizeX / 2, ScreenH / 2 - sizeY / 2 + sizeY + posY }, Tmpl8::vec2{ sizeX, sizeY }, [this] { setState(GameState::SCORE);  }));
		buttons.push_back(std::make_shared<Button>(assetManager->getSprite(SpriteName::BUTTON_EXIT), assetManager, Tmpl8::vec2{ ScreenW / 2 - sizeX / 2, ScreenH / 2 - sizeY / 2 + sizeY * 2 + posY }, Tmpl8::vec2{ sizeX, sizeY }, [this] { Shutdown();  std::exit(0); }));

		// -- Score Menu -- //

		//Clear all player's data on click
		buttons.push_back(std::make_shared<Button>(assetManager->getSprite(SpriteName::BUTTON_DELETE), assetManager, Tmpl8::vec2{ ScreenW - 100.f - 50.f, ScreenH - sizeY - 25.f - sizeY }, Tmpl8::vec2{ sizeX, sizeY }, [this] { runDataVector.clear(); }));

		//Return to main menu
		buttons.push_back(std::make_shared<Button>(assetManager->getSprite(SpriteName::BUTTON_CANCEL), assetManager, Tmpl8::vec2{ ScreenW - 100.f - 50.f, ScreenH - sizeY - 25.f }, Tmpl8::vec2{ sizeX, sizeY }, [this] {setState(GameState::MENU); }));
	
		//Toggles
		toggles.push_back(std::make_shared<Toggle>
			(
				assetManager->getSprite(SpriteName::UI_TOGGLE_SOUND),
				Tmpl8::vec2{ScreenW - 60.f, 10.f},
				Tmpl8::vec2{50.f, 50.f},
				[this] {Audio::Device::setMasterVolume(0.1f); },
				[this] {Audio::Device::setMasterVolume(0.f); },
				false
			));
	}

	// -- First init of the game -- //
	void Game::Init()
	{
		// -- Read player's data -- //
		loadData();

		// -- Initialize basic game components -- //
		initAssets();
		initAnimators();
		initButtons();
		initInput();

		// -- Init menu -- //
		setState(GameState::MENU);

		initBgAsteroids();
	}

	void Game::Restart()
	{
		setState(GameState::MENU);

		assetManager->getSound(SoundName::MENU_MUSIC).replay();

		assetManager->loadSprites();
	}

	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
		saveData();
	}

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		deltaTime /= 1000.f;

		fpsCheckTimer += deltaTime;

		frames++;

		if (fpsCheckTimer >= 1.f)
		{
			
			fpsCounter = static_cast<int>(round(frames / fpsCheckTimer));

			frames = 0;
			fpsCheckTimer = 0.f;
		}

		update(deltaTime);
		render(*screen);

		if (restart) Restart(); restart = false;
#ifdef _DEBUG
		DebugContol(deltaTime);

		if (currentState == GameState::GAME)
			collisionManager->renderDEBUG(gameManager, *screen);

		//FPS Counter
		std::stringstream ss;
		ss << "FPS: " + std::to_string(fpsCounter);
		screen->PrintScaled(&ss.str()[0], 50, 100, 2, 2, 0xffffff);
#else 
		std::stringstream ss;
		ss << "FPS: " + std::to_string(fpsCounter);
		screen->PrintScaled(&ss.str()[0], 50, 100, 2, 2, 0xffffff);
#endif
	}

	void Game::update(float deltaTime)
	{
		Input::update();

		// -- Controls -- //
		wasMouseDown = Input::getButton("Mouse_Down");
		wasMouseUp = Input::getButton("Mouse_Up");
		mousePosition = { static_cast<int>(Input::getMouseX()), static_cast<int>(Input::getMouseY()) };

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
	}

	void Game::render(Tmpl8::Surface& screen)
	{
		screen.Clear(0);

		animator->setAnimationFrame(assetManager->getSprite(SpriteName::SPACE));

		assetManager->getSprite(SpriteName::SPACE)->DrawScaled(0, 0, ScreenWidth - 1, ScreenHeight - 1, screen);

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
		if (Input::getButton("Restart"))
		{
			Restart();
		}
		else if (Input::getButton("Kill_All"))
		{
			EventBus::Get().push<EventType::KILL_ALL>();
		}
		else if (Input::getButton("Upgrade"))
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

			//Generate random position
			vec2 position = Random::randomVector2(vec2{ 0.f }, vec2{ ScreenWidth, ScreenHeight });

			createBgAsteroid(position, vec2{ static_cast<float>(width), static_cast<float>(height) });
		}
	}

	void Game::spawnBgAsteroid()
	{
		int width = Random::randomRange(64, 256);
		int height = width;

		//Spawn on the left or on the right side of the screen
		float x = static_cast<float>(Random::ff() ? -width : ScreenWidth - 1);
		//Spawn above or beneath the screen
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
				assetManager->getSprite(SpriteName::ASTEROID),
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
			//I don't need to output ERROR here because the program expects that this file might not exist.
			//For example when you run this game for the first time
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