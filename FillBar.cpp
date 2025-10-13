#include "FillBar.h"

FillBar::FillBar(Tmpl8::Pixel color, float x, float y, int width, int height, float minValue, float  maxValue) :
	UIElement(nullptr, x, y, width, height), color(color), horizontal(true), minValue(minValue), maxValue(maxValue), barWidth(static_cast<float>(width) / (maxValue - minValue)), barHeight(static_cast<float>(height) / (maxValue - minValue))
{
}

void FillBar::render(Tmpl8::Surface& screen, float value)
{
	int x = static_cast<int>(position.x);
	int y = static_cast<int>(position.y);

	if (value > maxValue) value = maxValue;
	if (value < minValue) value = minValue;

	if (horizontal)
	{
		screen.Bar<true>(x, y, static_cast<int>(static_cast<float>(x) + barWidth * value), y + size.y, color);
	}
}
