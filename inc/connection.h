#ifndef CONNECTION_H
#define CONNECTION_H

#include "../inc/player.h"

#include <utility>
#include <string>
#include <enet/enet.h>

enum p_type {
	NEW, // start new session/game
	CONNECT, //connect to a session
	DISCONNECT, //disconnect from session
	UPDATE,	//game/object updates
	CREATE	//game/object creation
};

enum create_type {
	PLAYER,
	BULLET
};

struct Packet {
	int sessionID;
	p_type type;
	char* payload;
};

class Connection {
public:
	Connection(Packet);
	Connection(ENetHost* client, ENetPeer* server, p_type t, Player p, std::string map);
	~Connection();
	ENetHost* client;
	ENetPeer* server;
	void sendNewPlayer(Player);
	void handleClient();
	void sendNewSession(int sID, std::string map);
	void update();
	bool active();
	int sessionID;
};

#endif
