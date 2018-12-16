#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include "../inc/hitbox.h"

class Bullet : public Hitbox
{
public:

	Bullet(sf::IntRect, sf::Vector2i);
	Bullet(int, int, int, int);
	sf::IntRect hitbox;
	void update();
	sf::IntRect getHitbox() const;
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
	//virtual void draw(sf::RenderWindow &window);

protected:
private:
	sf::RectangleShape bullet;
	int xVel;
	int yVel;

};

#endif //BULLET_H