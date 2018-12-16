#include "../inc/connection.h"

//create new connection upon NEW packet type receipt
Connection::Connection(Packet p) : 
sessionID(p.sessionID)
{
	
}

Connection::Connection(ENetHost* c, ENetPeer* s, p_type t, Player p, std::string map) :
sessionID(1),
server(s),
client(c)
{
	switch(t) {
		case NEW:
			//sendNewSession(sessionID, map);
			sendNewSession(1, map);
			sendNewPlayer(p);
	}
}

void Connection::handleClient() {
	ENetEvent event;
	/* Wait up to 1000 milliseconds for an event. */
	while (enet_host_service (client, & event, 1000) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			printf ("A new client connected from %x:%u.\n", 
					event.peer -> address.host,
					event.peer -> address.port);
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
					event.packet -> dataLength,
					event.packet -> data,
					event.peer -> data,
					event.channelID);
			/* Clean up the packet now that we're done using it. */
			enet_packet_destroy (event.packet);
			
			break;
		   
		case ENET_EVENT_TYPE_DISCONNECT:
			printf ("%s disconnected.\n", event.peer -> data);
			/* Reset the peer's client information. */
			event.peer -> data = NULL;
		}
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
											  53, //send all 53 bytes
											  ENET_PACKET_FLAG_RELIABLE);
	if(!e_packet) {
		std::cerr << "Error creating packet" << std::endl;	
	}
	enet_peer_send(server, 0, e_packet);
	
	delete [] packet;
	enet_host_flush (client);

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
	if(!e_packet) {
		std::cerr << "Error creating packet" << std::endl;	
	}

	if(enet_peer_send(server, 0, e_packet) < 0){
		std::cerr << "Error sending packet!" << std::endl;
	}
	
	delete [] packet;
	enet_host_flush (client);

}

Connection::~Connection() {}

