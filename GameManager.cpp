#include "GameManager.hpp"
#include "Upgrade.hpp"
#include "EventBus.hpp"
#include "game.hpp"
#include <sstream>

#include <random>

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

static float randomRange(float min, float max)
{
    return static_cast<float>(randomRange(static_cast<int>(min), static_cast<int>(max)));
}

static Tmpl8::vec2 generateRandomVec2(const Tmpl8::vec2& min = { 0.f, 0.f }, const Tmpl8::vec2& max = {0.f, 0.f})
{
    float x = randomRange(min.x, max.x);
    float y = randomRange(min.y, max.y);

    return Tmpl8::vec2{ x,y };
}

GameManager::GameManager(std::shared_ptr<CollisionManager> collisionManager, const std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>>& spriteMap, const std::unordered_map<std::string, Audio::Sound>& soundMap) :
    collisionManager(collisionManager), spriteMap(spriteMap), soundMap(soundMap)
{
    player = std::make_shared<Player>
        (
            spriteMap,
            soundMap,
            Tmpl8::vec2{ static_cast<float>(ScreenWidth) / 2.f - 32.f / 2.f, static_cast<float>(ScreenHeight) / 2.f - 32.f / 2.f },
            Tmpl8::vec2{ 96.f, 96.f }
        );

    player->setMaxSpeed(150.f);
    player->setAcceleration({ 550.f, 550.f });
    player->setDirection({ 0.f, -1.f });
    player->setTag("player");

    //Init asteroid frame sprites
    for (int i = 0; i < 3; i++)
    {
        auto sprite = std::make_shared<Tmpl8::Sprite>(new Tmpl8::Surface("assets/asteroid.png"), 3);
        sprite->SetFrame(i);
        asteroidSprites.push_back(sprite);
    }

    initTimerManager();

#ifdef _DEBUG
    EventBus::Get().subscribe<EventType::KILL_ALL>(this, [this] {objects.clear(); });
#endif

}

GameManager::~GameManager()
{
#ifdef _DEBUG
    EventBus::Get().unsubscribe<EventType::KILL_ALL>(this);
#endif
}

void GameManager::update(float deltaTime)
{
    timerManager->update(deltaTime);


    // -- Asteroid Spawner -- //
    spawnTimer += deltaTime;

    if (spawnTimer >= spawnRate)
    {
        spawnTimer = 0.f;

        spawnAsteroid();
    }

    // -- Upgrade Spawner -- //
    upgradeSpawnTimer -= deltaTime;

    if (isUpgradeUsed && !player->isUpgraded())
    {
        isUpgradeUsed = false;
        upgradeSpawnTimer = upgradeSpawnTime;
    }

    if (upgradeSpawnTimer <= 0.f && !isUpgradeOnMap && !isUpgradeUsed)
    {
        spawnUpgrade();
        upgradeSpawnTimer = upgradeSpawnTime;
        isUpgradeOnMap = true;

    }

    //Spawn hit markers when bullet hits an asteroid
    auto& collisions = collisionManager->getCollisions();
    for (auto& [col1, col2] : collisions)
    {
        std::shared_ptr<Object> bullet;
        std::shared_ptr<Object> asteroid;
        
        if (col1->getTag() == "bullet" && col2->getTag() == "asteroid")
        {
            bullet = col1;
            asteroid = col2;
        }
        else if (col2->getTag() == "bullet" && col1->getTag() == "asteroid")
        {
            bullet = col2;
            asteroid = col1;
        }

        if (bullet && asteroid)
        {
            auto par = std::make_shared<Object>(spriteMap["hitEffect"], bullet->getPosition(), Tmpl8::vec2{ 30.f, 30.f });

            particles.push_back({ par, particleSpawnTime });
        }
    }


    // -- Update all gameObjects -- //
    updatePlayer(deltaTime);
    updateObjects(deltaTime);
}

void GameManager::updatePlayer(float deltaTime)
{
    player->update(deltaTime);
}

