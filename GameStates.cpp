#include "game.hpp"
#include "Random.hpp"
#include <fstream>

namespace Tmpl8
{
	void Game::setState(GameState state)
	{
		if (currentState != state)
		{
			exitState(currentState);

			currentState = state;

			enterState(currentState);
		}
	}

	void Game::enterState(GameState state)
	{
		switch (state)
		{
		case GameState::MENU:
			//initGameManager();
			
			break;
		case GameState::GAME:
			// -- Init game -- //

			EventBus::Get().subscribe<EventType::GAMEOVER>(this, [this] {restart = true; soundMap[SoundName::GAME_OVER].replay(); });

			initCollisionManager();
			initGameManager();
			gameManager->initPlayer();
			

			gameTimer = 0.f;
			break;
		case GameState::SCORE:
			scrolled = 0;
			break;
		}
	}

	void Game::exitState(GameState state)
	{
		switch (state)
		{
		case GameState::MENU:
			//gameManager = nullptr;
			break;
		case GameState::GAME:
		{//Add braces because case: does not have it own scope 
			// -- Save data on game finish -- //

			runData.spriteIndex = Random::randomRange(0, 19);

			std::time_t t = std::time(0);
			std::tm* now = std::localtime(&t);

			runData.saveTimeDay = now->tm_mday;
			runData.saveTimeMonth = now->tm_mon + 1;
			runData.saveTimeYear = now->tm_year + 1900;
					
			runData.saveTimeHours = now->tm_hour;
			runData.saveTimeMinutes = now->tm_min;

			runData.sheepScore = gameManager->getNumberOfSheep();

			runData.runTime = gameTimer;

			runDataVector.push_back(runData);

			// -- Destroy all game objects -- //
			EventBus::Get().unsubscribe<EventType::GAMEOVER>(this);

			gameManager = nullptr;
			collisionManager = nullptr;
			break;
		}
		case GameState::SCORE:
			break;
		}
	}

	void Game::updateMenu(float deltaTime)
	{
		updateAsteroids(deltaTime);

		//gameManager->update(deltaTime);
		
		buttons[0]->CheckClick(mousePosition, wasMouseDown, wasMouseUp);
		buttons[1]->CheckClick(mousePosition, wasMouseDown, wasMouseUp);
		buttons[2]->CheckClick(mousePosition, wasMouseDown, wasMouseUp);
	}

	void Game::updateGame(float deltaTime)
	{
		gameTimer += deltaTime;

		gameManager->update(deltaTime);

		collisionManager->checkCollision(gameManager, deltaTime);

		animator->update(deltaTime);
	}

	void Game::updateScore(float deltaTime)
	{
		updateAsteroids(deltaTime);

		buttons[3]->CheckClick(mousePosition, wasMouseDown, wasMouseUp);
		buttons[4]->CheckClick(mousePosition, wasMouseDown, wasMouseUp);

		if (isKeyHold('w'))
		{
			toScroll += scrollButtonSpeed * deltaTime;
		}
		else if (isKeyHold('s'))
		{
			toScroll -= scrollButtonSpeed * deltaTime;
		}

		scrolled += static_cast<int>(toScroll * inverseMouseWheel * scrollWheelSpeed);
		toScroll = 0;

		int maxPossibleScroll = 0;

		if (runDataVector.size() > 6)
		{
			maxPossibleScroll = static_cast<int>(runDataVector.size()) * (iconHeight + scoreYSpace) + scoreOffSetX - scoreYSpace;
			maxPossibleScroll -= 6 * (iconHeight + scoreYSpace) + scoreOffSetX;
		}

		scrolled = std::clamp(scrolled, 0, maxPossibleScroll);
	}

	void Game::renderMenu(Tmpl8::Surface& screen)
	{
		for (auto& [asteroid, frame] : bgAsteroids)
		{
			asteroid->setFrame(frame);
			asteroid->render(screen);
		}
		
		//gameManager->render(screen);
		
		buttons[0]->render(screen);
		buttons[1]->render(screen);
		buttons[2]->render(screen);

		spriteMap[SpriteName::UI_LOGO]->DrawScaled(175, 10, ScreenWidth - 350, 225, screen);

		
	}

