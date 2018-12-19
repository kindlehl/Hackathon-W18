#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <enet/enet.h>

#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

#include "../inc/bullet.h"
#include "../inc/player.h"
#include "../inc/environment.h"
#include "../inc/connection.h"
#include "../inc/inviswall.h"
#include "../inc/rain.h"

using namespace std;

vector<Hitbox*> envs;	//Environment objects like walls
vector<Bullet> bulletVec;
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

	sf::Music music;
	if (!music.openFromFile("sounds/Showdown.wav"))
		return -1; // error
	music.play();
	
	ENetPeer* server; 
	ENetHost* client;

	//Connect to server 
	if(!establishConnection(&server, &client)){
		return 1;
	}

	//creates pattern for uber pwnage
	Rain matrix_background;

	string map = "maps/test.map";
	direction bulletDir = None;

	auto background = loadMap(map);

	
	envs.push_back(new InvisWall(sf::IntRect(-100, 0, 100, screenSize.y)));//left wall
	envs.push_back(new InvisWall(sf::IntRect(screenSize.x, 0, 100, screenSize.y)));	//right wall
	envs.push_back(new InvisWall(sf::IntRect(0, -100, screenSize.x, 100)));//top wall
	envs.push_back(new InvisWall(sf::IntRect(0, screenSize.y, screenSize.x, 100)));	//bottom wall

	//create player locally and on the server
	Player player(FRIENDLY_PLAYER_TYPE);
	envs.push_back(&player);

	Player enemy(ENEMY_PLAYER_TYPE);
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
            if (event.type == sf::Event::Closed) {
                window.close();
			} else if (event.type == sf::Event::MouseMoved) {
				sf::Vector2i pixelCoord(event.mouseMove.x, event.mouseMove.y);
				cout << "x: " << window.mapPixelToCoords(pixelCoord).x << "y: " << window.mapPixelToCoords(pixelCoord).y << std::endl;
			}
			if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
				player.action(event);
			}
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
				switch (player.currentState) {
				case Idle:
					if (player.facingState == WalkingRight) {
						bulletDir = Right;
						break;
					} else if (player.facingState == WalkingLeft) {
						bulletDir = Left;
						break;
					} else if (player.facingState == WalkingUp) {
						bulletDir = Up;
						break;
					} else if (player.facingState == WalkingDown) {
						bulletDir = Down;
						break;
					} else {
						bulletDir = Right;
						break;
					}
				case WalkingRight: 
					bulletDir = Right;
					break;
				case WalkingLeft:
					bulletDir = Left;
					break;
				case WalkingUp:
					bulletDir = Up;
					break;
				case WalkingDown:
					bulletDir = Down;
					break;
				case WalkingUpRight:
					bulletDir = UpRight;
					break;
				case WalkingDownRight:
					bulletDir = DownRight;
					break;
				case WalkingDownLeft:
					bulletDir = DownLeft;
					break;
				case WalkingUpLeft:
					bulletDir = UpLeft;
					break;
				}
			}
        }
		
		/*
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::)) {
			isFiring = true;
		}
		*/


        // Clear screen
        window.clear();
        // Draw the sprite
		
		window.draw(background);

		player.sendUpdate(server, client);
		checkServer(enemy, client);

		for(auto iter = bulletVec.begin(); iter < bulletVec.end(); iter++) {
			iter->update();
			Hitbox* hitEnv = iter->checkCollision(envs);
			if(hitEnv != NULL){
				if(hitEnv->type != ENVIRONMENT_TYPE){
					bulletVec.erase(iter);
					player.rewinding = true;
					enemy.rewinding = true;
					break;
				}
				if(hitEnv->type == ENVIRONMENT_TYPE){

					bulletVec.erase(iter);
					break;
				}
			}
		}



		matrix_background.update();

		window.draw(matrix_background);
		window.draw(background);
		
		//sort environmental objects based on their z-index
		sort(envs.begin(), envs.end(), hitboxCompare());

		//draw all environmental things
		for(auto &e : envs ) {
			window.draw(*e);
		}

		sf::Vector2i velocity;
		sf::Vector2i offset;
		// load bullet vectors 
		if (bulletDir == Right) {
			velocity = sf::Vector2i(20, 0);
			offset = sf::Vector2i(player.hitbox.width, 0);
		} else if (bulletDir == Left) {
			velocity = sf::Vector2i(-20, 0);
			offset = sf::Vector2i(player.hitbox.width * -1, 0);
		} else if (bulletDir == Up) {
			velocity = sf::Vector2i(0, -20);
			offset = sf::Vector2i(0, player.hitbox.height * -1);
		} else if (bulletDir == Down) {
			velocity = sf::Vector2i(0, 20);
			offset = sf::Vector2i(0, player.hitbox.height);
		} else if (bulletDir == UpRight) {
			velocity = sf::Vector2i(12, -12);
			offset = sf::Vector2i(player.hitbox.width, player.hitbox.height * -1);
		} else if (bulletDir == DownRight) {
			velocity = sf::Vector2i(12, 12);
			offset = sf::Vector2i(player.hitbox.width, player.hitbox.height);
		} else if (bulletDir == DownLeft) {
			velocity = sf::Vector2i(-12, 12);
			offset = sf::Vector2i(player.hitbox.width * -1, player.hitbox.height);
		} else if (bulletDir == UpLeft) {
			velocity = sf::Vector2i(-12, -12);
			offset = sf::Vector2i(player.hitbox.width * -1, player.hitbox.height * -1);
		}

		if(velocity.x || velocity.y){

			bulletVec.push_back(Bullet(player.hitbox, velocity, offset, client, server));
			bulletDir = None;
		}

		player.update();
		enemy.update();

		for(auto& b : bulletVec) {
			window.draw(b);
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
	
			int type;
			memcpy(&type, event.packet->data, 4);
			if(type == PLAYER) {
				std::cout << "Recieved player update from server" << std::endl;
				enemy.updateFromBuffer((char*)event.packet->data+4);
			}else if (type == BULLET) {
				cout << "REQUEST TO CREATE BULLET RECEIVED" << endl;
				bulletVec.push_back(Bullet((char*)event.packet->data+4));
			}

			enet_packet_destroy (event.packet);
		}	
	}
}
