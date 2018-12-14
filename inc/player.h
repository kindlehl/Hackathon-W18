#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

enum State {
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
	void action(int KeyEnumValue);
	void update();
	void resetFrame(State playerState);
};

#endif
