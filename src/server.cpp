#include "../inc/server.h"
#include "../inc/connection.h"
#include <SFML/Graphics.hpp>

#include <vector>

using namespace std;

vector<Hitbox*> envs; //used to appease the mighty linker
sf::Vector2i screenSize(800,800);

void handleEvents(ENetHost* server);
int main(int argc, char** argv) {

	if (enet_initialize () != 0) {
        fprintf (stderr, "An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }

    atexit (enet_deinitialize);

	ENetAddress address;
	ENetHost * server;

	/* Bind the server to the default localhost.     */
	/* A specific host address can be specified by   */
	 //enet_address_set_host (& address, "127.0.0.1"); 
	address.host = ENET_HOST_ANY;
	/* Bind the server to port 1234. */
	address.port = 9000;
	server = enet_host_create (& address /* the address to bind the server host to */, 
								  2      /* allow up to 2 clients and/or outgoing connections */,
								  2      /* allow up to 2 channels to be used, 0 and 1 */,
								  0      /* assume any amount of incoming bandwidth */,
								  0      /* assume any amount of outgoing bandwidth */);
	if (server == NULL)
	{
		fprintf (stderr, "An error occurred while trying to create an ENet server host.\n");
		exit (EXIT_FAILURE);
	}

	vector<Connection> sessions;

	while(1) {
		
		handleEvents(server);

		//for(Connection& c : sessions) {
			//if(c.active()) {
				//c.handleClients();
			//}					
		//}		
	}
	enet_host_destroy(server);
}

void handleEvents(ENetHost* server) {
	//std::cout << "Checking event queue" << std::endl;
	ENetEvent event;
	/* Wait up to 1000 milliseconds for an event. */
	while (enet_host_service (server, & event, 1000) > 0)
	{
		std::cout << "Found event" << std::endl;

		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			printf ("A new client connected from %x:%u.\n", 
					event.peer -> address.host,
					event.peer -> address.port);
			/* Store any relevant client information here. */
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			printf ("A packet of length %u containing %c was received from %s on channel %u.\n",
					event.packet -> dataLength,
					event.packet -> data,
					event.peer -> data,
					event.channelID);
			{
				int c = 0;
				while(c < event.packet->dataLength) {
					printf("%x", (unsigned char) event.packet->data[c]);
				}
			}
			/* Clean up the packet now that we're done using it. */
			enet_packet_destroy (event.packet);
			
			break;
		   
		case ENET_EVENT_TYPE_DISCONNECT:
			printf ("%s disconnected.\n", event.peer -> data);
			/* Reset the peer's client information. */
			event.peer -> data = NULL;
			break;
		case ENET_EVENT_TYPE_NONE:
			printf("NOTHING HAPPENED\n");
		}
	}
}

