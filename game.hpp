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

#include "Player.hpp"
#include "GameManager.hpp"
#include "CollisionManager.hpp"
#include "Button.hpp"
#include <Audio/Device.hpp>
#include "EventBus.hpp"

enum class GameState
{
	NONE, MENU, GAME, SCORE
};

struct RunData
{
	int spriteIndex; //Index in an array

	int saveTimeDay;
	int saveTimeMonth;
	int saveTimeYear;
	int saveTimeHours;
	int saveTimeMinutes;

	int sheepScore;

	float runTime;
};

namespace Tmpl8 {

class Surface;
class Game : public Listener
{
public:
	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Restart();
	void Shutdown();
	void Tick( float deltaTime );
	void MouseWheelScrolled(int y) { if (currentState == GameState::SCORE) toScroll = static_cast<float>(y); }
	void MouseUp(int button) { wasMouseUp = true; }
	void MouseDown(int button) { wasMouseDown = true; }
	void MouseMove(int x, int y) { mousePosition = { x, y }; }
	void KeyUp(int key) { /*upButtons.insert(key); holdButtons.erase(key); */ if (key < 256) keys.reset(key); }
	void KeyDown(int key) { /*if(!isKeyHold(key)) downButtons.insert(key);*/ if (key < 256) keys.set(key); }

	static bool isKeyDown(int key);
	static bool isKeyHold(int key);
	static bool isKeyUp(int key);	

	bool restart = false;
private:

	static std::bitset<256> pressedButtons;
	static std::bitset<256> releasedButtons;
	static std::bitset<256> heldButtons;
	std::bitset<256> keys;

	Surface* screen;

	GameState currentState = GameState::NONE;

	RunData runData;
	std::vector<RunData> runDataVector;

	std::shared_ptr<GameManager> gameManager;
	std::shared_ptr<CollisionManager> collisionManager;

	std::unordered_map<std::string, std::shared_ptr<Sprite>> spriteMap;
	std::unordered_map<std::string, Audio::Sound> soundMap;

	std::unique_ptr<Animator> animator;

	std::vector<std::shared_ptr<Button>> buttons;
	Tmpl8::vec2i mousePosition;
	bool wasMouseDown, wasMouseUp;

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
	float scrollButtonSpeed = 75.f;
	float scrollWheelSpeed = 20.f;
	int scrolled = 0;
	

	float gameTimer = 0.f;

	void loadData();
	void saveData();

	void setState(GameState state);

	void initSprites();
	void initSounds();
	void initGameManager();
	//void initEvents();
	void initCollisionManager();
	void initAnimators();
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