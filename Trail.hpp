#include "Object.hpp"
#include "Animator.hpp"

class Trail : public Object
{
public:
	Trail(std::shared_ptr<Tmpl8::Sprite> sprite, Tmpl8::vec2 position, Tmpl8::vec2 size, std::shared_ptr<Object> asteroid);
	~Trail() override;

	void update(float deltaTime) override;
	void render(Tmpl8::Surface& screen) override;

	std::shared_ptr<Object> asteroid;
private:
	void initAnimations();

	std::unique_ptr<Animator> animator;

	Tmpl8::vec2 p0{};
	Tmpl8::vec2 p1{};
	Tmpl8::vec2 p2{};
	Tmpl8::vec2 p3{};
};