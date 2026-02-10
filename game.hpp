#pragma once

#include <set>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <string>
#include <sstream>
#include <bitset>
#include <chrono>
#include <ctime>

#include "AssetManager.hpp"
#include "Player.hpp"
#include "GameManager.hpp"
#include "CollisionManager.hpp"
#include "Button.hpp"
#include <Audio/Device.hpp>
#include "EventBus.hpp"
#include "Toggle.hpp"
#include "EventSubscriber.hpp"

#include <SDL2/SDL.h>

enum class GameState
{
	NONE, MENU, GAME, SCORE
};

/// <summary>
/// Structure that is used to store run data after of a player after each game.
/// </summary>
struct RunData
{
	int spriteIndex; //Index in an array

	std::chrono::time_point<std::chrono::system_clock> time_point;
	int saveTimeHours;
	int saveTimeMinutes;

	int sheepScore;

	float runTime;
};

namespace Tmpl8 {

class Surface;
class Game
{
public:
	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Restart();
	void Shutdown();
	void Tick( float deltaTime );
	void MouseWheelScrolled(int y) { /*if (currentState == GameState::SCORE) toScroll += static_cast<float>(y);*/ }
	void MouseUp(int button) { /*if (button == SDL_BUTTON_LEFT) wasMouseUp = true;*/ }
	void MouseDown(int button) { /*if( button == SDL_BUTTON_LEFT) wasMouseDown = true;*/ }
	void MouseMove(int x, int y) {/* mousePosition = { x, y }; */}
	void KeyUp(int key) { /*if (key < 256) keys.reset(key);*/ }
	void KeyDown(int key) {/* if (key < 256) keys.set(key);*/ }

	bool restart = false;
private:

	Surface* screen;

	GameState currentState = GameState::NONE;

	//Run information(score, time) of the current run
	RunData runData;

	//Previous runs are stored here
	std::vector<RunData> runDataVector;

	std::shared_ptr<GameManager> gameManager;
	std::shared_ptr<CollisionManager> collisionManager;

	std::shared_ptr<AssetManager> assetManager;

	std::unique_ptr<Animator> animator;

	//Events
	//ID used to unsubscribe from an event
	int caller_id;

	//UI Elements
	std::vector<std::shared_ptr<Toggle>> toggles;

	std::vector<std::shared_ptr<Button>> buttons;
	Tmpl8::vec2i mousePosition;
	bool wasMouseDown, wasMouseUp;

	//FPS
	float fpsCheckTimer = 0.f;

	int fpsCounter = 0;
	int frames = 0;

	//Score UI
	int scoreOffSetX = 25;
	int scoreOffSetY = 25;

	int scoreXSpace = 5;
	int scoreYSpace = 15;

	int iconWidth = 60;
	int iconHeight = 60;

	int scoreTextScale = 3;

	float inverseMouseWheel = -1.f; // -1.f - scroll down to move down, 1.f - scroll up to move down
	float toScroll = 0.f;
	float scrollButtonSpeed = 1000.f;
	float scrollWheelSpeed = 15.f;
	int scrolled = 0;

	//UI player data
	int playerHealth = 0;
	bool isPlayerWeaponUpgraded = false;
	bool isPlayerEngineUpgraded = false;

	float gameTimer = 0.f;

	// -- Background asteroids -- //
	//Asterpoid and frame
	std::unordered_map<std::shared_ptr<Object>, int> bgAsteroids;

	void initBgAsteroids();
	void spawnBgAsteroid();
	void createBgAsteroid(const vec2& position, const vec2& size);
	void updateAsteroids(float deltaTime);

	float bgAsteroidsSpawnTime = 5.f;
	float bgAsteroidsSpawnTimer = 5.f;
	float bgAsteroidMaxSpeed = 100.f;
	float bgAsteroidAcceleration = 35.f;
	float bgAsteroidRotation = 15.f;
	// -- -- //

	void loadData();
	void saveData();

	void setState(GameState state);

	void initAssets();
	void initGameManager();
	void initCollisionManager();
	void initAnimators();
	void initInput();
	void initButtons();

	void enterState(GameState state);
	void exitState(GameState state);

	void update(float deltaTime);
	void render(Tmpl8::Surface& screen);

	void updateMenu(float deltaTime);
	void updateGame(float deltaTime);
	void updateScore(float deltaTime);

	void renderMenu(Tmpl8::Surface& screen);
	void renderGame(Tmpl8::Surface& screen);
	void renderScore(Tmpl8::Surface& screen);

	void DebugContol(float deltaTime);
};

}; // namespace Tmpl8