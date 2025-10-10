#include "Button.h"

Button::Button(std::shared_ptr<Tmpl8::Sprite> sprite, float x, float y, int width, int height, const std::function<void()>& func) : UIElement(sprite, x, y, width, height), func(func)
{
}

Button::~Button()
{
}

void Button::render(Tmpl8::Surface& screen)
{
	sprite->DrawScaled(position.x, position.y, size.x, size.y, screen);
}

void Button::CheckClick(Tmpl8::vec2i cpos, bool wasMouseDown, bool wasMouseUp) // cursor position
{
	if (cpos.x > position.x && cpos.x < position.x + size.x &&
		cpos.y > position.y && cpos.y < position.y + size.y)
	{
		if (sprite->getCurrentFrame() != 2)
		{
			if (wasMouseDown)
			{
				//func();
				sprite->SetFrame(2);
			}
			else
			{
				sprite->SetFrame(1);
			}
		}
		else
		{
			if (wasMouseUp)
			{
				sprite->SetFrame(0);
				func();
			}
		}
	}
	else
	{
		sprite->SetFrame(0);
	}
}
