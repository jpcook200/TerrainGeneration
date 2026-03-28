#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

int main()
{
    // ---------------------------------------------- Initialise ----------------------------------------------------------------
    sf::RenderWindow window(sf::VideoMode({1920, 1080 }), "SFML window");

    // Start the game loop
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
        window.clear();;
        window.display();
    }
}