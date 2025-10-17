#include "FillBar.h"

FillBar::FillBar(Tmpl8::Pixel color, Tmpl8::vec2 position, Tmpl8::vec2 size, float minValue, float  maxValue) :
	UIElement(nullptr, position, size), color(color), horizontal(true), minValue(minValue), maxValue(maxValue), barWidth(size.x / (maxValue - minValue)), barHeight(size.y / (maxValue - minValue))
{
}

void FillBar::render(Tmpl8::Surface& screen, float value)
{
	int x = static_cast<int>(position.x);
	int y = static_cast<int>(position.y);

	value = clamp(value, minValue, maxValue);

	if (horizontal)
	{
		screen.Bar<true>(x, y, static_cast<int>(position.x + barWidth * value), y + static_cast<int>(size.y), color);
	}
}
