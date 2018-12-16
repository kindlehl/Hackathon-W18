#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <enet/enet.h>

#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

#include "../inc/player.h"
#include "../inc/environment.h"
#include "../inc/connection.h"

using namespace std;

vector<Hitbox*> envs;	//Environment objects like walls
sf::Vector2i screenSize(800,800);

bool establishConnection(ENetPeer**, ENetHost**);
sf::RectangleShape loadMap(string map);

int main (int argc, char** argv) {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(screenSize.x, screenSize.y), "Hackathon");
	sf::View view;
	view.setCenter(window.getSize().x / 2, window.getSize().y / 2);
	view.rotate(45);
	view.setSize(view.getSize().x, view.getSize().y*2);
	window.setView(view);
	window.setFramerateLimit(60);

	ENetPeer* server; 
	ENetHost* client;
	
	//Connect to server 
	if(!establishConnection(&server, &client)){
		return 1;
	}

	string map = "maps/test.map";

	auto background = loadMap(map);

	//create player locally and on the server
	Player player;
	Connection connection(client, server, NEW, player, map);

	envs.push_back(&player);
	
    while (window.isOpen())
    {
        // Clear screen
        window.clear();

        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
			else if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
				player.action(event);
			} else if (event.type == sf::Event::MouseMoved) {
				sf::Vector2i pixelCoord(event.mouseMove.x, event.mouseMove.y);
				cout << "x: " << window.mapPixelToCoords(pixelCoord).x << "y: " << window.mapPixelToCoords(pixelCoord).y << std::endl;
			}
        }

		player.update();

		window.draw(background);
		
		//move drawables into proper drawing order
		//player is never sorted into the array
		//because he should be drawn first each time
		sort(envs.begin(), envs.end(), hitboxCompare());

		//draw environmental things
		for(auto &e : envs ) {
			window.draw(*e);
		}

        // Draw the string
        window.display();
    }
    return EXIT_SUCCESS;
}
bool establishConnection(ENetPeer** server, ENetHost** client){

	if (enet_initialize () != 0) {
        fprintf (stderr, "An error occurred while initializing ENet.\n");
        return false;
    }
    atexit (enet_deinitialize);

	//set up address to local server
	ENetAddress address;
	enet_address_set_host(&address, "hack.hunterlannon.net");
	address.port = 9000;

	//create client to send data
	*client = enet_host_create(NULL, 1, 2, 0,0);
	//create peer to send data to server
	*server = enet_host_connect(*client, &address, 2, 0);

	if(*server == NULL){
		cerr << "Error connecting to server" << endl;
	}

	//establish connection to the server
	ENetEvent netevent;
	if (enet_host_service (*client, &netevent, 5000) > 0 && netevent.type == ENET_EVENT_TYPE_CONNECT) {
		puts ("Connection to some.server.net:1234 succeeded.");
	}

	////send packet to create session
	//[> Create a reliable packet of size 7 containing "packet\0" <]
	//ENetPacket * packet = enet_packet_create ("packet", 
											  //strlen ("packet") + 1, 
											  //ENET_PACKET_FLAG_RELIABLE);
	//[> Send the packet to the peer over channel id 0. <]
	//[> One could also broadcast the packet by         <]
	//[> enet_host_broadcast (host, 0, packet);         <]
	//enet_peer_send (server, 0, packet);
	//[> One could just use enet_host_service() instead. <]
	//THIS IS NEEDED TO SEND DATA
	enet_host_flush (*client);
	
	return true;

}

sf::RectangleShape loadMap(string map){
	int num_tiles;
	string background_path;
	ifstream test_map(map); //map I'm using to test with
	
	test_map >> background_path;
	test_map >> num_tiles;

	//fill map with environmental stuff until an invalid env is found (END OF FILE also)
	while(num_tiles--) {
		envs.push_back(new Env(test_map));
	}

	sf::RectangleShape background;
	sf::Texture* background_tex = new sf::Texture;
	background_tex->loadFromFile(background_path);

	background.setTexture(background_tex);
	background.setPosition(sf::Vector2f(0,0));
	background.setSize(static_cast<sf::Vector2f>(screenSize));
	
	return background;
}
