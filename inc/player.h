#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player : public sf::Drawable {
public:
	Player();
	sf::Sprite hitbox;
	sf::Texture static_stick;
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
	void action(int KeyEnumValue);
};

#endif
