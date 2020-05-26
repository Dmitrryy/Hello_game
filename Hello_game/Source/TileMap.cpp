#include "../Header/TileMap.h"


namespace ezg {

    /*virtual*/ void 
    TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const /* override */
    {
        // apply the transform
        //states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }


    bool 
    TileMap::load(const std::string& tileset, sf::Vector2u tileSize/*, const int* tiles*/, unsigned int width, unsigned int height)
    {
        m_tile_size = tileSize;


        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset)) {
            return false;
        }


        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(static_cast<size_t>(width) * height * 4);


        // populate the vertex array, with one quad per tile
        for (unsigned int i = 0; i < height; ++i)
            for (unsigned int j = 0; j < width; ++j)
            {

                // get the current tile number
                // const int tileNumber = tiles[i * width + j];
                const int tileNumber = level[i * width + j];


                // find its position in the tileset texture
                const int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                const int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);


                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i * width + j) * 4];


                // define its 4 corners
                quad[0].position = sf::Vector2f(j * tileSize.x, i * tileSize.y);
                quad[1].position = sf::Vector2f((j + 1) * tileSize.x, i * tileSize.y);
                quad[2].position = sf::Vector2f((j + 1) * tileSize.x, (i + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(j * tileSize.x, (i + 1) * tileSize.y);


                // define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }

        return true;
    }


    void 
    TileMap::upTile(int _position, int _num_tile) {

        sf::Vertex* quad = &m_vertices[_position * 4];

        const int tu = _num_tile % (m_tileset.getSize().x / m_tile_size.x);
        const int tv = _num_tile / (m_tileset.getSize().x / m_tile_size.x);

        quad[0].texCoords = sf::Vector2f(tu * m_tile_size.x, tv * m_tile_size.y);
        quad[1].texCoords = sf::Vector2f((tu + 1) * m_tile_size.x, tv * m_tile_size.y);
        quad[2].texCoords = sf::Vector2f((tu + 1) * m_tile_size.x, (tv + 1) * m_tile_size.y);
        quad[3].texCoords = sf::Vector2f(tu * m_tile_size.x, (tv + 1) * m_tile_size.y);

        level[_position] = _num_tile;
    }

} // namespace ezg