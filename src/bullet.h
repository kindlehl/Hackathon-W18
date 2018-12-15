#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>

class Bullet : public sf::CircleShape
{
public:

	Bullet();
	void fireRight(float);
	void fireLeft(float);

protected:
private:

};

#endif //BULLET_H