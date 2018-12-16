#include "../inc/connection.h"

//create new connection upon NEW packet type receipt
Connection::Connection(Packet p) : 
sessionID(p.sessionID)
{
	
}
 
Connection::Connection(ENetHost* client, ENetPeer* server, p_type t, Player p, std::string map) :
sessionID(1)
{
	switch(t) {
		case NEW:
			//sendNewSession(sessionID, map);
			sendNewSession(1, map);
			sendNewPlayer(p);
	}
}

void Connection::sendNewPlayer(Player p) {
	char* packet = new char[200];	
	
	int create = CREATE;
	int player = PLAYER;

	memcpy(packet, &sessionID, 4);
	memcpy(packet + 4, &create, 4);
	memcpy(packet + 8, &player, 4);
	
	//send hitbox
	auto hitbox = p.getHitbox();
	memcpy(packet + 12, &hitbox.left, 4);
	memcpy(packet + 16, &hitbox.top, 4);
	memcpy(packet + 20, &hitbox.width, 4);
	memcpy(packet + 24, &hitbox.height, 4);

	//send animation frame
	memcpy(packet + 28, &p.frame.left, 4);
	memcpy(packet + 32, &p.frame.top, 4);
	memcpy(packet + 36, &p.frame.width, 4);
	memcpy(packet + 40, &p.frame.height, 4);

	//send state
	memcpy(packet + 44, &p.currentState, 4);
	memcpy(packet + 48, &p.previousState, 4);
	
	//null terminator
	packet[52] = '\0';

	ENetPacket* e_packet = enet_packet_create(packet,
											  strlen(packet) + 1,
											  ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(server, 0, e_packet);
	
	delete [] packet;

}

void Connection::sendNewSession(int sID, std::string map) {
	char* packet = new char[200];

	int val = NEW;

	memcpy(packet, &sID, 4);
	memcpy(packet + 4, &val, 4);
	memcpy(packet + 8, map.c_str(), map.length() + 1);

	ENetPacket* e_packet = enet_packet_create(packet,
											  strlen(packet) + 1,
											  ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(server, 0, e_packet);
	
	delete [] packet;
}

Connection::~Connection() {}

