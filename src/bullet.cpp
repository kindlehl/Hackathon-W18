#include "../inc/bullet.h"

// ctor takes in the 
Bullet::Bullet(sf::IntRect, sf::Vector2i, ENetHost* client, ENetPeer* server) {
	hitbox.left = rectIn.left + rectIn.width/2;
	hitbox.top = rectIn.top - rectIn.height/2;
	hitbox.width = rectIn.width/4;
	hitbox.height = rectIn.height/4;
	bullet.setFillColor(sf::Color::Red);
	bullet.setPosition(hitbox.left, hitbox.top);
	bullet.setSize(sf::Vector2f(hitbox.width, hitbox.height));

	xVel = velocity.x;
	yVel = velocity.y;

	sendCreateSignal(client, server);
}

Bullet::Bullet(char* packet) {
	pullData(packet, &bulletx, 4);
	pullData(packet, &bullety, 4);
	pullData(packet, &xVel, 4);
	pullData(packet, &yVel, 4);
}

Bullet::sendCreateSignal(ENetHost* client, ENetPeer* server) {
	char* data;
	char* base = data = new char[200];

	int type = BULLET;
	shoveData(data, &type, 4);

	//put hitbox into buffer
	//This function advances the data
	//pointer each time its called.
	int bulletx = bullet.getPosition().x;
	int bullety = bullet.getPosition().y;
	shoveData(data, &bulletx, 4);
	shoveData(data, &bullety, 4);
	shoveData(data, &xVel, 4);
	shoveData(data, &yVel, 4);
	*data = '\0';

	//create packet
	ENetPacket* packet = enet_packet_create(base, data - base, ENET_PACKET_FLAG_RELIABLE);

	enet_peer_send(server, 0, packet);
	enet_host_flush(client);
}

Bullet::Bullet(int leftIn, int topIn, int widthIn, int heightIn)
{
	
	hitbox.left = leftIn;
	hitbox.top = topIn;
	hitbox.width = widthIn;
	hitbox.height = heightIn;
}

void Bullet::update()
{
	hitbox.left += xVel;
	hitbox.top += yVel;
	bullet.move(xVel, yVel);
}


sf::IntRect Bullet::getHitbox() const
{
	return hitbox;
}
void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(bullet);
}


