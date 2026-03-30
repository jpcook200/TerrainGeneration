#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <map>

#include "Blocks.h"

class World
{
	// world settings
	sf::Vector2i m_worldSize;
	int m_gridSize;
	std::vector<std::vector<Blocks>> m_terrainData;
	std::map<Blocks, sf::IntRect> m_blockTextureMap;

	// tiles variables
	sf::Texture		m_tilesTexture;
	sf::Sprite		m_block{ m_tilesTexture };

	//perlin vairables
	float	amplitude		= 2.f;
	float	baseHeight		= 800.f;

	float	scale			= 0.05;
	int		octaves			= 3;
	float	persitence		= 0.11f;

	void init();
	
	//perlin methods
	// creates smoothness between interpolations - taken from perlin's cubic formula
	// takes a point and returns the polynomial
	float fade(float t);

	//linear interpolation between two points
	//takes start, end value an dinteprolation factor
	float lerp(float a, float b, float t);
	float gradient(int x);
	float perlin(float x);
	float perlinOctaves(float x, int octaves, float peristance, float scale);

	void initBlockMap();

public:
	
	World(sf::Vector2i worldSize, int gridSize);	// constructor world size as vector2i and grid size as int
	void generate();
	void render(sf::RenderWindow& window);

};

