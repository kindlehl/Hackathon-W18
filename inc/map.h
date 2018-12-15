#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <vector>

class Map : public sf::Drawable {
	public:
		Map();
		Map(std::string);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const ;


#endif
