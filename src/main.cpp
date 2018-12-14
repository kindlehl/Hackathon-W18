#include <SFML/Graphics.hpp>
#include "../inc/player.h"

using namespace std;

int main (int argc, char** argv) {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Hackathon");
	Player player;

    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
			else if (event.type == sf::Event::KeyPressed) {
				player.action(event.key.code);
			}	
        }
        // Clear screen
        window.clear();
        // Draw the sprite
        window.draw(player);
        // Draw the string
        window.display();
    }
    return EXIT_SUCCESS;
}
