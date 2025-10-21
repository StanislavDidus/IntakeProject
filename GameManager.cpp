#include "GameManager.hpp"
#include "game.hpp"
#include "Upgrade.hpp"
#include "EventBus.hpp"

#include <random>

namespace
{
    std::random_device rd; // Source of entropy.
    std::minstd_rand rng(rd());
}

static int randomRange(int min, int max)
{
    std::uniform_int_distribution dist(min, max);
    return dist(rng);
}

GameManager::GameManager(std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>>& sprites, const std::unordered_map<std::string, Audio::Sound>&  sounds, Tmpl8::Game* game) :
    sprites(sprites), game(game), spawnRate(2.5f), spawnTimer(0.f), upgradeSpawnTime(3.f), upgradeSpawnTimer(upgradeSpawnTime), isUpgradeOnMap(false), isUpgradeUsed(false), sounds(sounds)
{
    player = std::make_shared<Player>
        (
            sprites,
            sounds,
            Tmpl8::vec2{ static_cast<float>(ScreenWidth) / 2.f - 32.f / 2.f,
            static_cast<float>(ScreenHeight) / 2.f - 32.f / 2.f },
            Tmpl8::vec2{96.f, 96.f}
        );

    player->setMaxSpeed(150.f);
    player->setAcceleration({550.f, 550.f});
    player->setDirection({0.f, -1.f});
    player->setTag("player");

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
    if (Tmpl8::Game::isKeyDown('k'))
    {
        for (auto& obj : objects)
        {
            obj->destroy = true;
       }
    }
    
    spawnTimer += deltaTime;

    if (spawnTimer >= spawnRate)
    {
        spawnTimer = 0.f;

        spawnAsteroid();
    }

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
            //If deleted objects is asteroid
            //Spawn a sheep
            if (obj->getTag() == "asteroid")
            {
                auto asteroid = std::static_pointer_cast<Asteroid>(obj);
                
                if(asteroid->turnToSheep)
                    spawnSheep(obj->getPosition(), obj->getSize(), obj->getDirection(), obj->getAngle());
                else if (asteroid->divide)
                {
                    //spawnAsteroid();
                }
            }
           
            else if (obj->getTag() == "upgrade")
            {
                isUpgradeOnMap = false;
                isUpgradeUsed = true;
            }

            /*else if (obj->getTag() == "sheep")
            {
                sheepCounter--;
            }*/
            
            it = objects.erase(it);
        }
        else
            ++it;
    }

    //Move objects from temprorary vector
    objects.reserve(tempObjects.size());
    objects.insert(objects.end(), tempObjects.begin(), tempObjects.end());
    tempObjects.clear();

    //Update all objects
    for (const auto& obj : objects)
    {
        obj->update(deltaTime);
    }

    //Delete objects when they get out of screen
    for (const auto& obj : objects)
    {
        auto pos = obj->getPosition();
        auto size = obj->getSize();

        if (pos.x < -size.x ||
            pos.x + size.x >= ScreenWidth + size.x ||
            pos.y < -size.y ||
            pos.y + size.y >= ScreenHeight + size.y)
        {
            obj->destroy = true;
        }
    }

    if (player->destroy)
    {
        EventBus::Get().push<EventType::GAMEOVER>();
    }
}

void GameManager::render(Tmpl8::Surface& screen)
{
    for (const auto& obj : objects)
    {
        obj->render(screen);
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

    asteroid->setMaxSpeed(100.f);
    asteroid->setAcceleration({ 500.f * direction.x, 500.f * direction.y });
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
        sprites["upgrade"],
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
            sprites["sheep"],
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
