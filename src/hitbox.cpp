#include "../inc/hitbox.h"

bool hitboxCompare::operator() (const Hitbox* l, const Hitbox* r) const {
	return l->value() > r->value();
}

float Hitbox::value() const {
	sf::IntRect r = getHitbox();
	int x = r.left + r.width/2;
	int y = screenSize.y - r.top - r.height/2;
	//return distance from bottom corner of map to top corner of map (z-index)
	return std::sqrt(y*y + x*x);
}
