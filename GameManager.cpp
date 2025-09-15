#include "GameManager.h"

int randomRange(int min, int max)
{
	return min + rand() % (max - min + 1);
}

GameManager::GameManager() : spawnRate(3.f), spawnTimer(0.f)
{
	srand(time(NULL));
}

void GameManager::update(float deltaTime)
{
	spawnTimer += deltaTime;

	if (spawnTimer >= spawnRate)
	{
		spawnTimer = 0.f;

		spawnAsteroid();
	}

	for (const auto& asteroid : asteroids)
	{
		asteroid->update(deltaTime);
	}
}

void GameManager::render(Tmpl8::Surface* screen)
{
	for (const auto& asteroid : asteroids)
	{
		asteroid->render(screen);
	}
}

void GameManager::spawnAsteroid()
{
	int width = randomRange(64, 256);
	int height = width;

	float x = static_cast<float>(rand() % 2 == 0 ? -width : ScreenWidth);
	float y = static_cast<float>(rand() % 2 == 0 ? -height : ScreenHeight);

	Tmpl8::vec2 randomTarget = {static_cast<float>(randomRange(100, ScreenWidth - 100)), static_cast<float>(randomRange(50, ScreenHeight - 50))};
	Tmpl8::vec2 direction = randomTarget - Tmpl8::vec2{x , y};
	direction.normalize();

	Tmpl8::Surface* surface = new Tmpl8::Surface("assets/asteroid.png");
	surfaces.push_back(surface);

	Tmpl8::Sprite* sprite = new Tmpl8::Sprite(surface, 3);
	sprite->SetFrame(randomRange(0,2));
	sprites.push_back(sprite);

	auto asteroid = std::make_shared<Asteroid>
		(
			sprite,
			x,
			y,
			width,
			height,
			Tmpl8::vec2{0.f, 0.f},
			Tmpl8::vec2{ 100.f, 100.f},
			Tmpl8::vec2{ 500.f, 500.f},
			direction
		);

	asteroids.push_back(asteroid);
}
