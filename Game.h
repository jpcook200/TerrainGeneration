#pragma once

#include "World.h"

class Game
{
	sf::RenderWindow m_window;
	World m_world{ sf::Vector2i(1630, 1600), 16};

public:
	Game();
	void init();
	void run();
	void processInput();
	void update();
	void render();
};

