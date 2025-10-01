#include "FillBar.h"

FillBar::FillBar(Tmpl8::Pixel color, float x, float y, int width, int height, int minValue, int maxValue) : 
	UIElement(nullptr, x, y, width, height), color(color), horizontal(true), minValue(minValue), maxValue(maxValue), barWidth(width / (maxValue - minValue)), barHeight(height / (maxValue - minValue))
{
}

void FillBar::render(Tmpl8::Surface& screen, int value)
{
	int x = static_cast<int>(position.x);
	int y = static_cast<int>(position.y);

	if (value > maxValue) value = maxValue;
	if (value < minValue) value = minValue;

	if(horizontal)
		screen.Bar<true>(x, y, x + barWidth * value, y + size.y, color);
}
