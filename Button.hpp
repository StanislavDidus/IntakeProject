#pragma once

#include "UIElement.hpp"
#include <functional>
#include "Audio/Device.hpp"
#include "AssetNames.hpp"
#include "AssetManager.hpp"

class Button : public UIElement
{
public:
	Button(std::shared_ptr<Tmpl8::Sprite> sprite, std::shared_ptr<AssetManager> assetManager, Tmpl8::vec2 position, Tmpl8::vec2 size, const std::function<void()>& func);
	 ~Button() override;

	void render(Tmpl8::Surface& screen) override;

	void CheckClick(Tmpl8::vec2i cpos, bool wasMouseDown, bool wasMouseUp);
private:
	void enter();
	void stay(bool down, bool up);
	void exit();

	inline bool intersects(int x, int y) const;

	std::shared_ptr<AssetManager> assetManager;

	std::function<void()> func;

	bool wasCovered;
	bool isHeld;
};

