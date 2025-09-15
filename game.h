#pragma once

#include <set>
#include <memory>
#include <unordered_map>
#include <string>

#include "Player.h"
#include "GameManager.h"

namespace Tmpl8 {

class Surface;
class Game
{
public:
	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Shutdown();
	void Tick( float deltaTime );
	void MouseUp( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseDown( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseMove( int x, int y ) { /* implement if you want to detect mouse movement */ }
	void KeyUp(int key) { upButtons.insert(key); holdButtons.erase(key); }
	void KeyDown(int key) { if(!isKeyHold(key)) downButtons.insert(key); }
private:
	Surface* screen;

	std::shared_ptr<Player> player;
	std::shared_ptr<GameManager> gameManager;
	
	std::set<int> downButtons;
	std::set<int> holdButtons;
	std::set<int> upButtons;

	std::unordered_map<std::string, Tmpl8::Surface*> surfaces;
	std::unordered_map<std::string, Tmpl8::Sprite*> sprites;

	void initSprites();
	void initGameManager();
	void initPlayer();

	void update(float deltaTime);
	void render(Tmpl8::Surface& screen);

	void updateControls(float deltaTime);
	bool isKeyDown(int key);
	bool isKeyHold(int key);
	bool isKeyUp(int key);
};

}; // namespace Tmpl8