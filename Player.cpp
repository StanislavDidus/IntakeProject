#include "Player.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>
#include "CollisionHelper.hpp"

#include <input/Input.hpp>

using namespace input;

Player::Player
(
    std::shared_ptr<AssetManager> assetManager,
    Tmpl8::vec2 position,
    Tmpl8::vec2 size
) : Object(assetManager->getSprite(SpriteName::SHIP), position, size), assetManager(assetManager)
{
    setState(PlayerState::IDLE);

    initInputs();
    initAnimator();
    initTimerManager();
    initEvents();

    //assetManager->getSprite(SpriteName::SHIP_WEAPON)->SetFrame(1);

    //center = { 0.f, 0.f };
}

Player::~Player()
{
}

void Player::initInputs()
{
    //Forward
    Input::addButtonCallback("Forward", [](std::span<const GamepadStateTracker> gamepads, const KeyboardStateTracker& keyboard, const MouseStateTracker&)
        {
            bool rt = false;
            bool leftStickUp = false;
            for (auto& gamepad : gamepads)
            {
                rt = rt || (gamepad.getLastState().connected && gamepad.rightTrigger == ButtonState::Held);
                leftStickUp = leftStickUp || gamepad.getLastState().thumbSticks.leftY < -0.5f;
            }

            bool w = keyboard.getLastState().W;
            bool up = keyboard.getLastState().Up;

            //std::cout << rt << ", " << w << ", " << up << std::endl;

            return rt || leftStickUp || w || up;
        });

    //Left Rotation
    Input::addAxisCallback("Rotate", [](std::span<const GamepadStateTracker> gamepads, const KeyboardStateTracker& keyboard, const MouseStateTracker&)
        {
            float dPad = 0.0f;
            float leftStick = 0.0f;
            for (auto& gamepad : gamepads)
            {
                dPad += gamepad.dPadLeft == ButtonState::Held ? -1.0f : 0.0f;
                dPad += gamepad.dPadRight == ButtonState::Held ? 1.0f : 0.0f;
                leftStick += gamepad.getLastState().thumbSticks.leftX;
            }

            auto state = keyboard.getLastState();

            float a = state.A ? -1.0f : 0.0f;
            float d = state.D ? 1.0f : 0.0f;
            float left = state.Left ? -1.0f : 0.0f;
            float right = state.Right ? 1.0f : 0.0f;

            return std::clamp(dPad + leftStick + a + d + left + right, -1.0f, 1.0f);
        });

    //Shoot
    Input::addButtonCallback("Shoot", [](std::span<const GamepadStateTracker> gamepads, const KeyboardStateTracker& keyboard, const MouseStateTracker&)
        {
            bool a = false; // A-button on gamepad.
            bool x = false; // X-button on gamepad.
            for (auto& gamepad : gamepads)
            {
                x = x || gamepad.x == ButtonState::Held;
                a = a || gamepad.a == ButtonState::Held;
            }

            bool e = keyboard.getLastState().E;
            bool s = keyboard.getLastState().Space;
            bool ad = keyboard.getLastState().Down; // Arrow-Down

            return a || x || e || s || ad;
        });
}

void Player::initAnimator()
{
    animator = std::make_unique<Animator>();

    //EngineEffect
    animator->addFrameAnimation(assetManager->getSprite(SpriteName::SHIP_ENGINE_EFFECT), 0.1f, 0, 2, [this]() {return abs(velocity.length()) <= 75.f; });

    animator->addFrameAnimation(assetManager->getSprite(SpriteName::SHIP_ENGINE_EFFECT), 0.1f, 3, 6, [this]() {return abs(velocity.length()) > 75.f; });

    //EngineEffect1
    animator->addFrameAnimation(assetManager->getSprite(SpriteName::SHIP_ENGINE_UPGRADED_EFFECT), 0.1f, 0, 3, [this]() {return abs(velocity.length()) <= 75.f; });

    animator->addFrameAnimation(assetManager->getSprite(SpriteName::SHIP_ENGINE_UPGRADED_EFFECT), 0.1f, 4, 7, [this]() {return abs(velocity.length()) > 75.f; });

    //Common weapon shooting
    animator->addFrameCycledAnimation(assetManager->getSprite(SpriteName::SHIP_WEAPON), shootTime / 2.f, 1, 2, "LeftShoot");
    animator->addFrameCycledAnimation(assetManager->getSprite(SpriteName::SHIP_WEAPON), shootTime / 2.f, 3, 4, "RightShoot");

    //Upgraded weapon charge and shooting
    animator->addFrameCycledAnimation(assetManager->getSprite(SpriteName::SHIP_WEAPON_UPGRADED), chargeTime / 6.f, 1, 6, "Charge", false);
    animator->addFrameCycledAnimation(assetManager->getSprite(SpriteName::SHIP_WEAPON_UPGRADED), shootTime / 2.f, 7, 11, "ChargeShoot", true);
}

