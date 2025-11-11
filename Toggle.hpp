#pragma once

#include "UIElement.hpp"
#include <functional>

class Toggle : public UIElement
{
public:
	Toggle(
		std::shared_ptr<Tmpl8::Sprite> sprite,
		const Tmpl8::vec2& position,
		const Tmpl8::vec2& size,
		const std::function<void()>& off,
		const std::function<void()>& on,
		bool isOn = false
	);
	virtual ~Toggle();

	void CheckClick(const Tmpl8::vec2i& cpos, bool wasMouseDown, bool wasMouseUp);

	void update(float deltaTime) override;
	void render(Tmpl8::Surface& screen) override;
private:
	std::function<void()> off;
	std::function<void()> on;

	inline bool intersects(int x, int y) const;

	bool isOn;
};

