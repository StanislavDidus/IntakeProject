#pragma once

#include "UIElement.hpp"

class FillBar : public UIElement
{
public:
	FillBar(Tmpl8::Pixel color, Tmpl8::vec2 position, Tmpl8::vec2 size, float minValue, float maxValue);

	void render(Tmpl8::Surface& screen, float value);
private:
	Tmpl8::Pixel color;
	bool horizontal; //true-horizontal, false-vertical
	float minValue, maxValue;
	float barWidth, barHeight;
};