void Player::initTimerManager()
{
    timerManager = std::make_unique<TimerManager>();

    timerManager->addTimer(8.f, [this]() {drawInputHelp = false; });
}

void Player::initEvents()
{

}

void Player::update(float deltaTime)
{
    timerManager->update(deltaTime);

    shootTimer -= deltaTime;

    if (isHit)
    {
        blinkTimer -= deltaTime;
        if (blinkTimer <= 0.f)
        {
            blink = !blink;
            blinkTimer = blinkTime;
        }
    }

    updateBullets(deltaTime);

    animator->update(deltaTime);

    switch (state)
    {
    case PlayerState::NONE:
        break;
    case PlayerState::IDLE:
        updateIdle(deltaTime);
        break;
    case PlayerState::SHOOT:
        updateShoot(deltaTime);
        break;
    case PlayerState::SUPERSHOOT:
        updateSuperShoot(deltaTime);
        break;

    }

    applyVelocity(deltaTime);

    //position.x = fmodf(static_cast<float>(position.x) + ScreenWidth, ScreenWidth);
    //position.y = fmodf(static_cast<float>(position.y) + ScreenHeight, ScreenHeight);

    if (position.x >= ScreenWidth) position.x = 0.f;
    if (position.y >= ScreenHeight) position.y = 0.f;

    if (position.x + size.x < 0.f) position.x = ScreenWidth - size.x;
    if (position.y + size.y < 0.f) position.y = ScreenHeight - size.y;
}

void Player::render(Tmpl8::Surface& screen)
{
    for (const auto& bullet : bullets)
    {
        bullet->render(screen);
    }

    // Draw weapon
    if (!upgradedWeapon)
    {
        renderShipPart(assetManager->getSprite(SpriteName::SHIP_WEAPON), screen);
    }
    else
    {
        renderShipPart(assetManager->getSprite(SpriteName::SHIP_WEAPON_UPGRADED), screen);
    }

    if (!upgradedEngine)
    {
        // Draw engine
        renderShipPart(assetManager->getSprite(SpriteName::SHIP_ENGINE), screen);

        // Draw engine effect
        renderShipPart(assetManager->getSprite(SpriteName::SHIP_ENGINE_EFFECT), screen);
    }
    else
    {
        // Draw engine
        renderShipPart(assetManager->getSprite(SpriteName::SHIP_ENGINE_UPGRADED), screen);

        // Draw engine effect
        renderShipPart(assetManager->getSprite(SpriteName::SHIP_ENGINE_UPGRADED_EFFECT), screen);
    }

    // Draw main ship
    renderShipPart(sprite, screen);

    renderInputHelp(screen);
}


void Player::renderShipPart(std::shared_ptr<Tmpl8::Sprite> sprite, Tmpl8::Surface& screen)
{
    if (blink) return;

    animator->setAnimationFrame(sprite);

    int x = static_cast<int>(position.x);
    int y = static_cast<int>(position.y);
    int width = static_cast<int>(size.x);
    int height = static_cast<int>(size.y);

    //center = { size.x / 2.f, size.y };

    auto v1 = getVertices(sprite, position);
    auto v2 = getVertices(sprite, Tmpl8::vec2{ static_cast<float>(fmod(x + width + ScreenWidth, ScreenWidth) - width), static_cast<float>(fmod(y + height + ScreenHeight, ScreenHeight) - static_cast<double>(height)) });
    auto v3 = getVertices(sprite, Tmpl8::vec2{ static_cast<float>(fmod(x + width + ScreenWidth, ScreenWidth) - width), static_cast<float>(fmod(y + ScreenHeight, ScreenHeight)) });
    auto v4 = getVertices(sprite, Tmpl8::vec2{ static_cast<float>(fmod(x + ScreenWidth, ScreenWidth)),                 static_cast<float>(fmod(y + height + ScreenHeight, ScreenHeight) - static_cast<double>(height)) });

    sprite->DrawScaledRotated(v1[0], v1[1], v1[2], v1[3], screen);
    sprite->DrawScaledRotated(v2[0], v2[1], v2[2], v2[3], screen);
    sprite->DrawScaledRotated(v3[0], v3[1], v3[2], v3[3], screen);
    sprite->DrawScaledRotated(v4[0], v4[1], v4[2], v4[3], screen);
}

