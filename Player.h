#pragma once

#include "template.h"
#include "surface.h"
#include <iostream>
#include <algorithm>
#include <vector>

#include "Object.h"
#include "Animator.h"
#include "Bullet.h"
#include "SuperBullet.h"
#include "TimerManager.h"

enum class PlayerState
{
	NONE,
	IDLE, 
	SHOOT,
	SUPERSHOOT
};

class Player : public Object
{
public:
	Player
	(
		std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>>& sprites,
		float x,
		float y,
		int width,
		int height,
		Tmpl8::vec2 velocity,
		float maxSpeed,
		Tmpl8::vec2 acceleration,
		Tmpl8::vec2 direction
	);

	void update(float deltaTime) override;
	void render(Tmpl8::Surface& screen) override;

	const std::vector<std::shared_ptr<Bullet>>& getBullets() const;

	void move(float deltaTime) override;
	void rotate(float angle, float deltaTime);

	void onCollisionEnter(std::shared_ptr<Object> object) override;
	void onCollisionStay(std::shared_ptr<Object> object) override;
	void onCollisionExit(std::shared_ptr<Object> object) override;

	bool isUpgraded() const;
private:
	void initAnimator();
	void initTimerManager();

	void setState(PlayerState state);

	void checkMove(float deltaTime);
	void checkRotation(float deltaTime);

	void enterState(PlayerState state);
	void exitState(PlayerState state);

	void updateBullets(float deltaTime);

	void updateIdle(float deltaTime);
	void updateShoot(float deltaTime);
	void updateSuperShoot(float deltaTime);

	void renderShipPart(Tmpl8::Sprite& sprite, Tmpl8::Surface& screen);

	std::vector<std::shared_ptr<Bullet>> bullets;

	std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>> sprites;

	std::unique_ptr<Animator> animator;

	std::unique_ptr<TimerManager> timerManager;

	//State
	PlayerState state = PlayerState::NONE;

	//Health Component
	int maxHealth, currentHealth;
	float invulnerableTime;
	bool isHit;

	//Blinking after getting hit
	float blinkTimer, blinkTime;
	bool blink;

	//Shooting
	float shootSpeed;
	float shootTimer;
	bool shootLeft;
	bool canShoot;

	float rotationSpeed;

	//Upgrade
	bool upgraded;
	bool isChargedStarted;
	float chargeTimer, chargeTime;
};

