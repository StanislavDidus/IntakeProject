#pragma once

#include "UIElement.h"

class FillBar : public UIElement
{
public:
	FillBar(Tmpl8::Pixel color, float x, float y, int width, int height, int minValue, int maxValue);

	void render(Tmpl8::Surface& screen, int value);
private:
	Tmpl8::Pixel color;
	bool horizontal; //true-horizontal, false-vertical
	int minValue, maxValue;
	int barWidth, barHeight;
};

