#pragma once

#include <set>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <string>
#include <sstream>
#include <bitset>

#include "Player.h"
#include "GameManager.h"
#include "CollisionManager.h"
#include "Button.h"

enum GameState
{
	MENU, GAME
};

namespace Tmpl8 {

class Surface;
class Game
{
public:
	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Shutdown();
	void Tick( float deltaTime );
	void MouseUp(int button) { wasMouseUp = true; }
	void MouseDown(int button) { wasMouseDown = true; }
	void MouseMove(int x, int y) { mousePosition = { x, y }; }
	void KeyUp(int key) { /*upButtons.insert(key); holdButtons.erase(key); */ if (key >= 256) return; keys.reset(key); }
	void KeyDown(int key) { /*if(!isKeyHold(key)) downButtons.insert(key);*/ if (key >= 256) return; keys.set(key); }

	static bool isKeyDown(int key);
	static bool isKeyHold(int key);
	static bool isKeyUp(int key);

	
private:

	static std::bitset<256> pressedButtons;
	static std::bitset<256> releasedButtons;
	static std::bitset<256> heldButtons;
	std::bitset<256> keys;

	Surface* screen;

	GameState state;

	std::shared_ptr<GameManager> gameManager;
	std::shared_ptr<CollisionManager> colManager;

	std::unordered_map<std::string, std::shared_ptr<Sprite>> sprites;

	std::unique_ptr<Animator> bgAnimator;

	std::vector<std::shared_ptr<Button>> buttons;
	Tmpl8::vec2i mousePosition;
	bool wasMouseDown, wasMouseUp;

	void initSprites();
	void initGameManager();
	void initCollisionManager();
	void initAnimators();
	void initButtons();
	void initGame();

	void update(float deltaTime);
	void render(Tmpl8::Surface& screen);

	void updateControls(float deltaTime);
};

}; // namespace Tmpl8