#include "bullet.h"

Bullet::Bullet()
{
	setRadius(1.f);
}

//fire to the right
void Bullet::fireRight(float speed)
{
	//change x direction of bullet
	move(speed, 0);
}

//fire to the left
void Bullet::fireLeft(float speed)
{
	//change dir in x axis
	move(-speed, 0);
}
