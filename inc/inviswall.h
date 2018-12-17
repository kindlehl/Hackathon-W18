#ifndef INVIS_H
#define INVIS_H

#include "../inc/hitbox.h"

class InvisWall : public Hitbox {
public:
	InvisWall() = delete;
	InvisWall(sf::IntRect r) : hitbox(r) {type = ENVIRONMENT_TYPE;};
	void draw(sf::RenderTarget&, sf::RenderStates) const {};
	sf::IntRect getHitbox() const { return hitbox; };
	sf::IntRect hitbox;
};

#endif
