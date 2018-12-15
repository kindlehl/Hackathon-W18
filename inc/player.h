#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <vector>

#include "../inc/environment.h"

extern std::vector<Env> envs;

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

class Player : public sf::Drawable {
public:
	Player();
	
	State currentState, previousState;


	sf::IntRect hitbox;
	sf::Sprite spritebox;
	sf::IntRect frame; //rectangle that determines the frame
					   //of the cowboy and his scale factor
	sf::Texture cowboy;

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
	void action(sf::Event);
	void update();
	void move();
	void resetFrame(State playerState);
private:
	bool processMovement(sf::Vector2i& offset);
	int framecount = 0;
};

#endif
