#include "Player.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include "Audio/Device.hpp"

Player::Player
(
	std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>>& sprites,
	const std::unordered_map<std::string, Audio::Sound>& soundMap,
	float x,
	float y,
	int width,
	int height,
	Tmpl8::vec2 velocity,
	float maxSpeed,
	Tmpl8::vec2 acceleration,
	Tmpl8::vec2 direction
) : Object(sprites["ship"], x, y, width, height, velocity, maxSpeed, acceleration, direction, 0.f, "player"),
spriteMap(sprites),
soundMap(soundMap),
rotationSpeed(200.f),
shootSpeed(.25f),
shootTimer(0.f),
canShoot(true),
shootLeft(true),
maxHealth(4),
currentHealth(maxHealth),
invulnerableTime(2.5f),
isHit(false),
blinkTimer(0.30f),
blinkTime(blinkTimer),
blink(false),
upgraded(false),
chargeTime(1.f),
isChargedStarted(false),
chargeTimer(chargeTime)
{
	setState(PlayerState::IDLE);

	initAnimator();
	initTimerManager();
}

void Player::initAnimator()
{
	animator = std::make_unique<Animator>();

	animator->addFrameAnimation(spriteMap["engineEffect"], 0.1f, 0, 2, [this]() {return abs(velocity.length()) <= 75.f; });

	animator->addFrameAnimation(spriteMap["engineEffect"], 0.1f, 3, 6, [this]() {return abs(velocity.length()) > 75.f; });

	//Common weapon shooting
	animator->addFrameCycledAnimation(spriteMap["weapon"], shootSpeed / 2.f, 1, 2, "LeftShoot");
	animator->addFrameCycledAnimation(spriteMap["weapon"], shootSpeed / 2.f, 3, 4, "RightShoot");

	//Upgraded weapon charge and shooting
	animator->addFrameCycledAnimation(spriteMap["weapon1"], chargeTime / 6.f, 1, 6, "Charge", false);
	animator->addFrameCycledAnimation(spriteMap["weapon1"], shootSpeed / 2.f, 7, 11, "ChargeShoot", true);

	//animator->addFrameAnimation(sprite, 1.f, 0, 3, [this]() {return true; });
}

void Player::initTimerManager()
{
	timerManager = std::make_unique<TimerManager>();
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

	checkMove(deltaTime);
	checkRotation(deltaTime);

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

	x = fmodf(x + ScreenWidth, ScreenWidth);
	y = fmodf(y + ScreenHeight, ScreenHeight);
}

void Player::render(Tmpl8::Surface& screen)
{
	for (const auto& bullet : bullets)
	{
		bullet->render(screen);
	}

	// Draw engine
	renderShipPart(*spriteMap["shipEngine"].get(), screen);

	// Draw weapon
	if (!upgraded)
		renderShipPart(*spriteMap["weapon"].get(), screen);
	else
		renderShipPart(*spriteMap["weapon1"].get(), screen);

	//// Draw engine effect
	renderShipPart(*spriteMap["engineEffect"].get(), screen);

	// Draw main ship
	renderShipPart(*sprite, screen);

}


void Player::renderShipPart(Tmpl8::Sprite& sprite, Tmpl8::Surface& screen)
{
	//sprite.DrawScaledRotated(*this, screen);

	if (blink) return;

	renderAt(sprite, x, y, screen);

	renderAt(sprite, fmodf(x + width + ScreenWidth, ScreenWidth) - width, fmodf(y + height + ScreenHeight, ScreenHeight) - height, screen);
	renderAt(sprite, fmodf(x + width + ScreenWidth, ScreenWidth) - width, fmodf(y + ScreenHeight, ScreenHeight), screen);
	renderAt(sprite, fmodf(x + ScreenWidth, ScreenWidth), fmodf(y + height + ScreenHeight, ScreenHeight) - height, screen);
}



const std::vector<std::shared_ptr<IBullet>>& Player::getBullets() const
{
	return bullets;
}

void Player::move(float deltaTime)
{
	velocity += acceleration * direction * deltaTime;

	float speed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
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
		soundMap["upgrade"].replay();

		object->destroy = true;
		upgraded = true;
	}
}

void Player::onCollisionStay(std::shared_ptr<Object> object, float deltaTime)
{
	//Stay
	if (object->getTag() == "asteroid" && !isHit)
	{
		currentHealth--;
		isHit = true;

		soundMap["shipDamaged"].replay();

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

bool Player::isUpgraded() const
{
	return upgraded;
}

void Player::updateBullets(float deltaTime)
{
	for (const auto& bullet : bullets)
	{
		bullet->update(deltaTime);
	}

	for (const auto& bullet : bullets)
	{
		auto& pos = bullet->getPosition();
		auto& size = bullet->getSize();

		if (pos.x < -size.x ||
			pos.x + size.x >= ScreenWidth + size.x ||
			pos.y < -size.y ||
			pos.y + size.y >= ScreenHeight + size.y)
		{
			bullet->destroy = true;
		}
	}

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
}
