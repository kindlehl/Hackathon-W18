#include "../inc/environment.h"


/* Maps are stored in files in the following format:
 * num_tiles
 * x y w h
 * relative/path/to/image/
 * type
 * x y w h
 * relative/path/to/image/
 * type
 * x y w h
 * relative/path/to/image/
 * type
*/


Env::Env(std::istream& map) {
	int x, y, w, h;
	std::string path, t;

	tex = new sf::Texture;

	map >> x >> y >> w >> h >> path >> t;
	std::cout << "opening texture at " << path << std::endl;

	//load environment texture from file
	tex->loadFromFile(path);

	//set hitbox to use cowboy spritesheet
	hitbox.setTexture(tex);
	
	hitbox.setSize(sf::Vector2f(w, h));
	hitbox.setPosition(sf::Vector2f(x, y));

	if(t == "CLIMB") {
		type = CLIMB;
	} else if (t == "SOLID_STATIC"){
		type = SOLID_STATIC;
	} else if (t == "SOLID_MOVING"){
		type = SOLID_MOVING;
	} else if (t == "DEADLY"){
		type = DEADLY;
	}else {
		std::cerr << "INVALID ENVIRONMENT TYPE -> " << t << std::endl;
		valid = false;
	}

}

bool Env::good() {
	return valid;
}

void Env::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(hitbox, states);
}
