#include <SFML/Graphics.hpp>
#include <iostream>

class WorldGeneration 
{
    sf::RenderWindow& m_window;
    sf::Vector2i m_worldSize;
    sf::Vector2i m_gridSize;

    float amplitude = 2.f;
    float baseHeight = 800.f;

    float scale = 0.05;
    int octaves = 3;
    float persitance = 0.11f;

    //blocks
    sf::RectangleShape dirtBlock;
    sf::RectangleShape stoneBlock;

    //load the texture image

    sf::Texture tilesTexture;
    sf::Sprite block{tilesTexture};
    sf::Sprite block2{tilesTexture};
    
    void loadTexture()
    {
        if (!tilesTexture.loadFromFile("./assets/textures.png"))
        {
            std::cerr << "Couldn't load texture" << std::endl;
            exit(-1);
        }
    }

    // helper functions
    // creates smoothness between interpolations - taken from perlin's cubic formula
    // takes a point and returns the polynomial
    float fade(float t)
    {
        return (6 * (t * t * t * t * t)) - (15 * (t * t * t * t)) + (10 * (t * t * t));
    }

    //linear interpolation between two points
    //takes start, end value an dinteprolation factor
    float lerp(float a, float b, float t)
    {
        return a + t * (b - a);
    }

    float gradient(int x)
    {
        //scramble the bits
        x = (x << 13) ^ x;
        //hash to create pseudo randomness
        return 1.0f - ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f;

    }

    float perlin(float x)
    {
        // get left andm right points on the grid
        int xLeft = (int)x;
        int xRight = (int)x + 1;

        //get position of point in the grid cell
        float point = x - xLeft;

        // get gradients at grid points
        float gradLeft = gradient(xLeft);
        float gradRight = gradient(xRight);

        // get distances 
        float distLeft = x - xLeft;
        float distRight = x - xRight;

        // dot products
        float dotLeft = gradLeft * distLeft;
        float dotRight = gradRight * distRight;

        //fade
        float f = fade(point);

        return (lerp(dotLeft, dotRight, f));
    }

    float perlinOctaves(float x, int octaves, float peristance, float scale)
    {
        float value         =  0;
        float amplitude     = 1.f;
        float freq          = scale;
        float maxVal        = 0;

        //for the number of given octaves run perlin noise with varying frequency and amplitude
        for (int i = 0; i < octaves; i++)
        {
            value += perlin(x * freq) * amplitude;
            maxVal += amplitude;
            amplitude *= peristance;
            freq *= 2;
        }

        // normlaise the final value
        value /= maxVal;

        return value;
    }
public:
    // constructor, takes the window and draws the world to it
    // takes world size, grid size and reference to the window
    WorldGeneration(
        sf::RenderWindow& window,
        sf::Vector2i worldSize, 
        sf::Vector2i gridSize) : 
            m_window(window),
            m_worldSize(worldSize),
            m_gridSize(gridSize),
            dirtBlock(sf::Vector2f(gridSize.x, gridSize.y)),
            stoneBlock(sf::Vector2f(gridSize.x, gridSize.y))
    {

        loadTexture();
    };

    void generateWorld()
    {
        for (int x = 0; x < m_worldSize.x; x++)
        {
            float noise = perlinOctaves(x, octaves, persitance, scale);
            float noise2 = perlinOctaves(x, 4, persitance - 0.02, scale - 0.02);

            int height = floor(noise * m_gridSize.y * amplitude) + floor(baseHeight / m_gridSize.x);
            int height2 = floor(noise2 * m_gridSize.y * amplitude) + floor((baseHeight - 200.f) / m_gridSize.x);

            for (int y = height2; y <= height; y++)
            {
                block.setPosition(sf::Vector2f(x * m_gridSize.x, m_worldSize.y - (y * m_gridSize.y)));
                int tileX = 0;
                int tileY = 0;
                if (y == height)
                {
                    tileX = 1;
                    tileY = 6;
                }
                else
                {
                    tileX = 1;
                    tileY = 6;
                }
                sf::IntRect rect({ tileX * 16, tileY * 16 }, { 16, 16 });
                block.setTextureRect(rect);
                m_window.draw(block);
            }

            for (int y = 0; y <= height2; y++)
            {
                block2.setPosition(sf::Vector2f(x * m_gridSize.x, m_worldSize.y - (y * m_gridSize.y)));
                int tileX = 3;
                int tileY = 2;
                sf::IntRect rect({ tileX * 16, tileY * 16 }, { 16, 16 });
                block2.setTextureRect(rect);
                m_window.draw(block2);
            }
        }
    }

    void renderWorld()
    {

    }
};