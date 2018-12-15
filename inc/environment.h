#ifndef ENV_H
#define ENV_H

#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <iostream>

#include "../inc/hitbox.h"


//This file defines the object that
//represents terrain. These objects can be jumped onto,
//walked on, or climbed in the case of a ladder

enum Type {
	CLIMB,
	SOLID_STATIC,
	SOLID_MOVING,
	DEADLY	// you touch you die? probably won't implement
};

class Env : public Hitbox {
	public:
		Env() = delete;
		Env(std::istream& map);
		~Env() {}; //CAUSES MEMORY LEAK
		Type type;
		sf::Sprite spritebox;
		sf::IntRect hitbox;
		sf::Texture* tex;
		void draw(sf::RenderTarget&, sf::RenderStates) const;
		sf::IntRect getHitbox() const;

		void update();
		void move();
		bool good();
	private:
		bool valid = true;
};

#endif
