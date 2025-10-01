#include "GameManager.h"

#include <random>

namespace
{
    std::random_device rd; // Source of entropy.
    std::minstd_rand rng(rd());
}

int randomRange(int min, int max)
{
    std::uniform_int_distribution dist(min, max);
    return dist(rng);
}

GameManager::GameManager(std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>>& sprites) :
    sprites(sprites), spawnRate(2.5f), spawnTimer(0.f)
{
    player = std::make_shared<Player>
        (
            sprites["ship"].get(),
            sprites["shipEngine"].get(),
            sprites["engineEffect"].get(),
            sprites["weapon"].get(),
            sprites["bullet"].get(),
            static_cast<float>(ScreenWidth) / 2.f - 32.f / 2.f,
            static_cast<float>(ScreenHeight) / 2.f - 32.f / 2.f,
            96,
            96,
            Tmpl8::vec2{ 0.f, 0.f },
            Tmpl8::vec2{ 150.f, 150.f },
            Tmpl8::vec2{ 550.f, 550.f },
            Tmpl8::vec2{ 0.f, -1.f }
        );

    //Init asteroid frame sprites
    for (int i = 0; i < 3; i++)
    {
        auto sprite = std::make_shared<Tmpl8::Sprite>(new Tmpl8::Surface("assets/asteroid.png"), 3);
        sprite->SetFrame(i);
        asteroidSprites.push_back(sprite);
    }
}

GameManager::~GameManager()
{
}

void GameManager::update(float deltaTime)
{
    spawnTimer += deltaTime;

    if (spawnTimer >= spawnRate)
    {
        spawnTimer = 0.f;

       spawnAsteroid();
    }


    updatePlayer(deltaTime);
    updateObjects(deltaTime);
}

void GameManager::updatePlayer(float deltaTime)
{
    player->update(deltaTime);
}

void GameManager::updateObjects(float deltaTime)
{
    for (auto it = objects.begin(); it != objects.end();)
    {
        auto& obj = *it;

        if (obj->destroy)
        {
            it = objects.erase(it);
        }
        else
            ++it;
    }

    for (const auto& obj : objects)
    {
        obj->update(deltaTime);
    }

    for (const auto& obj : objects)
    {
        auto& pos = obj->getPosition();
        auto& size = obj->getSize();

        if (pos.x < -size.x ||
            pos.x + size.x >= ScreenWidth + size.x ||
            pos.y < -size.y ||
            pos.y + size.y >= ScreenHeight + size.y)
        {
            obj->destroy = true;
        }
    }
}

void GameManager::render(Tmpl8::Surface& screen)
{
    player->render(screen);

    for (const auto& obj : objects)
    {
        obj->render(screen);
    }

}

std::shared_ptr<Player> GameManager::getPlayer() const
{
    return player;
}

std::vector<std::shared_ptr<Object>> GameManager::getObjects() const
{
    std::vector<std::shared_ptr<Bullet>> bullets = player->getBullets();

    size_t vectorSize = 1 + objects.size() + bullets.size();
    std::vector<std::shared_ptr<Object>> vec;
    vec.reserve(vectorSize);

    vec.push_back(player);
    vec.insert(vec.end(), objects.begin(), objects.end());
    vec.insert(vec.end(), bullets.begin(), bullets.end());

    return vec;
}

void GameManager::spawnAsteroid()
{
    int width = randomRange(64, 256);
    int height = width;

    std::bernoulli_distribution dist(0.5); // 50% true / 50% false.

    float x = static_cast<float>(dist(rng) ? -width : ScreenWidth);
    float y = static_cast<float>(dist(rng) ? -height : ScreenHeight);

    Tmpl8::vec2 randomTarget = { static_cast<float>(randomRange(100, ScreenWidth - 100)), static_cast<float>(randomRange(50, ScreenHeight - 50)) };
    Tmpl8::vec2 direction = randomTarget - Tmpl8::vec2{ x , y };
    direction.normalize();

    int health = 0;

    if (width < 128) health = 1;
    else if (width < 192) health = 2;
    else health = 3;

    auto asteroid = std::make_shared<Asteroid>
        (
            asteroidSprites[randomRange(0, 2)].get(),
            x,
            y,
            width,
            height,
            Tmpl8::vec2{ 0.f, 0.f },
            Tmpl8::vec2{ 100.f, 100.f },
            Tmpl8::vec2{ 500.f, 500.f },
            direction,
            health
        );

    objects.push_back(asteroid);
}
