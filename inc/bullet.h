#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../inc/hitbox.h"
#include "../inc/connection.h"

class Bullet : public Hitbox
{
public:

	Bullet(sf::IntRect, sf::Vector2i, ENetHost* client, ENetPeer* server);
	Bullet(char*);//load from network
	Bullet(int, int, int, int);
	sf::IntRect hitbox;
	void update();
	void sendCreateSignal(ENetHost*, ENetPeer*); //send over network
	sf::IntRect getHitbox() const;
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
	//virtual void draw(sf::RenderWindow &window);
	

	sf::RectangleShape bullet;
	int xVel;
	int yVel;

};

#endif //BULLET_H
