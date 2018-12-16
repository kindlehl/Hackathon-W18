#include "../inc/bullet.h"

// ctor takes in the 
Bullet::Bullet(sf::IntRect rectIn, sf::Vector2i velocity) {
	hitbox.left = rectIn.left + rectIn.width/2;
	hitbox.top = rectIn.top - rectIn.height/2;
	hitbox.width = rectIn.width/4;
	hitbox.height = rectIn.height/4;
	bullet.setFillColor(sf::Color::Red);
	bullet.setPosition(hitbox.left, hitbox.top);
	bullet.setSize(sf::Vector2f(hitbox.width, hitbox.height));

	xVel = velocity.x;
	yVel = velocity.y;
}
Bullet::Bullet(int leftIn, int topIn, int widthIn, int heightIn)
{
	
	hitbox.left = leftIn;
	hitbox.top = topIn;
	hitbox.width = widthIn;
	hitbox.height = heightIn;
}

void Bullet::update()
{
	hitbox.left += xVel;
	hitbox.top += yVel;
	bullet.move(xVel, yVel);
}


sf::IntRect Bullet::getHitbox() const
{
	return hitbox;
}
void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(bullet);
}


