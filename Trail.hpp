#include "Object.hpp"

class Trail : public Object
{
public:
	Trail(std::shared_ptr<Tmpl8::Sprite> sprite, Tmpl8::vec2 position, Tmpl8::vec2 size, std::shared_ptr<Object> asteroid, Tmpl8::vec2 bulletSize);
	virtual ~Trail();

	void update(float deltaTime) override;
	void render(Tmpl8::Surface& screen) override;

	std::shared_ptr<Object> asteroid;
private:
	Tmpl8::vec2 bulletSize;
};