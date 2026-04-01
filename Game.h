#pragma once

#include "World.h"

class Game
{
	sf::RenderWindow m_window;
	sf::View m_view;

	// create world instance takes world size as number of blocks
	World m_world{ sf::Vector2i(500, 300), 16};

public:
	Game();
	void init();
	void run();
	void processInput();
	void update();
	void render();
	void clampView();
};

