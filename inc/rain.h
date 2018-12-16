#ifndef RAIN_H
#define RAIN_H

#define WIDTH 10

#include <SFML/Graphics.hpp>
#include <vector> 
#include <random> 
#include <ctime> 

extern sf::Vector2i screenSize;

class Rain : public sf::Drawable {
public:
	Rain();
	void update();
	sf::Font font;
	std::vector<sf::Text> codelines;
	void draw(sf::RenderTarget&, sf::RenderStates) const;
};

#endif
