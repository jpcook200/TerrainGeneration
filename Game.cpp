#include "Game.h"

Game::Game()
{
	init();
}

void Game::init()
{
	m_window.create(sf::VideoMode({ 1920, 1080 }), "Terrain Generation");
	m_window.setFramerateLimit(60);

    m_world.generate();
}

void Game::run()
{
    while (m_window.isOpen())
    {
        processInput();
        update();
        render();
    }
}

void Game::processInput()
{ 
    while (const std::optional event = m_window.pollEvent())
    {
        // Close window: exit
        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
        }

    }
}

void Game::update()
{

}

void Game::render()
{
    m_window.clear(sf::Color(50, 150, 250));
    m_world.render(m_window);
    m_window.display();
}
