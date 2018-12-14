#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

enum State {
	Idle,
	WalkingLeft,
	WalkingRight,
	ShootingLeft,
	ShootingRight,
	JumpingRight,
	JumpingLeft
};

class Player : public sf::Drawable {
public:
	Player();
	
	State currentState, previousState;


	sf::Sprite hitbox;
	sf::IntRect frame;
	sf::Texture cowboy;
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
	void action(sf::Event);
	void update();
	void resetFrame(State playerState);
private:
	int framecount = 0;
};

#endif
