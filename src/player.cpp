#include "../inc/player.h"
#include <iostream>

Player::Player() {
	//player defaults to the walkingRight position
	currentState = previousState = Idle;
	
	hitbox.width = 40;
	hitbox.height = 40;

	bool tryagain = false;

	//set unique coordinates for hitbox
	do {
		hitbox.left = rand() % (screenSize.x - hitbox.width);
		hitbox.top = rand() % (screenSize.y - hitbox.height);

		for(auto& e : envs) {
			if(hitbox.intersects(e->getHitbox())){
				tryagain = true;
				continue;
			}
		}
		
		tryagain = false;

	} while(tryagain);

	startingHitbox = hitbox;

	//set sprite relative to its hitbox using black magic
	spritebox.setPosition(hitbox.left + 1.61 * hitbox.width, hitbox.top - 1.41 * hitbox.height - hitbox.height / 2);

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

sf::IntRect Player::getHitbox() const {
	return hitbox;
}

////function to update your character to the enemy
void Player::sendUpdate(ENetPeer* server, ENetHost* client) {
	//store 2 pointers to buffer
	char* data;
	char* base = data = new char[200];
	int type = PLAYER;
	shoveData(data, &type, 4);

	//put hitbox into buffer
	//This function advances the data
	//pointer each time its called.
	shoveData(data, &hitbox.left, 4);
	shoveData(data, &hitbox.top, 4);
	shoveData(data, &hitbox.width, 4);
	shoveData(data, &hitbox.height, 4);
	
	//put spritebox position into buffer
	auto coords = spritebox.getPosition();
	shoveData(data, &coords.x, 4);
	shoveData(data, &coords.y, 4);
	
	//put states into buffer
	shoveData(data, &currentState, 4);
	shoveData(data, &previousState, 4);
	
	//put animation frame into buffer
	shoveData(data, &frame.left, 4);
	shoveData(data, &frame.top, 4);
	shoveData(data, &frame.width, 4);
	shoveData(data, &frame.height, 4);

	*data = '\0';

	//create packet
	ENetPacket* packet = enet_packet_create(base, data - base, ENET_PACKET_FLAG_RELIABLE);

	enet_peer_send(server, 0, packet);
	enet_host_flush(client);
}

//buf should have NOTHING but player data, not even the header byte
void Player::updateFromBuffer(char* buf) {
	//put hitbox into Datafer
	//This function advances the Data
	//pointer each time its called.
	pullData(buf, &hitbox.left, 4);
	pullData(buf, &hitbox.top, 4);
	pullData(buf, &hitbox.width, 4);
	pullData(buf, &hitbox.height, 4);

	//extract sprite position
	sf::Vector2f pos;
	pullData(buf, &pos.x, 4);
	pullData(buf, &pos.y, 4);

	//set sprite position
	spritebox.setPosition(pos);
	
	//put states into Datafer
	pullData(buf, &currentState, 4);
	pullData(buf, &previousState, 4);
	
	//reset animation frame
	pullData(buf, &frame.left, 4);
	pullData(buf, &frame.top, 4);
	pullData(buf, &frame.width, 4);
	pullData(buf, &frame.height, 4);

}

////function to update enemy player's character from server
//void Player::Receive() {
	
//}

void Player::printDebug() {
	std::cerr << "Player x y w h: " << hitbox.left << " " 
	<< hitbox.top << " " << hitbox.width << " " << hitbox.height << std::endl;

	switch(currentState) {
		case Idle:
			std::cerr << "State: Idle" << std::endl;
			break;
		case WalkingDown:
			std::cerr << "State: WalkingDown" << std::endl;
			break;
		case WalkingUp:
			std::cerr << "State: WalkingUp" << std::endl;
			break;
		case WalkingRight:
			std::cerr << "State: WalkingRight" << std::endl;
			break;
		case WalkingLeft:
			std::cerr << "State: WalkingLeft" << std::endl;
			break;
	}
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
		case ShootingUp:
			frame = {
				frame.left = 11 * 128,
				frame.top = 6 * 128,
				frame.width = 128,
				frame.height = 128
			};
			break;
		case ShootingRight:
			frame = {
				frame.left = 11 * 128,
				frame.top = 4 * 128,
				frame.width = 128,
				frame.height = 128
			};
			break;
		case ShootingDown:
			frame = {
				frame.left = 11 * 128,
				frame.top = 2 * 128,
				frame.width = 128,
				frame.height = 128
			};
			break;
		case ShootingLeft:
			frame = {
				frame.left = 11 * 128,
				frame.top = 8 * 128,
				frame.width = 128,
				frame.height = 128
			};
			break;
		case ShootingUpRight:
			frame = {
				frame.left = 11 * 128,
				frame.top = 5 * 128,
				frame.width = 128,
				frame.height = 128
			};
			break;
		case ShootingDownRight:
			frame = {
				frame.left = 11 * 128,
				frame.top = 3 * 128,
				frame.width = 128,
				frame.height = 128
			};
			break;
		case ShootingUpLeft:
			frame = {
				frame.left = 11 * 128,
				frame.top = 7 * 128,
				frame.width = 128,
				frame.height = 128
			};
			break;
		case ShootingDownLeft:
			frame = {
				frame.left = 11 * 128,
				frame.top = 9 * 128,
				frame.width = 128,
				frame.height = 128
			};
			break;
	}
}