void Player::renderInputHelp(Tmpl8::Surface& screen)
{
    if (!drawInputHelp) return;

    int scale = 2;
    int letterSizeY = scale * 5;
    int space = 5;

    int x = static_cast<int>(position.x) + 50;
    int y = static_cast<int>(position.y);

    screen.PrintScaled("(W) or Left-Trigger to MOVE", x, y, scale, scale, 0xFFFFFF);

    y += letterSizeY + space;

    screen.PrintScaled("(A)(D) or DPad-L-R to ROTATE", x, y, scale, scale, 0xFFFFFF);

    y += letterSizeY + space;

    screen.PrintScaled("(E) or (SPACE) or (X) to Shoot", x, y, scale, scale, 0xFFFFFF);
}

const std::vector<std::shared_ptr<IBullet>>& Player::getBullets() const
{
    return bullets;
}

int Player::getHealth() const
{
    return currentHealth;
}

void Player::upgradeEngine()
{
    upgradedEngine = true;

    maxSpeed = 175.f;
    acceleration = Tmpl8::vec2{ 600.f, 600.f };
    rotationSpeed = 220.f;

    assetManager->getSound(SoundName::SHIP_UPGRADE_ENGINE).replay();
}

void Player::move(float deltaTime)
{
    velocity += acceleration * direction * deltaTime;

    float speed = velocity.length();
    if (speed > maxSpeed)
    {
        velocity.x = (velocity.x / speed) * maxSpeed;
        velocity.y = (velocity.y / speed) * maxSpeed;
    }
}

void Player::rotate(float angle, float deltaTime)
{
    this->angle += angle * rotationSpeed * deltaTime;
    this->angle = fmodf(this->angle, 360.f);

    float radians = this->angle * Tmpl8::PI / 180.f;
    float sin = std::sin(radians);
    float cos = std::cos(radians);

    direction.x = sin;
    direction.y = -cos;

    direction.normalize();

    //std::cout << direction.x << ", " << direction.y << "\n";
}

void Player::onCollisionEnter(std::shared_ptr<Object> object)
{
    //Enter
    if (object->getTag() == "sheep")
    {
        maxSpeed += 50.f;
    }

    if (object->getTag() == "upgrade")
    {
        assetManager->getSound(SoundName::SHIP_UPGRADE_WEAPON).replay();

        object->destroy = true;
        upgradedWeapon = true;
    }
}

void Player::onCollisionStay(std::shared_ptr<Object> object, float deltaTime)
{
    //Stay
    if (object->getTag() == "asteroid" && !isHit)
    {
        currentHealth--;
        isHit = true;

        assetManager->getSound(SoundName::SHIP_DAMAGED).replay();

        //Disable invulnerability after some time
        timerManager->addTimer(invulnerableTime, [this]
            {
                isHit = false;
                blink = false;
            }
        );

        sprite->SetFrame(4 - currentHealth);

        if (currentHealth <= 0)
        {
            //setState(PlayerState::DAMAGE);

            destroy = true;
        }
    }
}

void Player::onCollisionExit(std::shared_ptr<Object> object)
{
    //Exit
    if (object->getTag() == "sheep")
    {
        maxSpeed -= 50.f;
    }
}

bool Player::isUpgradedWeapon() const
{
    return upgradedWeapon;
}

bool Player::isUpgradedEngine() const
{
    return upgradedEngine;
}

void Player::updateBullets(float deltaTime)
{
    for (auto it = bullets.begin(); it != bullets.end();)
    {
        auto& bullet = *it;

        if (bullet->destroy)
        {
            it = bullets.erase(it);
        }
        else
            ++it;
    }

    for (const auto& bullet : bullets)
    {
        bullet->update(deltaTime);
    }

    for (const auto& bullet : bullets)
    {
        auto pos = bullet->getPosition();
        auto size = bullet->getSize();

        if (pos.x < -size.x ||
            pos.x + size.x >= ScreenWidth + size.x ||
            pos.y < -size.y ||
            pos.y + size.y >= ScreenHeight + size.y)
        {
            bullet->destroy = true;
        }
    }
}
