#include "Game.h"

Game::Game()
{
	init();
}

void Game::init()
{
	m_window.create(sf::VideoMode({ 1920, 1080 }), "Terrain Generation");
    m_window.setFramerateLimit(60);

    m_view.setCenter({
        static_cast<float>(m_window.getSize().x) / 2.f,
        static_cast<float>(m_window.getSize().y) / 2.f
    });

    m_view.setSize({
        static_cast<float>(m_window.getSize().x),
        static_cast<float>(m_window.getSize().y)
    });

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

        // move the view
        float speed = 16.f;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            m_view.move({ -speed, 0 });

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            m_view.move({ speed, 0 });

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            m_view.move({ 0, -speed });

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            m_view.move({ 0, speed });

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Equal))
            m_view.zoom(0.9f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Hyphen))
            m_view.zoom(1.1f);

        clampView();

        m_window.setView(m_view);
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

void Game::clampView()
{
    sf::Vector2f center = m_view.getCenter();

    float halfW = m_view.getSize().x / 2.0f;
    float halfH = m_view.getSize().y / 2.0f;

    float worldW = m_world.getWorldSize().x * m_world.getGridSize();
    float worldH = m_world.getWorldSize().y * m_world.getGridSize();

    // left
    if (center.x - halfW < 0)
        center.x = halfW;
    // right
    if (worldW - center.x < halfW)
        center.x = worldW - halfW;
    // down
    if (worldH - center.y < halfH)
        center.y = worldH - halfH;
    // up
    if (center.y < halfH)
        center.y = halfH;

    m_view.setCenter(center);
}
