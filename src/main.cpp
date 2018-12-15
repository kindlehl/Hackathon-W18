#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <enet/enet.h>

#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

#include "../inc/bullet.h"
#include "../inc/player.h"
#include "../inc/environment.h"
#include "../inc/connection.h"
#include "../inc/rain.h"

using namespace std;

vector<Hitbox*> envs;	//Environment objects like walls
sf::Vector2i screenSize(800,800);

bool establishConnection(ENetPeer**, ENetHost**);
void checkServer(Player&, ENetHost*);
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

	//creates pattern for uber pwnage
	Rain matrix_background;

	string map = "maps/test.map";

	auto background = loadMap(map);

	//create player locally and on the server
	Player player;
	envs.push_back(&player);

	Player enemy;
	envs.push_back(&enemy);
	
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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			Bullet bullet;
			//bullet.setPosition(player.hitbox.getPosition());
			bullet.setPosition(20, 20);
			bullet.draw(window);
			window.draw(bullet);
		}
        // Clear screen
        window.clear();
        // Draw the sprite
		
		window.draw(background);

		player.sendUpdate(server, client);
		checkServer(enemy, client);

		player.update();
		enemy.update();

		matrix_background.update();
		window.draw(matrix_background);
		window.draw(background);
		
		//sort environmental objects based on their z-index
		sort(envs.begin(), envs.end(), hitboxCompare());

		//draw all environmental things
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
	if (enet_host_service (*client, &netevent, 1000) > 0 && netevent.type == ENET_EVENT_TYPE_CONNECT) {
		puts ("Connection to some.server.net:1234 succeeded.");
	}
	
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

void checkServer(Player& enemy, ENetHost* client) {
	ENetEvent event;
	/* check for incoming messages */
	while (enet_host_service (client, & event, 0) > 0)
	{
		if(event.type == ENET_EVENT_TYPE_RECEIVE) {
			std::cout << "Received update from server" << std::endl;
	
			int type;
			memcpy(&type, event.packet->data, 4);
			if(type == PLAYER) {
				std::cout << "Recieved player update from server" << std::endl;
				enemy.updateFromBuffer((char*)event.packet->data+4);
				cout << "Enemy hitbox: " << enemy.getHitbox().left <<
				" " << enemy.getHitbox().top << " " << enemy.getHitbox().width
				<< " " << enemy.getHitbox().height << endl;
			}

			enet_packet_destroy (event.packet);
		}	
	}
}
