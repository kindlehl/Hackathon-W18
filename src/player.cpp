#include "../inc/player.h"
#include <iostream>

Player::Player() {
	//player defaults to the walkingRight position
	currentState = previousState = Idle;

	hitbox.setPosition(0,200);

	//load cowboy texture from file
	cowboy.loadFromFile("img/player.png");

	//set hitbox to use cowboy spritesheet
	hitbox.setTexture(cowboy);
	hitbox.rotate(45);
	//set frame to beginning of walking right animation
	resetFrame(WalkingRight);
	

	//slap current animation frame onto the hitbox
	hitbox.setTextureRect(frame);

	hitbox.setPosition(0,100);
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

	this->move();

	//set frame
	hitbox.setTextureRect(frame);

	//set previousState
	previousState = currentState;
	framecount++;
}

//This function compares checks for collisions between every tile/entity with the player
//if the player were to actually move. 
//Returns true if collision occured
//Modifies argument 'offset' to be the largest offest the player could move
//without intersecting with an environmental object
template <class T>
bool Player::collisionWouldHappen(sf::Vector2<T>& offset) {
	
	//player rect
	sf::Rect<T> player_rect(hitbox.getPosition() + offset, sf::Vector2f(frame.width, frame.height));
	bool collision = false;
	sf::Rect<T> collided_rect;
	for(const auto& e : envs) {

		sf::Rect<T> env_rect(e.hitbox.getPosition(), e.hitbox.getSize());
		if(player_rect.intersects(env_rect)) {
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
					offset.x = right_wall - hitbox.getPosition().x;
					break;
				}
			case WalkingRight:
				{
					//set offset to move player to left side of environment object
					int left_wall = collided_rect.left;
					offset.x = left_wall - (hitbox.getPosition().x + frame.width);
					break;
				}

		}
	}

	return collision;
}

void Player::move() {
	sf::Vector2f offset(0,0); //no offset initially
	switch (currentState) {
		case WalkingUp:
			offset.y = -6;
			collisionWouldHappen(offset);
			hitbox.move(offset);
			break;
		case WalkingRight:
			offset.x = 6;
			collisionWouldHappen(offset);
			hitbox.move(offset);
			break;
		case WalkingDown:
			offset.y = 6;
			collisionWouldHappen(offset);
			hitbox.move(offset);
			break;
		case WalkingLeft:
			offset.x = -6;
			collisionWouldHappen(offset);
			hitbox.move(offset);
			break;
		case WalkingUpRight:
			offset.x = 3;
			offset.y = -3;
			collisionWouldHappen(offset);
			hitbox.move(offset);
			break;
		case WalkingDownRight:
			offset.x = 3;
			offset.y = 3;
			collisionWouldHappen(offset);
			hitbox.move(offset);
			break;
		case WalkingUpLeft:
			offset.x = -3;
			offset.y = -3;
			collisionWouldHappen(offset);
			hitbox.move(offset);
			break;
		case WalkingDownLeft:
			offset.x = -3;
			offset.y = 3;
			collisionWouldHappen(offset);
			hitbox.move(offset);
			break;

	}
}


void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(hitbox, states);
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
