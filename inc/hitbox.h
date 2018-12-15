#ifndef HITBOX_H
#define HITBOX_H

#include <SFML/Graphics.hpp>

extern sf::Vector2i screenSize;

class Hitbox : public sf::Drawable {
	public:
		Hitbox() {};
		virtual ~Hitbox() {};
		bool operator< (const Hitbox& H) const;
		int value() const;
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;
		virtual sf::IntRect getHitbox() const = 0;
};

#endif
