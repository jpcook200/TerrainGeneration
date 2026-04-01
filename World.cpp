#include "World.h"

World::World(sf::Vector2i worldSize, int gridSize) 
    :   m_worldSize(worldSize),
        m_gridSize(gridSize)
{
    init();
}

void World::init()
{
    

    // load the texture into the texture member variable
    if (!m_tilesTexture.loadFromFile("./assets/textures.png"))
    {
        std::cerr << "Couldn't load texture" << std::endl;
        exit(-1);
    }

    initBlockMap();
    initTerrainLayers();
}

float World::fade(float t)
{
    return (6 * (t * t * t * t * t)) - (15 * (t * t * t * t)) + (10 * (t * t * t));
}

float World::lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

float World::gradient(int x)
{
    //scramble the bits
    x = (x << 13) ^ x;
    //hash to create pseudo randomness
    return 1.0f - ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f;
}

float World::perlin(float x)
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

    return lerp(dotLeft, dotRight, f);
}

float World::perlinOctaves(float x, int octaves, float peristence, float scale)
{
    float amplitude = 1.f;
    float value = 0;
    float freq = scale;
    float maxVal = 0;

    //for the number of given octaves run perlin noise with varying frequency and amplitude
    for (int i = 0; i < octaves; i++)
    {
        value += perlin(x * freq) * amplitude;
        maxVal += amplitude;
        amplitude *= peristence;
        freq *= 2;
    }

    // normalise the final value
    value /= maxVal;

    return value;
}

void World::initBlockMap()
{
    m_blockTextureMap[Blocks::Sand] = sf::IntRect({ 1 * m_gridSize, 6 * m_gridSize }, { m_gridSize, m_gridSize });
    m_blockTextureMap[Blocks::Stone] = sf::IntRect({ 0 * m_gridSize, 5 * m_gridSize }, { m_gridSize, m_gridSize });
}

void World::initTerrainLayers()
{
    m_terrainBaseHeight = m_worldSize.y * 0.75;

    m_terrainLayers.push_back({Blocks::Sand, m_terrainBaseHeight, 60, 0.02, 0.5f, 3});
    m_terrainLayers.push_back({Blocks::Stone, static_cast<int>(m_terrainBaseHeight * 0.5), 60, 0.05, 1.f, 5});
}

void World::generate()
{
    // initialise the vector size
    m_terrainData.resize(m_worldSize.y);
    for(auto& row : m_terrainData)
    { 
        row.resize(m_worldSize.x);
    }

    // add sand blocks at perlin noise heights
    for (int x = 0; x < m_worldSize.x; x++)
    {
        std::vector<int> heights;
        heights.reserve(m_terrainLayers.size());

        // for each defined terrain layer generate noise values 
        for (auto& layer : m_terrainLayers)
        {
            float noise = perlinOctaves(x, layer.octaves, layer.persistence, layer.scale);
            int height = layer.baseHeight + noise * layer.amplitude;
            std::cout << height << std::endl;
            heights.push_back(height);
        }

        // check heiaghts and set blocks under each height
        for (int y = 0; y < m_worldSize.y; y++)
        {
            if (y > heights[0])
            {
                m_terrainData[y][x] = Blocks::Air;
            }
            else if (y > heights[1])
            {
                m_terrainData[y][x] = Blocks::Sand;
            }
            else 
            {
                m_terrainData[y][x] = Blocks::Stone;
            }
        }
    }
}

void World::render(sf::RenderWindow& window)
{
    for (int y = 0; y < m_terrainData.size(); y++)
    {
        for (int x = 0; x < m_terrainData[y].size(); x++)
        {
            Blocks block = m_terrainData[y][x];
            
            // skip air blocks
            if (block == Blocks::Air)
            {
                continue;
            }

            // get texture coords
            sf::IntRect texRect = m_blockTextureMap.at(block);
            
            //set texture and position on the texture
            m_block.setTextureRect(texRect);
            
            float worldPixelHeight = m_worldSize.y * m_gridSize;
            m_block.setPosition({
                static_cast<float>(x * m_gridSize),
                worldPixelHeight - ((y + 1) * m_gridSize)
            });

            // draw to window
            window.draw(m_block);

        }
    }
}

sf::Vector2i World::getWorldSize()
{
    return m_worldSize;
}

int World::getGridSize()
{
    return m_gridSize;
}

