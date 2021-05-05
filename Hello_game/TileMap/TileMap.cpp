#include <iostream>
#include <iomanip>
#include <sstream>
#include <cassert>


#include "TileMap.h"

namespace ezg {

    void 
    Layer::draw(sf::RenderTarget& target, sf::RenderStates states) const /* override */
    {
        
        target.draw(m_vertices, states);

    }


    bool 
    Layer::load(const sf::Texture& tileset, sf::Vector2u tileSize, std::unique_ptr<int[]>&& tiles, size_t width, size_t height)
    {
        //m_tile_size = tileSize;
        m_level = std::move(tiles);

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);


        // populate the vertex array, with one quad per tile
        for (unsigned int i = 0; i < height; ++i)
            for (unsigned int j = 0; j < width; ++j)
            {

                // get the current tile number
                // const int tileNumber = tiles[i * width + j];
                const int tileNumber =  m_level[i * width + j] - 1;

                if (tileNumber < 0) {
                    continue;
                }

                // find its position in the tileset texture
                const int tu = tileNumber % (tileset.getSize().x / tileSize.x);
                const int tv = tileNumber / (tileset.getSize().x / tileSize.x);


                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i * width + j) * 4];
                
                //std::cout << quad.size() << std::endl;

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


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // functions for class TileMap
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////


    TileMap::TileMap() noexcept
        : m_backGround(nullptr)
        , m_frontGround(nullptr)
        , m_width(0)
        , m_height(0)
    {

    }


    TileMap::~TileMap() {

        clear();

    }


    void TileMap::clear() {

        delete m_backGround;
        delete m_frontGround;
        m_backGround = m_frontGround = nullptr;

        for (auto& m : m_map) {
            delete m;
            m = nullptr;
        }
        m_map.clear();

        m_tile_size = sf::Vector2u();

        m_width = m_height = 0;
    }


    void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {

        states.texture = &m_tileset;

        if (m_backGround != nullptr) {
            target.draw(*m_backGround, states);
        }

        for (const auto& l : m_map) {
            target.draw(*l, states);
        }

        if (m_frontGround != nullptr) {
            target.draw(*m_frontGround, states);
        }
    }


    void TileMap::drawMap(sf::RenderTarget& target, sf::RenderStates states) const {

        states.texture = &m_tileset;

        for (const auto& l : m_map) {
            target.draw(*l, states);
        }

    }


    void TileMap::drawBackGround(sf::RenderTarget& target, sf::RenderStates states) const {

        states.texture = &m_tileset;

        if (m_backGround != nullptr) {
            target.draw(*m_backGround, states);
        }

    }


    void TileMap::drawFrontGround(sf::RenderTarget& target, sf::RenderStates states) const {

        states.texture = &m_tileset;

        if (m_frontGround != nullptr) {
            target.draw(*m_frontGround, states);
        }

    }


    void TileMap::addBackGround(tinyxml2::XMLElement* _layer) {

        if (m_backGround != nullptr) {

            std::cout << "background two times determined" << std::endl;

            m_backGround = nullptr;
        }

        m_backGround = _readXML_(_layer);

    }


    void TileMap::addFrontGround(tinyxml2::XMLElement* _layer) {

        if (m_frontGround != nullptr) {

            std::cout << "frontground two times determined" << std::endl;

            m_frontGround = nullptr;
        }

        m_frontGround = _readXML_(_layer);

    }


    void TileMap::addLayer(tinyxml2::XMLElement* _layer) {

        Layer* l = _readXML_(_layer);

        if (l != nullptr) {
            m_map.push_back(l);
        }
    }


    Layer* TileMap::_readXML_(tinyxml2::XMLElement* _layer) {

        if (_layer == nullptr) {
            return nullptr;
        }

        if (static_cast<size_t>(_layer->Int64Attribute("width")) != m_width 
         || static_cast<size_t>(_layer->Int64Attribute("height") != m_height)) 
        {
            std::cerr << "layer size does not match map size\n";
        }


        const tinyxml2::XMLElement* data = _layer->FirstChildElement("data");

        if (data == nullptr) {
            std::cerr << "\ncould not find date section in layer\n";
            return nullptr;
        }


        Layer* returnableLayer = new Layer();

        std::unique_ptr<int[]> lvl = _csvToArrInt_(data->GetText(), m_width * m_height);


        returnableLayer->load(m_tileset, m_tile_size, std::move(lvl), m_width, m_height);


        return returnableLayer;
    }


    std::unique_ptr<int[]> 
    TileMap::_csvToArrInt_(const char* _data, size_t _num_elem) const {

        std::istringstream in(_data);


        std::unique_ptr<int[]> returnable = std::make_unique<int[]>(_num_elem);
        for (size_t i = 0; i < _num_elem; i++) {

            in >> returnable[i];

            if (in.get() != ',' && i != _num_elem - 1) {
                assert(0);
            }

        }


        return returnable;
    }


    std::string TileMap::debugString() {

        using std::setw;
        using std::endl;
        using std::setfill;
        
        std::ostringstream out;
        out.setf(std::ios::left | std::ios::boolalpha);

        out << setw(7) << setfill('\t') << "TileMap:" << endl
            << setw(18) << "set background" << "    " << (m_backGround != nullptr) << endl
            << setw(19) << "set frontground" << "   " << (m_frontGround != nullptr) << endl
            << setw(16) << "num layers" << "   " << m_map.size() << endl
            << setw(14) << "size" << m_width << 'x' << m_height << endl
            << setw(17) << "tile size" << "  " << m_tile_size.x << 'x' << m_tile_size.y << endl;

        return out.str();
    }

} // namespace ezg