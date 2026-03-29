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

    ////movable camera
    sf::View camera(sf::FloatRect({ 0, 0 }, {1920, 1080}));
    camera.zoom(1.5);
  
    // Start the game loop -------------------------------------
    while (window.isOpen())
    {
        // ---------------------------------- Process events -----------------------------
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();

            float speed = 50.f;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
                camera.move({ -speed, 0 });

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                camera.move({ speed, 0 });

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
                camera.move({ 0, -speed });

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                camera.move({ 0, speed });
        }

        // ----------------- Render -------------------------------------
        window.clear(sf::Color(50, 150, 250));
        window.setView(camera);
        worldGen.generateWorld();
      
        window.display();
    }
}