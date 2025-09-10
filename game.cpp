#include "game.h"
#include "surface.h"
#include <cstdio> //printf

namespace Tmpl8
{
	static Sprite ship(new Surface("assets/airship.png"), 1);
	static Sprite space(new Surface("assets/space.png"), 1);

	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------

	void Game::initPlayer()
	{
		player = std::make_shared<Player>(&ship, 376.f, 220.f, 32, 32);
	}
	
	void Game::Init()
	{
		initPlayer();
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
		render(screen);
	}

	void Game::update(float deltaTime)
	{
		updateControls(deltaTime);

		player->update(deltaTime);
	}

	void Game::render(Tmpl8::Surface* screen)
	{
		screen->Clear(0);

		space.Draw(screen, 0, 0);
		player->render(screen);
	}

	void Game::updateControls(float deltaTime)
	{
		//Movement
		if (isKeyPressed('w'))
		{
			player->move(deltaTime);
			std::cout << "pressed\n";
		}
		
		//Rotation
		if(isKeyPressed('a'))
			player->rotate(-1.f);
			
		if(isKeyPressed('d'))
			player->rotate(1.f);
			
		//Stop
		if (!isKeyPressed('w'))
			player->stop(deltaTime);
		
	}
	bool Game::isKeyPressed(int key)
	{
		return buttons.find(key) != buttons.end();
	}
};