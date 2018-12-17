#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../inc/hitbox.h"
#include "../inc/connection.h"

//bullet containers and bools
enum direction {
	None,
	Up, 
	Right,
	Down,
	Left,
	UpRight,
	DownRight,
	UpLeft,
	DownLeft
};

class Bullet : public Hitbox
{
public:

	Bullet(sf::IntRect, sf::Vector2i, sf::Vector2i, ENetHost* client, ENetPeer* server);

	Bullet(char*);//load from network
	Bullet(int, int, int, int);
	sf::IntRect hitbox;
	void update();
	void sendCreateSignal(ENetHost*, ENetPeer*); //send over network
	Hitbox* checkCollision(std::vector<Hitbox*>& envs);
	sf::IntRect getHitbox() const;
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
	//virtual void draw(sf::RenderWindow &window);
	

	sf::RectangleShape bullet;
	int xVel;
	int yVel;

};

#endif //BULLET_H
