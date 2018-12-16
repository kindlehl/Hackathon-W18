#include "../inc/environment.h"


/* Maps are stored in files in the following format:
 * relative/path/to/background/image
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

	hitbox.left = x;
	hitbox.top = y;
	hitbox.width = w;
	hitbox.height = h;
	//load environment texture from file
	if(!tex->loadFromFile(path)){
		std::cerr << "ERROR LOADING TEXTURE" << std::endl;
	}

	//set hitbox to use cowboy spritesheet
	spritebox.setTexture(*tex);

	spritebox.rotate(45);
	
	//spritebox.setPosition(hitbox.left + 1.41 * hitbox.width, hitbox.top * .71);
	spritebox.setPosition(hitbox.left + 1.41 * hitbox.width, hitbox.top - 1.41 * hitbox.height);
	//spritebox.setScale(.71 * screenSize.x / hitbox.left, 1.41 * screenSize.y / hitbox.top);
	spritebox.setScale(1.41 * hitbox.width / tex->getSize().x, 2 * 1.41 * hitbox.height / tex->getSize().y);

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

sf::IntRect Env::getHitbox() const {
	return hitbox;
}

bool Env::good() {
	return valid;
}

void Env::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(spritebox, states);

	
	//sf::RectangleShape hitboxOutline;
	//hitboxOutline.setSize(sf::Vector2f(hitbox.width, hitbox.height));
	//hitboxOutline.setPosition(hitbox.left, hitbox.top);
	//hitboxOutline.setFillColor(sf::Color(150, 50, 250));
	//target.draw(hitboxOutline);
}
