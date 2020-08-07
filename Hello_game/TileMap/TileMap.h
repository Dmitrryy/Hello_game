#pragma once

//#include "config.h"
#include <SFML/Graphics.hpp>
#include "../Libs/tinyxml2/tinyxml2.h"


namespace ezg {


    class Layer : public sf::Drawable, public sf::Transformable
    {
    public:

        Layer            (const Layer& _that)   = delete; //not saported
        Layer& operator= (const Layer&)         = delete; //not saported
        Layer            (Layer&& _that)        = delete; //not saported
        Layer& operator= (Layer&&)              = delete; //not saported

    public:

        Layer() noexcept
            : m_level(nullptr)
        {

        }

        ~Layer() = default;


    public:

        // layer loading
        bool load(const sf::Texture& texture, sf::Vector2u tileSize, std::unique_ptr<int[]>&& tiles, size_t width, size_t height);


    private:


        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


    private:


        sf::VertexArray m_vertices;

        std::unique_ptr<int[]> m_level;


    }; // class Layer


    //////////////////////////////////////////////////////////////////////////////////////
    //
    //
    //////////////////////////////////////////////////////////////////////////////////////


    class TileMap
    {
    public:

        TileMap             (const TileMap& _that)  = delete;   //not saported
        TileMap& operator=  (const TileMap&)        = delete;   //not saported
        TileMap             (TileMap&& _that)       = delete;   //not saported
        TileMap& operator=  (TileMap&&)             = delete;   //not saported


    public:

        TileMap  () noexcept;
        ~TileMap ();


    public:


        //////////////////////////////////////////////////
        //drawing function:
        //  draws the entire map.
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        //  draws everything except the background.
        void drawMap(sf::RenderTarget& target, sf::RenderStates states) const;
        //
        void drawBackGround(sf::RenderTarget& target, sf::RenderStates states) const;
        //
        void drawFrontGround(sf::RenderTarget& target, sf::RenderStates states) const;
        //////////////////////////////////////////////////


        void addBackGround(tinyxml2::XMLElement* _layer);
        void addFrontGround(tinyxml2::XMLElement* _layer);
        void addLayer(tinyxml2::XMLElement* _layer);



        inline const size_t getWidth () const noexcept { return m_width;  }
        inline const size_t getHeight() const noexcept { return m_height; }


        void clear();


        bool setTexture  (const char* _name) { return m_tileset.loadFromFile(_name); }

        void setWidth    (size_t _new)          noexcept { m_width = _new;      }
        void setHeight   (size_t _new)          noexcept { m_height = _new;     }

        void setTileSize (sf::Vector2u _size)   noexcept { m_tile_size = _size; }


    private:

        Layer* _readXML_ (tinyxml2::XMLElement* _layer);

        std::unique_ptr<int[]> _csvToArrInt_(const char* _data, size_t _num_elem) const;


    private:

        Layer*                   m_backGround;
        std::vector < Layer* >   m_map;
        Layer*                   m_frontGround;

        sf::Vector2u m_tile_size;
        sf::Texture m_tileset;

        size_t m_width;
        size_t m_height;

    }; // class TileMap


} //namespace ezg