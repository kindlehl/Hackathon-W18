#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <fstream>
#include <vector>
#include "../inc/bullet.h"

#include "../inc/player.h"
#include "../inc/environment.h"

using namespace std;

vector<Env> envs;	//Environment objects like walls

int main (int argc, char** argv) {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 800), "Hackathon");
	sf::View view;
	view.setCenter(window.getSize().x / 2, window.getSize().y / 2);
	view.rotate(45);
	view.setSize(view.getSize().x, view.getSize().y*2);
	view.zoom(1.3);
	window.setView(view);
	window.setFramerateLimit(60);

	Player player;

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

	vector<Bullet> bulletVec;
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
			} else if (event.type == sf::Event::MouseMoved) {
				sf::Vector2i pixelCoord(event.mouseMove.x, event.mouseMove.y);
				cout << "x: " << window.mapPixelToCoords(pixelCoord).x << "y: " << window.mapPixelToCoords(pixelCoord).y << std::endl;
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
			Bullet newBullet;
			newBullet.setPosition(player.hitbox.getPosition());
			bulletVec.push_back(newBullet);
			//window.draw(bullet);
			//bullet.move(3, 0);
			//bullet.fireRight(3, player.hitbox.getPosition());
			//window.draw(bullet);
			isFiring = false;

		}
		for (int i = 0; i < bulletVec.size(); i++) {
			window.draw(bulletVec[i]);
			bulletVec[i].fireRight(5);
		}

        window.draw(player);
        // Draw the string
        window.display();
    }
    return EXIT_SUCCESS;
}