void GameManager::updateObjects(float deltaTime)
{
    // -- Delete Object -- //
    for (auto it = objects.begin(); it != objects.end();)
    {
        auto& obj = *it;

        if (obj->destroy)
        {
            //If deleted objects is asteroid
            //Spawn a sheep
            if (obj->getTag() == "asteroid")
            {
                auto asteroid = std::static_pointer_cast<Asteroid>(obj);
                
                if(asteroid->turnToSheep)
                    spawnSheep(obj->getPosition(), obj->getSize(), obj->getDirection(), obj->getAngle());
                else if (asteroid->divide)
                {
                    // -- Divide an asteroid in the small ones -- //
                    const auto& position = asteroid->getPosition();
                    const auto& size = asteroid->getSize();
                    const auto& direction = asteroid->getDirection();

                    int newAsteroidsNumber = 0;

                    //Size of an asteroid is in range of 64 to 256

                    if (size.x < 90.f) newAsteroidsNumber = 0;
                    else if (size.x < 150.f) newAsteroidsNumber = 1;
                    else if (size.x < 210.f) newAsteroidsNumber = 2;
                    else newAsteroidsNumber = 3;

                    for (int i = 0; i < newAsteroidsNumber; i++)
                    {
                        auto& newSprite = asteroidSprites[randomRange(0, 2)];
                        
                        Tmpl8::vec2 newPosition = generateRandomVec2(position, position + size);
                        Tmpl8::vec2 newSize = { randomRange(32.f, size.x / 1.5f) };

                        Tmpl8::vec2 randomTarget = { generateRandomVec2({0.f, 0.f}, {ScreenWidth, ScreenHeight}) };

                        Tmpl8::vec2 newDirection = randomTarget - newPosition;
                        newDirection.normalize();

                        auto ast = std::make_shared<Asteroid>(newSprite, newPosition, newSize, 4 - newAsteroidsNumber);

                        ast->setMaxSpeed(asteroidMaxSpeed * 0.9f);
                        ast->setAcceleration({ asteroidAcceleration * 0.9f * newDirection.x, asteroidAcceleration * 0.9f * newDirection.y });
                        ast->setDirection(newDirection);
                        ast->setTag("asteroid");

                        tempObjects.push_back(ast);
                    }
                  
                }
            }
           
            else if (obj->getTag() == "upgrade")
            {
                isUpgradeOnMap = false;
                isUpgradeUsed = true;
            }

            it = objects.erase(it);
        }
        else
            ++it;
    }

    //Move objects from temprorary vector
    objects.reserve(tempObjects.size());
    objects.insert(objects.end(), tempObjects.begin(), tempObjects.end());
    tempObjects.clear();

    //Update
    for (const auto& obj : objects)
    {
        obj->update(deltaTime);
    }

    //Update particles' timers
    for (auto& [par, timer] : particles)
    {
        timer -= deltaTime;
    }

    for (auto it = particles.begin(); it != particles.end();)
    {
        auto& pair = *it;

        pair.second -= deltaTime;

        if (pair.second <= 0.f)
        {
            it = particles.erase(it);
        }
        else
            ++it;
    }

    //Delete objects when they get out of screen
    for (const auto& obj : objects)
    {
        auto pos = obj->getPosition();
        auto size = obj->getSize();

        if (pos.x + size.x < 0.f ||
            pos.x >= ScreenWidth ||
            pos.y + size.y < 0.f ||
            pos.y >= ScreenHeight)
        {
            obj->destroy = true;
        }
    }

    //Restart game when player dies 
    if (player->destroy)
    {
        timerManager->addTimer(2.f, [this]() {EventBus::Get().push<EventType::GAMEOVER>(); });
    }
}

void GameManager::render(Tmpl8::Surface& screen)
{
    for (const auto& obj : objects)
    {
        obj->render(screen);
    }

    for (const auto& [par, timer] : particles)
    {
        par->render(screen);
    }

    player->render(screen);

}

int GameManager::getNumberOfSheep() const
{
    return sheepCounter;
}

std::shared_ptr<Player> GameManager::getPlayer() const
{
    return player;
}

std::vector<std::shared_ptr<Object>> GameManager::getObjects() const
{
   auto& bullets = player->getBullets();

    size_t vectorSize = 1 + objects.size() + bullets.size();
    std::vector<std::shared_ptr<Object>> vec;
    vec.reserve(vectorSize);

    vec.push_back(player);
    vec.insert(vec.end(), objects.begin(), objects.end());
    vec.insert(vec.end(), bullets.begin(), bullets.end());

    return vec;
}

void GameManager::initTimerManager()
{
    timerManager = std::make_unique<TimerManager>();
}

void GameManager::spawnAsteroid()
{
    int width = randomRange(64, 256);
    int height = width;

    std::bernoulli_distribution dist(0.5); // 50% true / 50% false.

    float x = static_cast<float>(dist(rng) ? -width : ScreenWidth);
    float y = static_cast<float>(dist(rng) ? -height : ScreenHeight);

   // Tmpl8::vec2 randomTarget = { static_cast<float>(randomRange(100, ScreenWidth - 100)), static_cast<float>(randomRange(50, ScreenHeight - 50)) };
    Tmpl8::vec2 randomTarget = { generateRandomVec2({100.f, 50.f}, {ScreenWidth - 100.f, ScreenHeight - 50.f}) };
    Tmpl8::vec2 direction = randomTarget - Tmpl8::vec2{ x , y };
    direction.normalize();

    float health = 0.f;

    if (width < 128) health = 1.f;
    else if (width < 192) health = 2.f;
    else health = 3.f;

    auto asteroid = std::make_shared<Asteroid>
        (
            asteroidSprites[randomRange(0, 2)],
            Tmpl8::vec2{x ,y},
            Tmpl8::vec2{static_cast<float>(width), static_cast<float>(height)},
            health
        );

    asteroid->setMaxSpeed(asteroidMaxSpeed);
    asteroid->setAcceleration({ asteroidAcceleration * direction.x, asteroidAcceleration * direction.y });
    asteroid->setDirection(direction);
    asteroid->setTag("asteroid");

    objects.push_back(asteroid);
}

void GameManager::spawnUpgrade()
{
    int w = 70, h = 70;

    float x = static_cast<float>(randomRange(0,ScreenWidth - w));
    float y = static_cast<float>(randomRange(100, ScreenHeight - h));

    auto upgrd = std::make_shared<Upgrade>(
        spriteMap["upgrade"],
        Tmpl8::vec2{x, y},
        Tmpl8::vec2{static_cast<float>(w), static_cast<float>(h)}
    );

    upgrd->setAngle(static_cast<float>(randomRange(0, 360)));
    upgrd->setTag("upgrade");

    objects.push_back(upgrd);

}

void GameManager::spawnSheep(Tmpl8::vec2 pos, Tmpl8::vec2 size, Tmpl8::vec2 direction, float angle)
{
    auto sheep = std::make_shared<Sheep>
        (
            spriteMap["sheep"],
            pos,
            size
        );

    sheep->setMaxSpeed(150.f);
    sheep->setAcceleration({ 600.f * -direction.x, 600.f * -direction.y });
    sheep->setDirection(-direction);
    sheep->setAngle(angle);
    sheep->setTag("sheep");

    tempObjects.push_back(sheep);

    sheepCounter++;
}
