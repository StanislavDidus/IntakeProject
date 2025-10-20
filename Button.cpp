#include "Button.hpp"

Button::Button(std::shared_ptr<Tmpl8::Sprite> sprite, const std::unordered_map<std::string, Audio::Sound>& sounds, Tmpl8::vec2 position, Tmpl8::vec2 size, const std::function<void()>& func) : 
	UIElement(sprite, position, size), func(func), isHeld(false), sounds(sounds), wasCovered(false)
{
}

Button::~Button()
{
}

void Button::render(Tmpl8::Surface& screen)
{
	sprite->DrawScaled(static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(size.x), static_cast<int>(size.y), screen);
}

void Button::CheckClick(Tmpl8::vec2i cpos, bool wasMouseDown, bool wasMouseUp) // cursor position
{
	bool isCovered = intersects(cpos.x, cpos.y);

	//If button was not held, we check condition: enter, stay, exit
	//If was, check if it is still held or cursor is not on a button any more
	if (!isHeld)
	{
		if (isCovered && !wasCovered)
		{
			enter();
		}
		else if (isCovered && wasCovered)
		{
			stay(wasMouseDown, wasMouseUp);
		}
		else if (!isCovered && wasCovered)
		{
			exit();
		}
	}
	else
	{
		//If cursor is not covering button - disable isHeld
		if (!isCovered)
		{
			isHeld = false;
			sprite->SetFrame(0);
		}
		//If you released the button - call a function
		else if (wasMouseUp)
		{
			isHeld = false;
			sprite->SetFrame(1);
			func();

			sounds["buttonUp"].replay();
		}
	}

	wasCovered = isCovered;
}

void Button::enter()
{
	sprite->SetFrame(1);

	sounds["buttonCover"].replay();
}

void Button::stay(bool down, bool up)
{
	//If during covering the button you hold it, change its sprite
	if (down)
	{
		sprite->SetFrame(2);
		isHeld = true;
	}
}

void Button::exit()
{
	sprite->SetFrame(0);
}

inline bool Button::intersects(int x, int y) const
{
	return x > position.x && x < position.x + size.x &&
		y > position.y && y < position.y + size.y;
}
