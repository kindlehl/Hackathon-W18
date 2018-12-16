#include "../inc/bullet.h"

// ctor takes in the 
Bullet::Bullet(sf::IntRect rectIn) {
	hitbox.left = rectIn.left + rectIn.width/2;
	hitbox.top = rectIn.top - rectIn.height/2;
	hitbox.width = rectIn.width/4;
	hitbox.height = rectIn.height/4;
	bullet.setFillColor(sf::Color::Red);
	bullet.setPosition(hitbox.left, hitbox.top);
	bullet.setSize(sf::Vector2f(hitbox.width, hitbox.height));
}
Bullet::Bullet(int leftIn, int topIn, int widthIn, int heightIn)
{
	
	hitbox.left = leftIn;
	hitbox.top = topIn;
	hitbox.width = widthIn;
	hitbox.height = heightIn;
}

//fire to the right
void Bullet::fireRight(int speed)
{
	//setPosition(posIn);
	//change x direction of bullet
	hitbox.left += speed;
	bullet.move(speed, 0);
}

//fire to the left
void Bullet::fireLeft(int speed)
{
	hitbox.left -= speed;
	bullet.move(-speed, 0);
}

void Bullet::fireUp(int speed)
{
	hitbox.top -= speed;
	bullet.move(0, -speed);
}

void Bullet::fireDown(int speed)
{
	hitbox.top += speed;
	bullet.move(0, speed);
}

sf::IntRect Bullet::getHitbox() const
{
	return hitbox;
}
void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(bullet);
}


