#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

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
    float value = 0;
    float amplitude = 1.f;
    float freq = scale;
    float maxVal = 0;

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

int main()
{
    // ---------------------------------------------- Initialise ----------------------------------------------------------------
    sf::RenderWindow window(sf::VideoMode({1920, 1080 }), "SFML window");
    sf::Vector2 worldSize(16000, 16000);
    sf::Vector2 gridSize(16, 16);
    
    float amplitude = 100.f;
    float baseHeight = 500.f;
    
    float scale = 0.01;
    int octaves = 3;
    float persitance = 0.5f;

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
            float noise = perlinOctaves(i, 5, 0.5f, scale);
            float height = baseHeight + noise * amplitude;
            perlinLine[i].position = sf::Vector2f(i, height);
        }


        // ----------------- Render -------------------------------------
        window.clear();
        window.draw(perlinLine);
        window.display();
    }
}