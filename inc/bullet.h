#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include "../inc/hitbox.h"

class Bullet : public Hitbox
{
public:

	Bullet(sf::IntRect);
	Bullet(int, int, int, int);
	sf::IntRect hitbox;
	void fireRight(int);
	//void fireRight(float);
	//void fireLeft(float, float, float);
	sf::IntRect getHitbox() const;
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
	//virtual void draw(sf::RenderWindow &window);

protected:
private:
	sf::RectangleShape bullet;

};

#endif //BULLET_H