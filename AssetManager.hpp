#pragma once

#include <unordered_map>

#include "AssetNames.hpp"
#include "Audio/Device.hpp"
#include "template.h"
#include "surface.h"

class AssetManager
{
public:
	AssetManager();
	virtual ~AssetManager();

	void loadSprites();
	void loadSounds();

	std::shared_ptr<Tmpl8::Sprite> getSprite (SpriteName name) const;
	Audio::Sound                   getSound  (SoundName name)  const;
private:
	void loadSprite(SpriteName spriteName, const std::string& path, int frames);
	void loadSound(SoundName soundName, const std::string& path, Audio::Sound::Type type = Audio::Sound::Type::Sound);

	std::unordered_map<SpriteName, std::shared_ptr<Tmpl8::Sprite>> spriteMap;
	std::unordered_map<SoundName, Audio::Sound> soundMap;
};

