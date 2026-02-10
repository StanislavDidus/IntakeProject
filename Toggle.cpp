#include "Toggle.hpp"

Toggle::Toggle
(
	std::shared_ptr<Tmpl8::Sprite> sprite,
	Tmpl8::vec2 position,
	Tmpl8::vec2 size,
	std::function<void()> off,
	std::function<void()> on,
	bool isOn) :
	UIElement(sprite, position, size), isOn(isOn), on(std::move(on)), off(std::move(off))
{
	if (isOn)
	{
		this->on();
		sprite->SetFrame(1);
	}
	else
	{
		this->off();
		sprite->SetFrame(0);
	}
}

Toggle::~Toggle()
{

}

void Toggle::CheckClick(const Tmpl8::vec2i& cpos, bool wasMouseDown, bool wasMouseUp)
{
	bool isCovered = intersects(cpos.x, cpos.y);

	if (isCovered && wasMouseUp)
	{
		isOn = !isOn;

		if (isOn)
		{
			on();
			sprite->SetFrame(1);
		}
		else
		{
			off();
			sprite->SetFrame(0);
		}
	}
}

void Toggle::update(float deltaTime)
{
}

void Toggle::render(Tmpl8::Surface& screen)
{
	int x = static_cast<int>(position.x);
	int y = static_cast<int>(position.y);
	int width = static_cast<int>(size.x);
	int height = static_cast<int>(size.y);

	sprite->DrawScaled(x, y, width, height, screen);
}

inline bool Toggle::intersects(int x, int y) const
{
	return x > position.x && x < position.x + size.x &&
		y > position.y && y < position.y + size.y;
}
