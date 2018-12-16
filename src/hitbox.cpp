#include "../inc/hitbox.h"

int Hitbox::numObjects = 0;

bool hitboxCompare::operator() (const Hitbox* l, const Hitbox* r) const {
	return l->value() > r->value();
}

float Hitbox::value() const {
	sf::IntRect r = getHitbox();
	//get coordinates of top-right corner of hitbox
	//int x = r.left + r.width;
	//int y = screenSize.y - r.top;
	int x = r.left;
	int y = screenSize.y - r.top - r.height;
	//return distance from bottom corner of map to top corner of map (z-index)
	return std::sqrt(y*y + x*x);
}