void Player::update() {
	//If the player is rewinding, then by golly rewind!
	
	if(rewinding) {

		if(freeze_frames.size() == 0) {
			std::cout << "Sorry, no freeze frames for you!" << std::endl;
			rewinding = false;
		} else{
			std::cout << "REWINDING IN ACTION" << std::endl;
			FreezeFrame data = *(freeze_frames.end()-1);
			freeze_frames.pop_back();
			spritebox = data.spritebox;
			hitbox = data.hitbox;
			frame = data.frame;
		}
		return;
	}


	//if the player changes directions or jumps
	if(previousState == currentState && framecount >= 10 && currentState != Idle) {

		frame.left += 128;

		if(frame.left > 1280) {
			resetFrame(currentState);
			//store player state for rewind
		}

		framecount = 0;
	} else if (previousState != currentState && currentState != Idle) {
		//reset animation in spritesheet
		resetFrame(currentState);
	} 
	if (framecount == 0) {

		//if storing max allowed frames
		if (freeze_frames.size() > MAX_FRAMES_SAVED){
			//erase oldest freeze frame
			freeze_frames.erase(freeze_frames.begin(), freeze_frames.begin() + 1);
		} 
		//store new frame
		FreezeFrame newFrame;
		newFrame.hitbox = hitbox;
		newFrame.spritebox = spritebox;
		newFrame.frame = frame;

		freeze_frames.push_back(newFrame);
		
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
bool Player::processMovement(sf::Vector2i& offset) {
	
	bool collision = false;
	sf::IntRect overlap;

	//move hitbox
	hitbox.left += offset.x;
	hitbox.top += offset.y;
	spritebox.move(offset.x, offset.y);

	for(const auto& e : envs) {

		sf::IntRect env_rect = e->getHitbox();
		
		//skip if reached your own hitbox
		if(hitbox == env_rect)
			continue;

		//if a collision occurs with new hitbox position
		if(hitbox.intersects(env_rect, overlap)) {
			std::cerr << "Collision detected, avoiding movements" <<  std::endl;
			//set collision to true
			collision = true;
			break;
		}

	}

	//if a collision occured
	if(collision) {
		switch(currentState) {
			case WalkingLeft:
				{
					//move hitbox+sprite by the amount the hitboxes overlap
					offset.x = overlap.width;
					break;
				}
			case WalkingRight:
				{
					//move hitbox+sprite by the amount the hitboxes overlap
					offset.x = -overlap.width;
					break;
				}
			case WalkingUp:
				{
					//move hitbox+sprite by the amount the hitboxes overlap
					offset.y = overlap.height;
					break;
				}
			case WalkingDown:
				{
					//move hitbox+sprite by the amount the hitboxes overlap
					offset.y = -overlap.height;
					break;
				}
			case WalkingDownRight:
				{
					//move hitbox+sprite by the amount the hitboxes overlap - 
					//priorities the dimension that is most overlapped
					if(overlap.height >= overlap.width)
						offset.x = -overlap.width;
					else
						offset.y = -overlap.height;
					break;
				}
			case WalkingDownLeft:
				{
					//move hitbox+sprite by the amount the hitboxes overlap - 
					//priorities the dimension that is most overlapped
					if(overlap.height >= overlap.width)
						offset.x = overlap.width;
					else
						offset.y = -overlap.height;
					break;
				}
			case WalkingUpRight:
				{
					//move hitbox+sprite by the amount the hitboxes overlap - 
					//priorities the dimension that is most overlapped
					if(overlap.height >= overlap.width)
						offset.x = -overlap.width;
					else
						offset.y = overlap.height;
					break;
				}
			case WalkingUpLeft:
				{
					//move hitbox+sprite by the amount the hitboxes overlap - 
					//priorities the dimension that is most overlapped
					if(overlap.height >= overlap.width)
						offset.x = overlap.width;
					else
						offset.y = overlap.height;
					break;
				}


		}
		hitbox.left += offset.x;
		hitbox.top += offset.y;
		spritebox.move(offset.x, offset.y);
	}

	return collision;
}

void Player::move() {
	sf::Vector2i offset(0,0); //offset - determines how far player moves. Modified in procesMovement in case of collision
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
		default:
			break;
	}

	processMovement(offset);

	if(currentState != Idle)
		std::cout << "The topleft corner of cowboy: " << this->spritebox.getPosition().x << ", " << this->spritebox.getPosition().y << std::endl;
}


void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(spritebox, states);

	//sf::RectangleShape hitboxOutline;
	//hitboxOutline.setSize(sf::Vector2f(hitbox.width, hitbox.height));
	//hitboxOutline.setPosition(hitbox.left, hitbox.top);
	//hitboxOutline.setFillColor(sf::Color(150, 50, 250));
	//target.draw(hitboxOutline);

}

