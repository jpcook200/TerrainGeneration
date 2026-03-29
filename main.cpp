#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "WorldGeneration.cpp"

int main()
{
    // ---------------------------------------------- Initialise ----------------------------------------------------------------
    sf::RenderWindow window(sf::VideoMode({1920, 1080 }), "SFML window");
    sf::Vector2i worldSize(1600, 1600);
    sf::Vector2i gridSize(16, 16);
    WorldGeneration worldGen{ window, worldSize, gridSize };


    // Start the game loop -------------------------------------
    while (window.isOpen())
    {
        // ---------------------------------- Process events -----------------------------
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // ----------------- Render -------------------------------------
        window.clear(sf::Color(50, 150, 250));
        worldGen.generateWorld();
        
        window.display();
    }
}