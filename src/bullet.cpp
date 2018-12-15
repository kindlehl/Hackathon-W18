#include "../inc/bullet.h"

Bullet::Bullet()
{
	setRadius(5.f);
	setFillColor(sf::Color::Red);
	setPosition(10, 10);
}

//fire to the right
void Bullet::fireRight(float speed, float playerX, float playerY)
{
	setPosition(playerX, playerY);
	//change x direction of bullet
	move(speed, 0);
}

//fire to the left
void Bullet::fireLeft(float speed, float playerX, float playerY)
{
	setPosition(playerX, playerY);
	//change dir in x axis
	move(-speed, 0);
}


