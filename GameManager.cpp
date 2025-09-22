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

GameManager::GameManager(std::shared_ptr<CollisionManager> colManager, Tmpl8::Sprite& bulletSprite) : colManager(colManager), bulletSprite(bulletSprite), spawnRate(2.5f), spawnTimer(0.f)
{
    EventBus::Get().AddListener<event::createBullet>(this, [&](const event::createBullet& event) 
        {
            auto  bullet = std::make_shared<Bullet>
                (
                    &bulletSprite,
                    event.pos.x,
                    event.pos.y,
                    event.size.x,
                    event.size.y,
                    Tmpl8::vec2{ 0.f, 0.f },
                    Tmpl8::vec2{ 1500.f, 1500.f },
                    Tmpl8::vec2{ 5000.f, 5000.f },
                    event.direction,
                    event.angle
                );

            bullets.push_back(bullet);
        });
}

GameManager::~GameManager()
{
    EventBus::Get().RemovedListener<event::createBullet>(this);
}

void GameManager::update(float deltaTime)
{
    spawnTimer += deltaTime;

    if (spawnTimer >= spawnRate)
    {
        spawnTimer = 0.f;

       spawnAsteroid();
    }


    updateBullets(deltaTime);
    updateAsteroids(deltaTime);
}

void GameManager::updateBullets(float deltaTime)
{
    for (const auto& bullet : bullets)
    {
        bullet->update(deltaTime);
    }

    for (auto it = bullets.begin(); it != bullets.end(); )
    {
        auto& bullet = *it;

        if (bullet->destroy)
        {
            it = bullets.erase(it);

            colManager->destroyObject(bullet.get());
        }
        else
            ++it;
    }
}

void GameManager::updateAsteroids(float deltaTime)
{
    for (const auto& asteroid : asteroids)
    {
        asteroid->update(deltaTime);
    }

    for (auto it = asteroids.begin(); it != asteroids.end();)
    {
        auto& asteroid = *it;
        if (asteroid->getPosition().x < -asteroid->getSize().x ||
            asteroid->getPosition().x + asteroid->getSize().x >= ScreenWidth + asteroid->getSize().x ||
            asteroid->getPosition().y < -asteroid->getSize().y ||
            asteroid->getSize().y + asteroid->getSize().y >= ScreenHeight + asteroid->getSize().y)
        {
            it = asteroids.erase(it);

            colManager->destroyObject(asteroid.get());
        }
        else
            ++it;
    }
}

void GameManager::render(Tmpl8::Surface& screen)
{
    for (const auto& bullet : bullets)
    {
        bullet->render(screen);
    }
    
    for (const auto& asteroid : asteroids)
    {
        asteroid->render(screen);
    }
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

    Tmpl8::Surface* surface = new Tmpl8::Surface("assets/asteroid.png");
    surfaces.push_back(surface);

    Tmpl8::Sprite* sprite = new Tmpl8::Sprite(surface, 3);
    sprite->SetFrame(randomRange(0, 2));
    sprites.push_back(sprite);

    auto asteroid = std::make_shared<Asteroid>
        (
            sprite,
            x,
            y,
            width,
            height,
            Tmpl8::vec2{ 0.f, 0.f },
            Tmpl8::vec2{ 100.f, 100.f },
            Tmpl8::vec2{ 500.f, 500.f },
            direction
        );

    asteroids.push_back(asteroid);
}
