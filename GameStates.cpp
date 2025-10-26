#include "game.hpp"
#include <random>
#include <fstream>

namespace
{
	std::random_device rd;
	std::minstd_rand rng(rd());
}

static int randomRange(int min, int max)
{
	std::uniform_int_distribution dist(min, max);
	return dist(rng);
}

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
			
			break;
		case GameState::GAME:
			// -- Init game -- //

			EventBus::Get().subscribe<EventType::GAMEOVER>(this, [this] {restart = true; soundMap["gameOver"].replay(); });

			initCollisionManager();
			initGameManager();
			initAnimators();

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
			

			break;
		case GameState::GAME:
		{//Add braces because case: does not have it own scope 
			// -- Save data on game finish -- //

			runData.spriteIndex = randomRange(0, 19);

			std::time_t t = std::time(0);
			std::tm* now = std::localtime(&t);

			runData.startTimeDay = now->tm_mday;
			runData.startTimeMonth = now->tm_mon + 1;
			runData.startTimeYear = now->tm_year + 1900;

			runData.startTimeHours = now->tm_hour;
			runData.startTimeMinutes = now->tm_min;

			runData.sheepScore = gameManager->getNumberOfSheep();

			runData.runTime = gameTimer;

			runDataVector.push_back(runData);

			// -- Destroy all game objects -- //
			EventBus::Get().unsubscribe<EventType::GAMEOVER>(this);

			gameManager = nullptr;
			collisionManager = nullptr;
			animator = nullptr;
			break;
		}
		case GameState::SCORE:
			break;
		}
	}

	void Game::updateMenu(float deltaTime)
	{
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
		buttons[3]->CheckClick(mousePosition, wasMouseDown, wasMouseUp);
		buttons[4]->CheckClick(mousePosition, wasMouseDown, wasMouseUp);

		if (isKeyHold('w'))
		{
			scrolled -= static_cast<int>(1000.f * deltaTime);
		}
		else if (isKeyHold('s'))
		{
			scrolled += static_cast<int>(1000.f * deltaTime);
		}

		int maxPossibleScroll = runDataVector.size() * (iconHeight + scoreYSpace) + scoreOffSetX;
		scrolled = std::clamp(scrolled, 0, maxPossibleScroll);
	}

	void Game::renderMenu(Tmpl8::Surface& screen)
	{
		buttons[0]->render(screen);
		buttons[1]->render(screen);
		buttons[2]->render(screen);

		spriteMap["logo"]->DrawScaled(175, 10, ScreenWidth - 350, 225, screen);
	}

	void Game::renderGame(Tmpl8::Surface& screen)
	{
		gameManager->render(screen);

		int uiSize = 75;

		float scale = static_cast<float>(uiSize) / (3.f * 6.f);

		int letterSize = static_cast<int>(6.f * scale);
		int posY = uiSize / 2 - letterSize / 2;

		//Render Ship UI

		spriteMap["engineEffect"]->DrawScaled(0, 0, uiSize, uiSize, screen);
		if (!gameManager->getPlayer()->isUpgraded())
			spriteMap["weapon"]->DrawScaled(0, 0, uiSize, uiSize, screen);
		else
			spriteMap["weapon1"]->DrawScaled(0, 0, uiSize, uiSize, screen);
		spriteMap["shipEngine"]->DrawScaled(0, 0, uiSize, uiSize, screen);
		spriteMap["ship"]->DrawScaled(0, 0, uiSize, uiSize, screen);

		std::stringstream hpText;
		hpText << std::to_string(gameManager->getPlayer()->getHealth()) << "x";

		screen.PrintScaled(&hpText.str()[0], uiSize, posY, static_cast<int>(scale), static_cast<int>(scale), 0xFFFFFF);

		//Render sheep UI
		spriteMap["sheep"]->DrawScaled(uiSize + letterSize * 2, 0, uiSize, uiSize, screen);

		std::stringstream sheepText;
		sheepText << std::to_string(gameManager->getNumberOfSheep()) << "x";

		screen.PrintScaled(&sheepText.str()[0], uiSize * 2 + letterSize * 2, posY, static_cast<int>(scale), static_cast<int>(scale), 0xFFFFFF);

		//Render clock
		spriteMap["clock"]->DrawScaled(uiSize * 2 + letterSize * 4, 0, uiSize, uiSize, screen);

		std::stringstream timerText;
		float roundedTimer = std::round(gameTimer * 100.f) / 100.f;
		timerText << roundedTimer;
		screen.PrintScaled(&timerText.str()[0], uiSize * 3 + letterSize * 4, posY, static_cast<int>(scale), static_cast<int>(scale), 0xFFFFFF);
	}

	void Game::renderScore(Tmpl8::Surface& screen)
	{
		buttons[3]->render(screen);
		buttons[4]->render(screen);

		for (int i = 0; i < runDataVector.size(); i++)
		{
			RunData& data = runDataVector[i];

			int letterSizeX = scoreTextScale * 6;
			int letterSizeY = scoreTextScale * 5;

			int xPos = scoreOffSetX;
			int yPos = iconHeight * i + scoreYSpace * i + scoreOffSetY - scrolled;

			spriteMap["label"]->DrawScaled(0, yPos - 10, 575, iconHeight + 20, screen);

			spriteMap["smileys"]->SetFrame(data.spriteIndex);
			spriteMap["smileys"]->DrawScaled(xPos, yPos, iconWidth, iconHeight, screen);

			xPos += iconWidth + scoreXSpace;

			std::stringstream dayMonthYearText;

			//Add dates to the stream and also include '0' before numbers that are less than 10
			

			dayMonthYearText << data.startTimeDay << '/' << data.startTimeMonth << '/' << data.startTimeYear;
			screen.PrintScaled(&dayMonthYearText.str()[0], xPos, yPos + scoreYSpace, scoreTextScale, scoreTextScale, 0xFFFFFF);

			std::stringstream hoursMinuteText;
			hoursMinuteText << data.startTimeHours << ':' << data.startTimeMinutes;
			screen.PrintScaled(&hoursMinuteText.str()[0], xPos + 2 * letterSizeX, yPos + letterSizeY + scoreYSpace * 2, scoreTextScale, scoreTextScale, 0xFFFFFF);

			xPos += (2 + 1 + 2 + 1 + 4) * letterSizeX + scoreXSpace;
			spriteMap["sheep"]->DrawScaled(xPos, yPos, iconWidth, iconHeight, screen);

			xPos += iconWidth + scoreXSpace;

			std::stringstream sheepScoreText;
			sheepScoreText << data.sheepScore;

			screen.PrintScaled(&sheepScoreText.str()[0], xPos, yPos + iconHeight / 2 - letterSizeY / 2, scoreTextScale, scoreTextScale, 0xFFFFFF);

			xPos += std::to_string(data.sheepScore).length() * letterSizeX + scoreXSpace;

			spriteMap["clock"]->DrawScaled(xPos, yPos, iconWidth, iconHeight, screen);

			xPos += iconWidth + scoreXSpace;

			std::stringstream runTimeText;
			runTimeText << std::round(data.runTime * 100.f) / 100.f;
			screen.PrintScaled(&runTimeText.str()[0], xPos, yPos + iconHeight / 2 - letterSizeY / 2, scoreTextScale, scoreTextScale, 0xFFFFFF);
		}
	}

}