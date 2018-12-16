#include "../inc/connection.h"

#include <enet/enet.h>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

//peers (players) 1 and 2
ENetPeer *p1 = NULL, *p2 = NULL;
ENetHost* server;

void handleEvents(ENetHost* server);
void send(ENetEvent);

int main(int argc, char** argv) {

	if (enet_initialize () != 0) {
        fprintf (stderr, "An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }

    atexit (enet_deinitialize);

	ENetAddress address;

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



	while(1) {
		handleEvents(server);
	}
	enet_host_destroy(server);
}

void handleEvents(ENetHost* server) {
	//std::cout << "Checking event queue" << std::endl;
	ENetEvent event;
	/* Wait up to 1000 milliseconds for an event. */
	while (enet_host_service (server, & event, 0) > 0)
	{
		std::cout << "Found event" << std::endl;

		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			printf ("A new client connected from %x:%u.\n", 
					event.peer -> address.host,
					event.peer -> address.port);

			//register players
			if(!p1) {
				p1 = event.peer;
				printf("Registered player 1\n");
			}else if (!p2) {
				p2 = event.peer;
				printf("Registered player 2\n");
			} else {
				printf("A third player tried to join but was rejected\n");
			}
			
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			printf ("A packet of length %u containing %c was received from %s on channel %u.\n",
					event.packet -> dataLength,
					event.packet -> data,
					event.peer -> data,
					event.channelID);
			//send packet to the other player that did not send it.
			send(event);
			/* Clean up the packet now that we're done using it. */
			enet_packet_destroy (event.packet);
			
			break;
		   
		case ENET_EVENT_TYPE_DISCONNECT:
			//register players
			if(event.peer->data == p1->data){
				printf("Player 1 disconnected\n");
				p1 = NULL;
			} else if( event.peer->data == p2->data ) {
				printf("Player 2 disconnected\n");
				p2 = NULL;
			}
		}
	}
}

void send(ENetEvent e) {
	if(p2 && p2) {
		printf("Transmitting packet\n");
	}else {
		printf("Dropping packet\n");
		return;
	}	

	if(e.peer->data == p1->data) {
		printf("Sending Player 1's data to Player 2\n");
		enet_peer_send(p2, 0, e.packet);
		enet_host_flush(server);
	}else if(e.peer->data == p2->data) {
		printf("Sending Player 2's data to Player 1\n");
		enet_peer_send(p1, 0, e.packet);
		enet_host_flush(server);
	}

}
