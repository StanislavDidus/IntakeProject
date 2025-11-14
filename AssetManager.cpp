#include "AssetManager.hpp"

AssetManager::AssetManager()
{
	
}

AssetManager::~AssetManager()
{
}

std::shared_ptr<Tmpl8::Sprite> AssetManager::getSprite(SpriteName name) const
{
	return spriteMap.at(name);
}

Audio::Sound AssetManager::getSound(SoundName name) const
{
	return soundMap.at(name);
}

void AssetManager::loadSprites()
{
	//Ship
	loadSprite(SpriteName::SHIP, "assets/Sprites/ship.png", 4);
	loadSprite(SpriteName::SHIP_ENGINE, "assets/Sprites/engine.png", 1);
	loadSprite(SpriteName::SHIP_ENGINE_UPGRADED, "assets/Sprites/engine1.png", 1);
	loadSprite(SpriteName::SHIP_ENGINE_EFFECT, "assets/Sprites/engineEffect.png", 7);
	loadSprite(SpriteName::SHIP_ENGINE_UPGRADED_EFFECT, "assets/Sprites/engineEffect1.png", 8);
	loadSprite(SpriteName::SHIP_EXPLOSION, "assets/Sprites/explosion.png", 11);
	loadSprite(SpriteName::SHIP_WEAPON, "assets/Sprites/weapon.png", 7);
	loadSprite(SpriteName::SHIP_WEAPON_UPGRADED, "assets/Sprites/weapon1.png", 12);
	loadSprite(SpriteName::SHIP_HIT_EFFECT, "assets/Sprites/hitEffect.png", 7);
	loadSprite(SpriteName::SHIP_BULLET, "assets/Sprites/bullet.png", 4);
	loadSprite(SpriteName::SHIP_BULLET_UPGRADED, "assets/Sprites/bullet1.png", 10);
	loadSprite(SpriteName::SHIP_BULLET_UPGRADED_TRAIL, "assets/Sprites/bulletEffect.png", 4);

	// Objects
	loadSprite(SpriteName::SHEEP, "assets/Sprites/sheep.png", 1);
	loadSprite(SpriteName::ASTEROID, "assets/Sprites/asteroid.png", 3);
	loadSprite(SpriteName::ASTEROID_EXPLOSION, "assets/Sprites/explosion1.png", 10);
	loadSprite(SpriteName::SPACE, "assets/Sprites/space.png", 2);
	loadSprite(SpriteName::UPGRADE, "assets/Sprites/upgrade.png", 1);

	// UI
	loadSprite(SpriteName::BUTTON_START, "assets/Sprites/startButton.png", 3);
	loadSprite(SpriteName::BUTTON_SCORE, "assets/Sprites/scoreButton.png", 3);
	loadSprite(SpriteName::BUTTON_EXIT, "assets/Sprites/exitButton.png", 3);
	loadSprite(SpriteName::BUTTON_DELETE, "assets/Sprites/deleteButton.png", 3);
	loadSprite(SpriteName::BUTTON_CANCEL, "assets/Sprites/cancelButton.png", 3);
	loadSprite(SpriteName::UI_LOGO, "assets/Sprites/logo.png", 1);
	loadSprite(SpriteName::UI_CLOCK, "assets/Sprites/clock.png", 1);
	loadSprite(SpriteName::UI_SMILEYS, "assets/Sprites/smileys.png", 20);
	loadSprite(SpriteName::UI_LABEL, "assets/Sprites/label.png", 1);
	loadSprite(SpriteName::UI_TOGGLE_SOUND, "assets/Sprites/soundToggle.png", 2);
}

void AssetManager::loadSounds()
{
	loadSound(SoundName::MENU_MUSIC, "assets/Sounds/music.mp3", Audio::Sound::Type::Stream);
	auto& menuMusic = soundMap[SoundName::MENU_MUSIC];
	menuMusic.setLooping(true);
	menuMusic.replay();

	loadSound(SoundName::SHIP_SHOOT, "assets/Sounds/shoot.mp3");
	loadSound(SoundName::SHIP_SHOOT_UPGRADED, "assets/Sounds/shoot1.mp3");
	loadSound(SoundName::SHIP_CHARGE, "assets/Sounds/charge.mp3");
	loadSound(SoundName::SHIP_UPGRADE_WEAPON, "assets/Sounds/upgradeWeapon.mp3");
	loadSound(SoundName::SHIP_UPGRADE_ENGINE, "assets/Sounds/upgradeEngine.mp3");
	loadSound(SoundName::SHIP_DAMAGED, "assets/Sounds/shipDamaged.mp3");
	loadSound(SoundName::SHIP_DESTROYED, "assets/Sounds/shipDestroyed.mp3");
	loadSound(SoundName::BULLET_HIT, "assets/Sounds/hitEffect.mp3");
	loadSound(SoundName::ASTEROID_DESTROYED, "assets/Sounds/asteroidDestroyed.mp3");
	loadSound(SoundName::GAME_OVER, "assets/Sounds/gameOver.mp3");
	loadSound(SoundName::BUTTON_COVER, "assets/Sounds/buttonCover.mp3");
	loadSound(SoundName::BUTTON_UP, "assets/Sounds/buttonUp.mp3");

	Audio::Device::setMasterVolume(0.1f);
}

void AssetManager::loadSprite(SpriteName spriteName, const std::string& path, int frames)
{
	spriteMap[spriteName] = std::make_shared<Tmpl8::Sprite>(new Tmpl8::Surface(&path[0]), frames);
}

void AssetManager::loadSound(SoundName soundName, const std::string& path, Audio::Sound::Type type)
{
	soundMap[soundName] = Audio::Sound{ path, type };
}