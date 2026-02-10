#pragma once

#include "UIElement.hpp"

class FillBar : public UIElement
{
public:
	FillBar(Tmpl8::Pixel color, Tmpl8::vec2 position, Tmpl8::vec2 size, float minValue, float maxValue);

	//Setters
	void setValue(float value);

	//Getters
	float getValue() const;

	void render(Tmpl8::Surface& screen) override;
private:
	Tmpl8::Pixel color;
	float minValue, maxValue;
	float barWidth, barHeight;
	float value;
};

