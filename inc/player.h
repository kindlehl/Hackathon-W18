#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <vector>

#include "../inc/environment.h"
#include "../inc/bullet.h"

extern std::vector<Env> envs;

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
	Bullet* bullet = nullptr;


	sf::Sprite hitbox;
	sf::IntRect frame; //rectangle that determines the frame
					   //of the cowboy and his scale factor
	sf::Texture cowboy;

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
	void action(sf::Event);
	void shoot(State);
	void update();
	void move();
	void resetFrame(State playerState);
private:
	template <class T>
	bool collisionWouldHappen(sf::Vector2<T>& offset);
	int framecount = 0;
};

#endif
