#pragma once

#include "UIElement.h"
#include <functional>

class Button : public UIElement
{
public:
	Button(std::shared_ptr<Tmpl8::Sprite> sprite, float x, float y, int width, int height, const std::function<void()>& func);
	virtual ~Button();

	void render(Tmpl8::Surface& screen) override;

	void CheckClick(Tmpl8::vec2i cpos, bool wasMouseDown, bool wasMouseUp);
private:
	std::function<void()> func;
};

