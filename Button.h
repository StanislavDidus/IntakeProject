#pragma once

#include "UIElement.h"
#include <functional>
#include "Audio/Device.hpp"

class Button : public UIElement
{
public:
	Button(std::shared_ptr<Tmpl8::Sprite> sprite, const std::unordered_map<std::string, Audio::Sound>& sounds, Tmpl8::vec2 position, Tmpl8::vec2 size, const std::function<void()>& func);
	virtual ~Button();

	void render(Tmpl8::Surface& screen) override;

	void CheckClick(Tmpl8::vec2i cpos, bool wasMouseDown, bool wasMouseUp);
private:
	void enter();
	void stay(bool down, bool up);
	void exit();

	inline bool intersects(int x, int y) const;

	std::unordered_map<std::string, Audio::Sound> sounds;

	std::function<void()> func;

	bool wasCovered;
	bool isHeld;
};

