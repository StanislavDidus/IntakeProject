#include "Player.hpp"
#include "game.hpp"
#include <Audio/Device.hpp>

void Player::setState(PlayerState state)
{
	if (this->state != state)
	{
		exitState(this->state);
		this->state = state;
		enterState(this->state);
	}
}

void Player::checkMove(float deltaTime)
{
	if (Tmpl8::Game::isKeyHold('w'))
		move(deltaTime);
	else
		stop(deltaTime);
}

void Player::checkRotation(float deltaTime)
{
	if (Tmpl8::Game::isKeyHold('a'))
		rotate(-1.f, deltaTime);

	if (Tmpl8::Game::isKeyHold('d'))
		rotate(1.f, deltaTime);
}


void Player::enterState(PlayerState state)
{
	switch (state)
	{
	case PlayerState::SUPERSHOOT:

		soundMap["charge"].replay();
		
		animator->playAnimation("Charge");

		chargeTimer = chargeTime;
		isChargedStarted = true;
		break;
	}
}

void Player::exitState(PlayerState state)
{
	switch (state)
	{
	case PlayerState::SUPERSHOOT:

		animator->stopAnimation("Charge");
		animator->playAnimation("ChargeShoot");

		soundMap["shoot1"].replay();

		canShoot = false;
		timerManager->addTimer(shootTime / 2.f * 5.f, [&] {upgraded = false; canShoot = true; shootTimer = .5f; });

		chargeTimer = std::max(chargeTimer, 0.f);
		float bulletForce = -300.f * chargeTimer + 400.f;
		//std::cout << bulletForce << "\n";

		float bulletWidth = size.x / 2.f, bulletHeight = size.y / 2.f;
		float areaMultiplier = 6.f;

		auto  bullet = std::make_shared<SuperBullet>
			(
				spriteMap["bullet1"],
				Tmpl8::vec2{ position.x + size.x / 2.f - bulletWidth * areaMultiplier / 2.f, position.y + size.y / 2.f - bulletHeight * areaMultiplier / 2.f },
				Tmpl8::vec2{ bulletWidth * areaMultiplier, bulletHeight * areaMultiplier},
				areaMultiplier
			);

		bullet->setVelocity(Tmpl8::vec2{ direction.x * bulletForce, direction.y * bulletForce });
		bullet->setMaxSpeed(1000.f);
		bullet->setDirection(direction);
		bullet->setAngle(angle);
		bullet->setTag("superBullet");

		bullets.push_back(bullet);

		velocity += -Tmpl8::vec2{ direction.x, direction.y } * bulletForce;

		isChargedStarted = false;

		break;
	}
}


void Player::updateIdle(float deltaTime)
{
	if ((Tmpl8::Game::isKeyHold('e') || Tmpl8::Game::isKeyHold(' ')) && !upgraded && canShoot)
	{
		setState(PlayerState::SHOOT);
	}
	else if ((Tmpl8::Game::isKeyHold('e') || Tmpl8::Game::isKeyHold(' ')) && upgraded && canShoot)
	{
		setState(PlayerState::SUPERSHOOT);
	}

}

void Player::updateShoot(float deltaTime)
{
	if (!Tmpl8::Game::isKeyHold('e') && !Tmpl8::Game::isKeyHold(' '))
	{
		setState(PlayerState::IDLE);
	}
	if (upgraded)
	{
		setState(PlayerState::SUPERSHOOT);
	}
	
	if (shootTimer > 0.f) return;
	else
	{
		shootTimer = shootTime;
	}


	Tmpl8::vec2 bulletPosition;
	if (shootLeft)
	{
		shootLeft = false;
		bulletPosition = getRotatedPoint({ position.x + size.x / 3.f, position.y + size.y / 3.f });
		animator->playAnimation("LeftShoot");
	}
	else
	{
		shootLeft = true;
		bulletPosition = getRotatedPoint({ position.x + size.x / 3.f * 2.f, position.y + size.y / 3.f });
		animator->playAnimation("RightShoot");
	}

	auto  bullet = std::make_shared<Bullet>
		(
			spriteMap["bullet"],
			Tmpl8::vec2{ bulletPosition.x - size.x / 4.f, bulletPosition.y - size.y / 4.f },
			Tmpl8::vec2{size.x / 2.f, size.y / 2.f}
		);

	bullet->setMaxSpeed(1500.f);
	bullet->setAcceleration({ 5000.f * direction.x, 5000.f * direction.y });
	bullet->setDirection(direction);
	bullet->setAngle(angle);
	bullet->setTag("bullet");

	bullets.push_back(bullet);

	velocity += -Tmpl8::vec2{ direction.x, direction.y } * 10.f;

	soundMap["shoot"].replay();
}

void Player::updateSuperShoot(float deltaTime)
{
	if (!Tmpl8::Game::isKeyHold('e') && !Tmpl8::Game::isKeyHold(' '))
	{
		setState(PlayerState::IDLE);

		soundMap["charge"].stop();
	}

	chargeTimer -= deltaTime;

	if (chargeTimer <= 0.f) soundMap["charge"].stop();
}

