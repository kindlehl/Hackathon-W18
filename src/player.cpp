#include "../inc/player.h"

Player::Player() {
	sf::Texture stick_tex;
	stick_tex.loadFromFile("img/sticky.png");
	hitbox = sf::Sprite(static_stick, sf::Rect<int>(400,400,50,100));
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(hitbox, states);
}

void Player::action(int Key) {
	switch (Key) {
		case sf::Keyboard::A:
			hitbox.move(sf::Vector2f(-5,0));
			break;
		case sf::Keyboard::D:
			hitbox.move(sf::Vector2f(5,0));
			break;
		case sf::Keyboard::W:
			hitbox.move(sf::Vector2f(0,5));
			break;
		case sf::Keyboard::S:
			hitbox.move(sf::Vector2f(0,-5));
			break;
	}
}
