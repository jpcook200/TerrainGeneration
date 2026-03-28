#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// helper functions
void fade()
{

}

void lerp()
{

}

void gradient()
{

}

int main()
{
   
    // ---------------------------------------------- Initialise ----------------------------------------------------------------
    sf::RenderWindow window(sf::VideoMode({1920, 1080 }), "SFML window");
    sf::Vector2 worldSize(10000, 10000);

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

        sf::VertexArray perlinLine(sf::PrimitiveType::LineStrip, worldSize.x);

        for (size_t i = 0; i <= worldSize.x; i++)
        {
            float height = 500.f + sin(i * 0.01) * 100;
            perlinLine[i].position = sf::Vector2f(i, height);
        }


        // ----------------- Render -------------------------------------
        window.clear();
        window.draw(perlinLine);
        window.display();
    }
}