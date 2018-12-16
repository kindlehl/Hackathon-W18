#include "../inc/connection.h"

//create new connection upon NEW packet type receipt
Connection::Connection(Packet p) : 
sessionID(p.sessionID),
clients(Player(p.payload), NULL)
{
	
}

Connection::~Connection() {}

