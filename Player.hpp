#pragma once

#include "template.h"
#include "surface.h"
#include <iostream>
#include <algorithm>
#include <vector>

#include "Object.hpp"
#include "Animator.hpp"
#include "Bullet.hpp"
#include "TimerManager.hpp"
#include "Audio/Device.hpp"
#include "EventBus.hpp"

enum class PlayerState
{
	NONE,
	IDLE, 
	SHOOT,
	SUPERSHOOT
};

class Player : public Object, public Listener
{
public:
	Player
	(
		const std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>>& sprites,
		const std::unordered_map<std::string, Audio::Sound>& soundMap,
		Tmpl8::vec2 position,
		Tmpl8::vec2 size
	);

	void update(float deltaTime) override;
	void render(Tmpl8::Surface& screen) override;

	const std::vector<std::shared_ptr<IBullet>>& getBullets() const;
	int getHealth() const;

	void move(float deltaTime) override;
	void rotate(float angle, float deltaTime);

	void onCollisionEnter(std::shared_ptr<Object> object) override;
	void onCollisionStay(std::shared_ptr<Object> object, float deltaTime) override;
	void onCollisionExit(std::shared_ptr<Object> object) override;

	bool isUpgraded() const;
private:
	void initAnimator();
	void initTimerManager();
	void initEvents();

	//void generateSprite();

	void setState(PlayerState state);

	void checkMove(float deltaTime);
	void checkRotation(float deltaTime);

	void enterState(PlayerState state);
	void exitState(PlayerState state);

	void updateBullets(float deltaTime);

	//States
	void updateIdle(float deltaTime);
	void updateShoot(float deltaTime);
	void updateSuperShoot(float deltaTime);
	void updateDamage(float deltaTime);

	void renderShipPart(std::shared_ptr<Tmpl8::Sprite> sprite, Tmpl8::Surface& screen);

	std::vector<std::shared_ptr<IBullet>> bullets;

	std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>> spriteMap;
	std::unordered_map<std::string, Audio::Sound> soundMap;

	std::unique_ptr<Animator> animator;

	std::unique_ptr<TimerManager> timerManager;

	//State
	PlayerState state = PlayerState::NONE;

	//Health Component
	int maxHealth = 4, currentHealth = maxHealth;
	float invulnerableTime = 2.f;
	bool isHit = false;

	//Blinking after getting hit
	float blinkTime = 0.3f, blinkTimer = blinkTime;
	bool blink = false;

	//Shooting
	float shootTime = 0.3f, shootTimer = shootTime;
	bool shootLeft = true;
	bool canShoot = true;

	float rotationSpeed = 200.f;

	//Upgrade
	bool upgraded = false;
	bool isChargedStarted = false;
	float chargeTime = 1.f, chargeTimer = chargeTime;

	//Explosion
	float explosionAnimationSpeed = 0.1f;	
};

