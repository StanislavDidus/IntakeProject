#pragma once

#include <set>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <string>
#include <sstream>
#include <bitset>

#include "Player.hpp"
#include "GameManager.hpp"
#include "CollisionManager.hpp"
#include "Button.hpp"
#include <Audio/Device.hpp>
#include "EventBus.hpp"

enum class GameState
{
	MENU, GAME
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

	GameState currentGameState = GameState::MENU;

	std::shared_ptr<GameManager> gameManager;
	std::shared_ptr<CollisionManager> collisionManager;

	std::unordered_map<std::string, std::shared_ptr<Sprite>> spriteMap;
	std::unordered_map<std::string, Audio::Sound> soundMap;

	std::unique_ptr<Animator> backgroundAnimator;

	std::vector<std::shared_ptr<Button>> buttons;
	Tmpl8::vec2i mousePosition;
	bool wasMouseDown, wasMouseUp;

	void initSprites();
	void initSounds();
	void initGameManager();
	void initEvents();
	void initCollisionManager();
	void initAnimators();
	void initButtons();
	void initGame();

	void update(float deltaTime);
	void render(Tmpl8::Surface& screen);

	void DebugContol(float deltaTime);
};

}; // namespace Tmpl8