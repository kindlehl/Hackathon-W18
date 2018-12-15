#include "../inc/hitbox.h"

bool Hitbox::operator< (const Hitbox& l) const {
	return value() > l.value();
}

int Hitbox::value() const {
	sf::IntRect r = getHitbox();
	int x = r.left + r.width;
	int y = screenSize.y - r.top;
	return y + x;
}
