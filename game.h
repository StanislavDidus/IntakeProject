#pragma once

#include <set>
#include <memory>

#include "Player.h"

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
	void KeyUp(int key) { buttons.erase(key); }
	void KeyDown(int key) { buttons.insert(key); }
private:
	Surface* screen;

	std::shared_ptr<Player> player;
	
	std::set<int> buttons;

	void initPlayer();

	void update(float deltaTime);
	void render(Tmpl8::Surface* screen);

	void updateControls(float deltaTime);
	bool isKeyPressed(int key);
};

}; // namespace Tmpl8