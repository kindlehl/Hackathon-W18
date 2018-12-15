#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>

class Bullet : public sf::CircleShape
{
public:

	Bullet();
	void fireRight(float, sf::Vector2f);
	void fireRight(float);
	void fireLeft(float, float, float);
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states);
	//virtual void draw(sf::RenderWindow &window);

protected:
private:

};

#endif //BULLET_H