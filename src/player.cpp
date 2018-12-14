#include "../inc/player.h"

Player::Player() {
	static_stick.loadFromFile("img/sticky.png");
	hitbox.setTexture(static_stick);
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
			hitbox.move(sf::Vector2f(0,-5));
			break;
		case sf::Keyboard::S:
			hitbox.move(sf::Vector2f(0,5));
			break;
	}
}
