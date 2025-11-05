#include "Player.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>
#include "CollisionHelper.hpp"

Player::Player
(
	const std::unordered_map<SpriteName, std::shared_ptr<Tmpl8::Sprite>>& sprites,
	const std::unordered_map<SoundName, Audio::Sound>& soundMap,
	Tmpl8::vec2 position,
	Tmpl8::vec2 size
) : Object(sprites.at(SpriteName::SHIP), position, size), spriteMap(sprites), soundMap(soundMap)
{
	setState(PlayerState::IDLE);

	initAnimator();
	initTimerManager();
	initEvents();

	//center = { 0.f, 0.f };
}

void Player::initAnimator()
{
	animator = std::make_unique<Animator>();

	//EngineEffect
	animator->addFrameAnimation(spriteMap[SpriteName::SHIP_ENGINE_EFFECT], 0.1f, 0, 2, [this]() {return abs(velocity.length()) <= 75.f; });

	animator->addFrameAnimation(spriteMap[SpriteName::SHIP_ENGINE_EFFECT], 0.1f, 3, 6, [this]() {return abs(velocity.length()) > 75.f; });

	//EngineEffect1
	animator->addFrameAnimation(spriteMap[SpriteName::SHIP_ENGINE_UPGRADED_EFFECT], 0.1f, 0, 3, [this]() {return abs(velocity.length()) <= 75.f; });

	animator->addFrameAnimation(spriteMap[SpriteName::SHIP_ENGINE_UPGRADED_EFFECT], 0.1f, 4, 7, [this]() {return abs(velocity.length()) > 75.f; });

	//Common weapon shooting
	animator->addFrameCycledAnimation(spriteMap[SpriteName::SHIP_WEAPON], shootTime / 2.f, 1, 2, "LeftShoot");
	animator->addFrameCycledAnimation(spriteMap[SpriteName::SHIP_WEAPON], shootTime / 2.f, 3, 4, "RightShoot");

	//Upgraded weapon charge and shooting
	animator->addFrameCycledAnimation(spriteMap[SpriteName::SHIP_WEAPON_UPGRADED], chargeTime / 6.f, 1, 6, "Charge", false);
	animator->addFrameCycledAnimation(spriteMap[SpriteName::SHIP_WEAPON_UPGRADED], shootTime / 2.f, 7, 11, "ChargeShoot", true);
}

void Player::initTimerManager()
{
	timerManager = std::make_unique<TimerManager>();
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
		renderShipPart(spriteMap[SpriteName::SHIP_WEAPON], screen);
	}
	else
	{
		renderShipPart(spriteMap[SpriteName::SHIP_WEAPON_UPGRADED], screen);
	}

	if (!upgradedEngine)
	{
		// Draw engine
		renderShipPart(spriteMap[SpriteName::SHIP_ENGINE], screen);

		// Draw engine effect
		renderShipPart(spriteMap[SpriteName::SHIP_ENGINE_EFFECT], screen);
	}
	else
	{
		// Draw engine
		renderShipPart(spriteMap[SpriteName::SHIP_ENGINE_UPGRADED], screen);

		// Draw engine effect
		renderShipPart(spriteMap[SpriteName::SHIP_ENGINE_UPGRADED_EFFECT], screen);
	}

	// Draw main ship
	renderShipPart(sprite, screen);
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
	acceleration = Tmpl8::vec2{600.f, 600.f};
	rotationSpeed = 220.f;

	soundMap[SoundName::SHIP_UPGRADE_ENGINE].replay();
}

void Player::move(float deltaTime)
{
	velocity += acceleration * direction * deltaTime;

	float speed =  velocity.length();
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
		soundMap[SoundName::SHIP_UPGRADE_WEAPON].replay();

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

		soundMap[SoundName::SHIP_DAMAGED].replay();

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
