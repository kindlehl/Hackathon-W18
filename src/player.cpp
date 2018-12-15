#include "../inc/player.h"
#include <iostream>

Player::Player() {
	//player defaults to the walkingRight position
	currentState = previousState = Idle;
	
	hitbox.left = 100;
	hitbox.top = 100;
	hitbox.width = 40;
	hitbox.height = 40;

	//set sprite relative to its hitbox using black magic
	spritebox.setPosition(hitbox.left + 1.41 * hitbox.width, hitbox.top - 1.41 * hitbox.height - hitbox.height / 2);

	//load cowboy texture from file
	cowboy.loadFromFile("img/player.png");

	//set spritebox to use cowboy spritesheet
	spritebox.setTexture(cowboy);
	spritebox.rotate(45);
	//random values that work. second argument needs to be twice the first
	spritebox.setScale(.5, 1);
	//set frame to beginning of walking right animation
	resetFrame(WalkingRight);
	

	//slap current animation frame onto the spritebox
	spritebox.setTextureRect(frame);
}

void Player::resetFrame(State playerState) {
	switch(playerState) {
		case WalkingUp:
			frame = {
			frame.left = 0,
			frame.top = 6 * 128,
			frame.width = 128,
			frame.height = 128
		};
		break;
		case WalkingRight:
			frame = {
				frame.left = 0,
				frame.top = 4 * 128,
				frame.width = 128,
				frame.height = 128
			};
			break;
		case WalkingDown:
			frame = {
				frame.left = 0,
				frame.top = 2 * 128,
				frame.width = 128,
				frame.height = 128
			};
			break;
		case WalkingLeft:
			frame = {
				frame.left = 0,
				frame.top = 8 * 128,
				frame.width = 128,
				frame.height = 128
			};
			break;
		case WalkingUpRight:
			frame = {
				frame.left = 0,
				frame.top = 5 * 128,
				frame.width = 128,
				frame.height = 128
			};
			break;
		case WalkingDownRight:
			frame = {
				frame.left = 0,
				frame.top = 3 * 128,
				frame.width = 128,
				frame.height = 128
			};
			break;
		case WalkingUpLeft:
			frame = {
				frame.left = 0,
				frame.top = 7 * 128,
				frame.width = 128,
				frame.height = 128
			};
			break;
		case WalkingDownLeft:
			frame = {
				frame.left = 0,
				frame.top = 9 * 128,
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
	spritebox.setTextureRect(frame);

	this->move();

	//set previousState
	previousState = currentState;
	framecount++;
}

//This function compares checks for collisions between every tile/entity with the player
//if the player were to actually move. 
//Returns true if collision occured
//Modifies argument 'offset' to be the largest offest the player could move
//without intersecting with an environmental object
bool Player::processMovement(sf::Vector2f& offset) {
	
	bool collision = false;
	sf::IntRect collided_rect;
	for(const auto& e : envs) {

		sf::IntRect env_rect(e.hitbox.left, e.hitbox.top, e.hitbox.width, e.hitbox.height), overlap;
		//if a major collision occurs
		if(hitbox.intersects(sf::IntRect(env_rect), overlap) && overlap.width > 1 && overlap.height > 1) {
			std::cerr << "Collision detected, avoiding movements" <<  std::endl;
			collision = true;
			collided_rect = env_rect;
			break;
		}

	}

	if(collision) {
		switch(currentState) {
			case WalkingLeft:
				{
					//set offset to move player to right side of environment object
					int right_wall = collided_rect.left + collided_rect.width;
					offset.x = right_wall - hitbox.left;
					break;
				}
			case WalkingRight:
				{
					//set offset to move player to left side of environment object
					int left_wall = collided_rect.left;
					offset.x = left_wall - (hitbox.left + hitbox.width );
					break;
				}

		}
	}

	hitbox.left += offset.x;
	hitbox.top += offset.y;
	spritebox.move(offset);

	return collision;
}

void Player::move() {
	sf::Vector2f offset(0,0); //offset - determines how far player moves. Modified in procesMovement in case of collision
	switch (currentState) {
		case WalkingUp:
			offset.y = -6;
			break;
		case WalkingRight:
			offset.x = 6;
			break;
		case WalkingDown:
			offset.y = 6;
			break;
		case WalkingLeft:
			offset.x = -6;
			break;
		case WalkingUpRight:
			offset.x = 3;
			offset.y = -3;
			break;
		case WalkingDownRight:
			offset.x = 3;
			offset.y = 3;
			break;
		case WalkingUpLeft:
			offset.x = -3;
			offset.y = -3;
			break;
		case WalkingDownLeft:
			offset.x = -3;
			offset.y = 3;
			break;

	}

	processMovement(offset);

	if(currentState != Idle)
		std::cout << "The topleft corner of cowboy: " << this->spritebox.getPosition().x << ", " << this->spritebox.getPosition().y << std::endl;
}


void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(spritebox, states);

	sf::RectangleShape hitboxOutline;
	hitboxOutline.setSize(sf::Vector2f(hitbox.width, hitbox.height));
	hitboxOutline.setPosition(hitbox.left, hitbox.top);
	hitboxOutline.setFillColor(sf::Color(150, 50, 250));
	target.draw(hitboxOutline);

}

void Player::action(sf::Event e) {
	if(e.type == sf::Event::KeyPressed) { 
		switch (e.key.code) {
			case sf::Keyboard::W:
				//set state
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					currentState = WalkingUpLeft;
					break;
				} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
					currentState = WalkingUpRight;
					break;
				}
				currentState = WalkingUp;
				break;
			case sf::Keyboard::A:
				//set state
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
					currentState = WalkingUpLeft;
					break;
				} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					currentState = WalkingDownLeft;
					break;
				}
				currentState = WalkingLeft;
				break;
			case sf::Keyboard::S:
				//set state
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					currentState = WalkingDownLeft;
					break;
				} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
					currentState = WalkingDownRight;
					break;
				}
				currentState = WalkingDown;
				break;
			case sf::Keyboard::D:
				//set state
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
					currentState = WalkingUpRight;
					break;
				} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					currentState = WalkingDownRight;
					break;
				}
				currentState = WalkingRight;
				break;
		}
	} else if (e.type == sf::Event::KeyReleased) {
		switch (e.key.code) {
			case sf::Keyboard::W:
				//if key released is the key that represents the cowboys directions
				if (currentState == WalkingUpRight) {
					//set animation to frame 1, set state to direction of first key held down
					resetFrame(WalkingUpRight);
					previousState = currentState;
					currentState = WalkingRight;
				} else if (currentState == WalkingUpLeft) {
					//set animation to frame 1, set state to direction of first key held down
					resetFrame(WalkingUpLeft);
					previousState = currentState;
					currentState = WalkingLeft;
				} else if (currentState == WalkingUp) {
					//set animation to frame 1, set state to idle
					resetFrame(WalkingUp);
					previousState = currentState;
					currentState = Idle;
				}
			break;
			case sf::Keyboard::A:
				//if key released is the key that represents the cowboys directions
				if (currentState == WalkingUpLeft) {
					//set animation to frame 1, set state to direction of first key held down
					resetFrame(WalkingUpLeft);
					previousState = currentState;
					currentState = WalkingUp;
				} else if (currentState == WalkingDownLeft) {
					//set animation to frame 1, set state to direction of first key held down
					resetFrame(WalkingDownLeft);
					previousState = currentState;
					currentState = WalkingDown;
				} else if (currentState == WalkingLeft) {
					//set animation to frame 1, set state to idle
					resetFrame(WalkingLeft);
					previousState = currentState;
					currentState = Idle;
				}
				break;
			case sf::Keyboard::S:
				//if key released is the key that represents the cowboys directions
				if (currentState == WalkingDownRight) {
					//set animation to frame 1, set state to direction of first key held down
					resetFrame(WalkingDownRight);
					previousState = currentState;
					currentState = WalkingRight;
				} else if (currentState == WalkingDownLeft) {
					//set animation to frame 1, set state to direction of first key held down
					resetFrame(WalkingDownLeft);
					previousState = currentState;
					currentState = WalkingLeft;
				} else if (currentState == WalkingDown) {
					//set animation to frame 1, set state to idle
					resetFrame(WalkingDown);
					previousState = currentState;
					currentState = Idle;
				}
				break;
			case sf::Keyboard::D:
				//if key released is the key that represents the cowboys directions
				if (currentState == WalkingUpRight) {
					//set animation to frame 1, set state to direction of first key held down
					resetFrame(WalkingUpRight);
					previousState = currentState;
					currentState = WalkingUp;
				} else if (currentState == WalkingDownRight) {
					//set animation to frame 1, set state to direction of first key held down
					resetFrame(WalkingDownRight);
					previousState = currentState;
					currentState = WalkingDown;
				} else if (currentState == WalkingRight) {
					//set animation to frame 1, set state to idle
					resetFrame(WalkingRight);
					previousState = currentState;
					currentState = Idle;
				}
				break;
		}
	}
}
