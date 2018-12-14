#include "../inc/player.h"
#include <iostream>

Player::Player() {
	//player defaults to the walkingRight position
	currentState = previousState = Idle;

	//load cowboy texture from file
	cowboy.loadFromFile("img/player.png");

	//set hitbox to use cowboy spritesheet
	hitbox.setTexture(cowboy);
	//set frame to beginning of walking right animation
	resetFrame(WalkingRight);
	
	//slap current animation frame onto the hitbox
	hitbox.setTextureRect(frame);
}

void Player::resetFrame(State playerState) {
	switch(playerState) {
		case WalkingRight:
			frame = {
				frame.left = 0,
				frame.top = 0,
				frame.width = 128,
				frame.height = 128
			};
			break;
		case WalkingLeft:
			frame = {
				frame.left = 0,
				frame.top = 1 * 128,
				frame.width = 128,
				frame.height = 128
			};
			break;
	}
}

void Player::update() {
	//if the player changes directions or jumps
	if(previousState == currentState && framecount >= 10 && currentState != Idle) {
		//frame.left = (frame.left + 128) % (128 * 11);
		frame.left += 128;

		if(frame.left > 1280) {
			resetFrame(currentState);
		}

		framecount = 0;
	} else if (previousState != currentState && currentState != Idle) {
		//reset animation in spritesheet
		resetFrame(currentState);
	}

	//set frame
	hitbox.setTextureRect(frame);

	//set previousState
	previousState = currentState;
	framecount++;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(hitbox, states);
}

void Player::action(sf::Event e) {
	if(e.type == sf::Event::KeyPressed) { 
		switch (e.key.code) {
			case sf::Keyboard::A:
				hitbox.move(sf::Vector2f(-5,0));
				//set state
				currentState = WalkingLeft;
				break;
			case sf::Keyboard::D:
				hitbox.move(sf::Vector2f(5,0));
				//set state
				currentState = WalkingRight;
				break;
		}
	} else if (e.type == sf::Event::KeyReleased) {
		switch (e.key.code) {
			case sf::Keyboard::A:
				//if key released is the key that represents the cowboys directions
				if(currentState == WalkingLeft){
					//set animation to frame 1, set state to idle
					resetFrame(WalkingLeft);
					previousState = currentState;
					currentState = Idle;
				}
				break;
			case sf::Keyboard::D:
				//if key released is the key that represents the cowboys directions
				if(currentState == WalkingRight){
					//set animation to frame 1, set state to idle
					resetFrame(WalkingRight);
					previousState = currentState;
					currentState = Idle;
				}
				break;
		}
	}
}
