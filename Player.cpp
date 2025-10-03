#include "Player.h"
#include <cmath>
#include <algorithm>
#include <iostream>

Player::Player
(
	Tmpl8::Sprite* playerSprite,
	Tmpl8::Sprite* engineSprite,
	Tmpl8::Sprite* engineEffectSprite,
	Tmpl8::Sprite* weaponSprite,
	Tmpl8::Sprite* bulletSprite,
	float x,
	float y,
	int width,
	int height,
	Tmpl8::vec2 velocity,
	Tmpl8::vec2 maxVelocity,
	Tmpl8::vec2 acceleration,
	Tmpl8::vec2 direction
) : Object(playerSprite, x, y, width, height, velocity, maxVelocity, acceleration, direction, 0.f, "player"),
	engineSprite(engineSprite),
	engineEffectSprite(engineEffectSprite),
	weaponSprite(weaponSprite),
	bulletSprite(bulletSprite),
	rotationSpeed(200.f),
	shootSpeed(.25f),
	shootTimer(0.f),	
	shootLeft(true)
{
	initAnimator();
}

void Player::initAnimator()
{
	animator = std::make_unique<Animator>();

	animator->addFrameAnimation(engineEffectSprite, 0.1f, 0, 2, [this]() {return abs(velocity.length()) <= 75.f; });

	animator->addFrameAnimation(engineEffectSprite, 0.1f, 3, 6, [this]() {return abs(velocity.length()) > 75.f; });

	animator->addFrameCycledAnimation(weaponSprite, shootSpeed / 2.f, 1, 2, "LeftShoot");
	animator->addFrameCycledAnimation(weaponSprite, shootSpeed / 2.f, 3, 4, "RightShoot");

	//animator->addFrameAnimation(sprite, 1.f, 0, 3, [this]() {return true; });
}

void Player::update(float deltaTime)
{
	shootTimer += deltaTime;
	
	x += velocity.x * direction.x * deltaTime;
	y += velocity.y * direction.y * deltaTime;

	x = fmodf(x + ScreenWidth, ScreenWidth);
	y = fmodf(y + ScreenHeight, ScreenHeight);

	updateBullets(deltaTime);

	animator->update(deltaTime);
}

void Player::render(Tmpl8::Surface& screen)
{
	for (const auto& bullet : bullets)
	{
		bullet->render(screen);
	}

	// Draw engine
	renderShipPart(*engineSprite, screen);

	// Draw weapon
	renderShipPart(*weaponSprite, screen);

	//// Draw engine effect
	renderShipPart(*engineEffectSprite, screen);

	// Draw main ship
	renderShipPart(*sprite, screen);

}


void Player::renderShipPart(Tmpl8::Sprite& sprite, Tmpl8::Surface& screen)
{
	//sprite.DrawScaledRotated(*this, screen);

	renderAt(sprite, x + 100, y, screen);
	
	/*sprite->DrawScaledRotated(x, y, width, height, angle, &screen);

	sprite->DrawScaledRotated(fmodf(x + width + ScreenWidth, ScreenWidth) - width, fmodf(y + height + ScreenHeight, ScreenHeight) - height, width, height, angle, &screen);
	sprite->DrawScaledRotated(fmodf(x + width + ScreenWidth, ScreenWidth) - width, fmodf(y + ScreenHeight, ScreenHeight), width, height, angle, &screen);
	sprite->DrawScaledRotated(fmodf(x + ScreenWidth, ScreenWidth), fmodf(y + height + ScreenHeight, ScreenHeight) - height, width, height, angle, &screen);*/

	/*renderAt(sprite, fmodf(x + width + ScreenWidth, ScreenWidth) - width, fmodf(y + height + ScreenHeight, ScreenHeight) - height, screen);
	renderAt(sprite, fmodf(x + width + ScreenWidth, ScreenWidth) - width, fmodf(y + ScreenHeight, ScreenHeight), screen);
	renderAt(sprite, fmodf(x + ScreenWidth, ScreenWidth), fmodf(y + height + ScreenHeight, ScreenHeight) - height, screen);*/
}



const std::vector<std::shared_ptr<Bullet>>& Player::getBullets() const
{
	return bullets;
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

void Player::shoot()
{
	if (shootTimer < shootSpeed)
		return;
	shootTimer = 0.f;

	Tmpl8::vec2 bulletPosition;
	if (shootLeft)
	{
		shootLeft = false;
		bulletPosition = getRotatedPoint({ x + static_cast<float>(width) / 3.f, y + static_cast<float>(height) / 3.f });
		animator->playAnimation("LeftShoot");
	}
	else
	{
		shootLeft = true;
		bulletPosition = getRotatedPoint({ x + static_cast<float>(width) / 3.f * 2.f, y + static_cast<float>(height) / 3.f});
		animator->playAnimation("RightShoot");
	}
		
	auto  bullet = std::make_shared<Bullet>
		(
			bulletSprite,
			bulletPosition.x - static_cast<float>(width) / 4.f,
			bulletPosition.y - static_cast<float>(width) / 4.f,
			width / 2,
			width / 2,
			Tmpl8::vec2{ 0.f, 0.f },
			Tmpl8::vec2{ 1500.f, 1500.f },
			Tmpl8::vec2{ 5000.f, 5000.f },
			direction,
			angle
		);

	bullets.push_back(bullet);
}
	
void Player::onCollisionEnter(std::shared_ptr<Object> object)
{
	//Enter
}

void Player::onCollisionStay(std::shared_ptr<Object> object)
{
	//Stay
}

void Player::onCollisionExit(std::shared_ptr<Object> object)
{
	//Exit
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

