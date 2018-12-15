#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>

class Bullet : public sf::CircleShape
{
public:

	Bullet();
	void fireRight(float, sf::Vector2f);
	void fireLeft(float, float, float);
	void draw(sf::RenderWindow &window);

protected:
private:

};

#endif //BULLET_H