#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <fstream>
#include <vector>

#include "../inc/player.h"
#include "../inc/environment.h"

using namespace std;

int main (int argc, char** argv) {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Hackathon");
	window.setFramerateLimit(60);
	Player player;

	int num_maps;
	ifstream test_map("maps/test.map"); //map I'm using to test with
	
	test_map >> num_maps;
	vector<Env> envs;	//Environment objects like walls

	//fill map with environmental stuff until an invalid env is found (END OF FILE also)
	while(num_maps--) {
		envs.push_back(Env(test_map));
	}

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
        // Clear screen
        window.clear();
        // Draw the sprite

		player.update();

		//draw environmental things
		for(auto &e : envs ) {
			window.draw(e);
		}

        window.draw(player);
        // Draw the string
        window.display();
    }
    return EXIT_SUCCESS;
}