void Player::action(sf::Event e) {

	if(e.type == sf::Event::KeyPressed) { 
		switch (e.key.code) {
			case sf::Keyboard::Space:
				if (currentState == Idle) {
					if (facingState == WalkingUp) {
						currentState = ShootingUp;
					} else if (facingState == WalkingRight) {
						currentState = ShootingRight;
					} else if (facingState == WalkingDown) {
						currentState = ShootingDown;
					} else if (facingState == WalkingLeft) {
						currentState = ShootingLeft;
					}
				}
				break;
			case sf::Keyboard::W:
				//set state
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
						currentState = ShootingUpLeft;
					} else {
						currentState = WalkingUpLeft;
					}
					break;
				} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
						currentState = ShootingUpRight;
					} else {
						currentState = WalkingUpRight;
					}
					break;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
					currentState = ShootingUp;
				} else {
					currentState = WalkingUp;
				}
				break;
			case sf::Keyboard::A:
				//set state
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
						currentState = ShootingUpLeft;
					} else {
						currentState = WalkingUpLeft;
					}
					break;
				} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
						currentState = ShootingDownLeft;
					} else {
						currentState = WalkingDownLeft;
					}
					break;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
					currentState = ShootingLeft;
				} else {
					currentState = WalkingLeft;
				}
				break;
			case sf::Keyboard::S:
				//set state
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
						currentState = ShootingDownLeft;
					} else {
						currentState = WalkingDownLeft;
					}
					break;
				} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
						currentState = ShootingDownRight;
					} else {
						currentState = WalkingDownRight;
					}
					break;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
					currentState = ShootingDown;
				} else {
					currentState = WalkingDown;
				}
				break;
			case sf::Keyboard::D:
				//set state
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
						currentState = ShootingUpRight;
					} else {
						currentState = WalkingUpRight;
					}
					break;
				} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
						currentState = ShootingDownRight;
					} else {
						currentState = WalkingDownRight;
					}
					break;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
					currentState = ShootingRight;
				} else {
					currentState = WalkingRight;
				}
				break;
		}
	} else if (e.type == sf::Event::KeyReleased) {
		switch (e.key.code) {
			case sf::Keyboard::W:
				//if key released is the key that represents the cowboys directions
				if (currentState == ShootingUpRight) {
					currentState = ShootingRight;
				} else if (currentState == ShootingUpLeft) {
					currentState = ShootingLeft;
				} else if (currentState == WalkingUpRight) {
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
					facingState = previousState;
					currentState = Idle;
				}
			break;
			case sf::Keyboard::A:
				//if key released is the key that represents the cowboys directions
				if (currentState == ShootingUpLeft) {
					currentState = ShootingUp;
				} else if (currentState == ShootingDownLeft) {
					currentState = ShootingDown;
				} else if (currentState == WalkingUpLeft) {
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
					facingState = previousState;
					currentState = Idle;
				}
				break;
			case sf::Keyboard::S:
				//if key released is the key that represents the cowboys directions
				if (currentState == ShootingDownRight) {
					currentState = ShootingRight;
				} else if (currentState == ShootingDownLeft) {
					currentState = ShootingLeft;
				} else if (currentState == WalkingDownRight) {
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
					facingState = previousState;
					currentState = Idle;
				}
				break;
			case sf::Keyboard::D:
				//if key released is the key that represents the cowboys directions
				if (currentState == ShootingUpRight) {
					currentState = ShootingUp;
				} else if (currentState == ShootingDownRight) {
					currentState = ShootingDown;
				} else if (currentState == WalkingUpRight) {
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
					facingState = previousState;
					currentState = Idle;
				}
				break;
			case sf::Keyboard::Space:
				//space released, indicates shooting
				if (currentState == ShootingUpRight) {
					resetFrame(WalkingUpRight);
					currentState = WalkingUpRight;
				} else if (currentState == ShootingDownRight) {
					resetFrame(WalkingDownRight);
					currentState = WalkingDownRight;
				} else if (currentState == ShootingDownLeft) {
					resetFrame(WalkingDownLeft);
					currentState = WalkingDownLeft;
				} else if (currentState == ShootingUpLeft) {
					resetFrame(ShootingUpLeft);
					currentState = WalkingUpLeft;
				} else if (currentState == ShootingUp) {
					resetFrame(WalkingUp);
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
						currentState = WalkingUp;
					} else {
						facingState = WalkingUp;
						currentState = Idle;
					}
				} else if (currentState == ShootingRight) {
					resetFrame(WalkingRight);
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
						currentState = WalkingRight;
					} else {
						facingState = WalkingRight;
						currentState = Idle;
					}
				} else if (currentState == ShootingDown) {
					resetFrame(WalkingDown);
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
						currentState = WalkingDown;
					} else {
						facingState = WalkingDown;
						currentState = Idle;
					}
				} else if (currentState == ShootingLeft) {
					resetFrame(WalkingLeft);
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
						currentState = WalkingLeft;
					} else {
						facingState = WalkingLeft;
						currentState = Idle;
					}
				}
				break;
		}
	}
}
