#include "../inc/rain.h"

Rain::Rain() {
	font.loadFromFile("fonts/Hack-Regular.ttf");
	srand(time(NULL));
	for(int i = 0; i < 10 * WIDTH; i++) {
		std::string hax;

		for(int j = 0; j < 100; j++){
			hax += (char) rand() % 10 + 48;
		}
	
		sf::Text raindrop;
		raindrop.setString(hax);
		raindrop.setFont(font);
		raindrop.setCharacterSize(8 * WIDTH);
		raindrop.setRotation(135);
		raindrop.setPosition(i * WIDTH * 6 - 600, 0);
		raindrop.setPosition(raindrop.getPosition().x, raindrop.getPosition().x * .78 - 2800);
		raindrop.setFillColor(sf::Color(0, 59, 0));//actual matrix green

		codelines.push_back(raindrop);
	}
}

void Rain::update() {

	for(auto& r : codelines) {

		std::string s = r.getString();
		int changed = rand() % 20;

		for(int i = 0; i < changed; i++){
			s[rand() % s.length()] = rand() % 10 + 48;
		}

		r.setString(s);
	}

}


void Rain::draw(sf::RenderTarget& t, sf::RenderStates s) const {
	for(auto& r : codelines) {
		t.draw(r);
	}
}