	void Game::renderGame(Tmpl8::Surface& screen)
	{
		gameManager->render(screen);

		int uiSize = 75;

		float scale = static_cast<float>(uiSize) / (3.f * 6.f);

		int letterSize = static_cast<int>(6.f * scale);

		int posX = 0;
		int posY = uiSize / 2 - letterSize / 2;

		//Save players data(if player exists)
		auto player = gameManager->getPlayer();
		if (player)
		{
			playerHealth = player->getHealth();
			isPlayerWeaponUpgraded = player->isUpgradedWeapon();
			isPlayerEngineUpgraded = player->isUpgradedEngine();
		}

		//Render Ship UI

		
		if (!isPlayerWeaponUpgraded)
			spriteMap[SpriteName::SHIP_WEAPON]->DrawScaled(posX, 0, uiSize, uiSize, screen);
		else
			spriteMap[SpriteName::SHIP_WEAPON_UPGRADED]->DrawScaled(posX, 0, uiSize, uiSize, screen);

		if (!isPlayerEngineUpgraded)
		{
			spriteMap[SpriteName::SHIP_ENGINE]->DrawScaled(posX, 0, uiSize, uiSize, screen);
			spriteMap[SpriteName::SHIP_ENGINE_EFFECT]->DrawScaled(posX, 0, uiSize, uiSize, screen);
		}
		else
		{
			spriteMap[SpriteName::SHIP_ENGINE_UPGRADED]->DrawScaled(posX, 0, uiSize, uiSize, screen);
			spriteMap[SpriteName::SHIP_ENGINE_UPGRADED_EFFECT]->DrawScaled(posX, 0, uiSize, uiSize, screen);
		}

		spriteMap[SpriteName::SHIP]->DrawScaled(posX, 0, uiSize, uiSize, screen);

		posX += uiSize;

		std::stringstream hpText;
		hpText << std::to_string(playerHealth) << "x";

		screen.PrintScaled(&hpText.str()[0], posX, posY, static_cast<int>(scale), static_cast<int>(scale), 0xFFFFFF);

		posX += letterSize * 2;

		//Render sheep UI
		spriteMap[SpriteName::SHEEP]->DrawScaled(posX, 0, uiSize, uiSize, screen);

		std::stringstream sheepText;
		int sheepTextLength = static_cast<int>(std::to_string(gameManager->getNumberOfSheep()).length()) + 1; // + 1 because we also and 'x' sign
		sheepText << std::to_string(gameManager->getNumberOfSheep()) << "x";

		posX += uiSize;

		screen.PrintScaled(&sheepText.str()[0], posX, posY, static_cast<int>(scale), static_cast<int>(scale), 0xFFFFFF);

		posX += letterSize * sheepTextLength;

		//Render clock
		spriteMap[SpriteName::UI_CLOCK]->DrawScaled(posX, 0, uiSize, uiSize, screen);

		std::stringstream timerText;
		float roundedTimer = std::round(gameTimer * 100.f) / 100.f;
		timerText << roundedTimer;

		posX += uiSize;

		screen.PrintScaled(&timerText.str()[0], posX, posY, static_cast<int>(scale), static_cast<int>(scale), 0xFFFFFF);
	}

	void Game::renderScore(Tmpl8::Surface& screen)
	{
		for (auto& [asteroid, frame] : bgAsteroids)
		{
			asteroid->setFrame(frame);
			asteroid->render(screen);
		}
		
		buttons[3]->render(screen);
		buttons[4]->render(screen);

		for (int i = 0; i < runDataVector.size(); i++)
		{
			RunData& data = runDataVector[i];

			int letterSizeX = scoreTextScale * 6;
			int letterSizeY = scoreTextScale * 5;

			int xPos = scoreOffSetX;
			int yPos = iconHeight * i + scoreYSpace * i + scoreOffSetY - scrolled;

			spriteMap[SpriteName::UI_LABEL]->DrawScaled(0, yPos - 10, 625, iconHeight + 20, screen);

			spriteMap[SpriteName::UI_SMILEYS]->SetFrame(data.spriteIndex);
			spriteMap[SpriteName::UI_SMILEYS]->DrawScaled(xPos, yPos, iconWidth, iconHeight, screen);

			xPos += iconWidth + scoreXSpace;

			std::stringstream dayMonthYearText;

			//Add dates to the stream and also include '0' before numbers that are less than 10
			dayMonthYearText
				<< std::setw(2) << std::setfill('0') << data.saveTimeDay << '/'
				<< std::setw(2) << std::setfill('0') << data.saveTimeMonth << '/'
				<< std::setw(4) << std::setfill('0') << data.saveTimeYear;

			screen.PrintScaled(&dayMonthYearText.str()[0], xPos, yPos + scoreYSpace, scoreTextScale, scoreTextScale, 0xFFFFFF);

			std::stringstream hoursMinuteText;


			hoursMinuteText
				<< std::setw(2) << std::setfill('0') << data.saveTimeHours << ':'
				<< std::setw(2) << std::setfill('0') << data.saveTimeMinutes;

			screen.PrintScaled(&hoursMinuteText.str()[0], xPos + 2 * letterSizeX, yPos + letterSizeY + scoreYSpace * 2, scoreTextScale, scoreTextScale, 0xFFFFFF);

			xPos += (2 + 1 + 2 + 1 + 4) * letterSizeX + scoreXSpace;
			spriteMap[SpriteName::SHEEP]->DrawScaled(xPos, yPos, iconWidth, iconHeight, screen);

			xPos += iconWidth + scoreXSpace;

			std::stringstream sheepScoreText;
			sheepScoreText << data.sheepScore;

			screen.PrintScaled(&sheepScoreText.str()[0], xPos, yPos + iconHeight / 2 - letterSizeY / 2, scoreTextScale, scoreTextScale, 0xFFFFFF);

			xPos += static_cast<int>(std::to_string(data.sheepScore).length()) * letterSizeX + scoreXSpace;

			spriteMap[SpriteName::UI_CLOCK]->DrawScaled(xPos, yPos, iconWidth, iconHeight, screen);

			xPos += iconWidth + scoreXSpace;

			std::stringstream runTimeText;
			runTimeText << std::round(data.runTime * 100.f) / 100.f;
			screen.PrintScaled(&runTimeText.str()[0], xPos, yPos + iconHeight / 2 - letterSizeY / 2, scoreTextScale, scoreTextScale, 0xFFFFFF);
		}
	}

}