#include "../inc/player.h"

Player::Player() {
	//player defaults to the walkingRight position
	currentState = previousState = WalkingLeft;

	//load cowboy texture from file
	cowboy.loadFromFile("img/player.png");

	//set hitbox to use cowboy spritesheet
	hitbox.setTexture(cowboy);
	//set frame to beginning of walking right animation
	resetFrame(WalkingLeft);
	
	//slap current animation frame onto the hitbox
	hitbox.setTextureRect(frame);
}

void Player::resetFrame(State playerState) {
	switch(playerState) {
		case WalkingRight:
			frame = {
				.left = 0,
				.top = 0,
				.width = 128,
				.height = 128
			};
			break;
		case WalkingLeft:
			frame = {
				.left = 0,
				.top = 7 * 128,
				.width = 128,
				.height = 128
			};
			break;
	}
}

void Player::update() {
	//if the player changes directions or jumps
	if(previousState == currentState) {
		//frame.left = (frame.left + 128) % (128 * 11);
		frame.left = (frame.left + 128) % (10 * 128);
	} else {
		//reset animation in spritesheet
		resetFrame(currentState);
	}

	//set frame
	hitbox.setTextureRect(frame);
	//set previousState
	previousState = currentState;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(hitbox, states);
}

void Player::action(int Key) {
	switch (Key) {
		case sf::Keyboard::A:
			hitbox.move(sf::Vector2f(-5,0));
			currentState = WalkingLeft;
			break;
		case sf::Keyboard::D:
			hitbox.move(sf::Vector2f(5,0));
			currentState = WalkingRight;
			break;
	}
}
