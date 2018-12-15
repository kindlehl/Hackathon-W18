#ifndef ENV_H
#define ENV_H

#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <iostream>

//This file defines the object that
//represents terrain. These objects can be jumped onto,
//walked on, or climbed in the case of a ladder

enum Type {
	CLIMB,
	SOLID_STATIC,
	SOLID_MOVING,
	DEADLY	// you touch you die? probably won't implement
};

class Env : public sf::Drawable {
	public:
		Env() = delete;
		Env(std::istream& map);
		~Env() {}; //CAUSES MEMORY LEAK
		Type type;
		sf::RectangleShape hitbox;
		sf::Texture* tex;
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

		void update();
		void move();
		bool good();
	private:
		bool valid = true;
};

#endif
