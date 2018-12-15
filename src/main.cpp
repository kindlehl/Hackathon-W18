#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <fstream>
#include <vector>

#include "../inc/player.h"
#include "../inc/environment.h"
#include "../inc/bullet.h"

using namespace std;

vector<Env> envs;	//Environment objects like walls

int main (int argc, char** argv) {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Hackathon");
	window.setFramerateLimit(60);

	Player player;
	Bullet bullet;

	int num_tiles;
	string background_path;
	ifstream test_map("maps/test.map"); //map I'm using to test with
	
	test_map >> background_path;
	test_map >> num_tiles;

	//fill map with environmental stuff until an invalid env is found (END OF FILE also)
	while(num_tiles--) {
		envs.push_back(Env(test_map));
	}

	sf::RectangleShape background;
	sf::Texture* background_tex = new sf::Texture;
	background_tex->loadFromFile(background_path);

	background.setTexture(background_tex);
	background.setPosition(sf::Vector2f(0,0));
	background.setSize(static_cast<sf::Vector2f>(window.getSize()));

	bool isFiring = false;

    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
			else if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
				player.action(event);
			}	
        }

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			isFiring = true;
		}
        // Clear screen
        window.clear();
        // Draw the sprite
		
		window.draw(background);

		player.update();

		//draw environmental things
		for(auto &e : envs ) {
			window.draw(e);
		}

		if (isFiring) {
			//bullet.setPosition(player.hitbox.getPosition());
			bullet.draw(window);
			bullet.fireRight(2, player.hitbox.getPosition());

			isFiring = false;
		}
        window.draw(player);
		//bullet testing
		//window.draw(bullet);
        // Draw the string
        window.display();
    }
    return EXIT_SUCCESS;
}
