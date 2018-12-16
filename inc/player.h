#ifndef PLAYER_H
#define PLAYER_H

#define MAX_FRAMES_SAVED 20

#include <SFML/Graphics.hpp>
#include <enet/enet.h>
#include <vector>
#include <cstring>
#include <ctime>
#include <random>

#include "../inc/environment.h"
#include "../inc/connection.h"
#include "../inc/hitbox.h"

extern std::vector<Hitbox*> envs;
extern sf::Vector2i screenSize;

enum State {
	Idle,
	WalkingUp,
	WalkingRight,
	WalkingDown,
	WalkingLeft,
	WalkingUpRight,
	WalkingDownRight,
	WalkingUpLeft,
	WalkingDownLeft,
	ShootingUp,
	ShootingRight,
	ShootingDown,
	ShootingLeft,
	ShootingUpRight,
	ShootingDownRight,
	ShootingUpLeft,
	ShootingDownLeft
};

struct FreezeFrame {
	sf::Sprite spritebox;
	sf::IntRect hitbox, frame;
};

class Player : public Hitbox {
public:
	Player();
	Player(char*);
	~Player() {};
	
	State currentState, previousState, facingState;

	void printDebug();

	sf::IntRect hitbox, startingHitbox;
	std::vector<FreezeFrame> freeze_frames;

	sf::Sprite spritebox;
	sf::IntRect frame; //rectangle that determines the frame
					   //of the cowboy and his scale factor
	sf::Texture cowboy;


	void draw(sf::RenderTarget&, sf::RenderStates) const;
	sf::IntRect getHitbox() const;

	bool rewinding = false;

	void action(sf::Event);
	void update();
	void updateFromBuffer(char*);
	void move();
	void sendUpdate(ENetPeer*, ENetHost*);
	void resetFrame(State playerState);
private:
	bool processMovement(sf::Vector2i& offset);
	int framecount = 0;
};

#endif
