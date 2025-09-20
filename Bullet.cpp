#include "Bullet.h"
#include "CollisionManager.h"

void Bullet::onCollisionEnter(const CollisionEvent& event)
{
	if (event.collider->getTag() == "asteroid")
	{
		destroy = true;
	}
}
