#ifndef HITBOX_H
#define HITBOX_H

#include <SFML/Graphics.hpp>
#include <cmath>

extern sf::Vector2i screenSize;


class Hitbox : public sf::Drawable {
	public:
		Hitbox() {};
		virtual ~Hitbox() {};
		bool operator< (const Hitbox& H) const;
		float value() const;
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;
		virtual sf::IntRect getHitbox() const = 0;
};

struct hitboxCompare {
	bool operator() (const Hitbox* l, const Hitbox* r) const;
};

#endif
