#ifndef CONNECTION_H
#define CONNECTION_H

#include "../inc/player.h"

#include <utility>
#include <enet/enet.h>

enum p_type {
	NEW, // start new session/game
	CONNECT, //connect to a session
	DISCONNECT, //disconnect from session
	UPDATE	//game/object updates
};

struct Packet {
	int sessionID;
	unsigned char type;
	char* payload;
};

class Connection {
public:
	Connection(Packet);
	~Connection();
	std::pair<Player, Player> clients;
	void update();
	void handleClients();
	bool active();
	int sessionID;
};

#endif
