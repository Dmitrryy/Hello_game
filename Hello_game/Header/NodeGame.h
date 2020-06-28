#pragma once

#include <vector>

#include <tinyxml2.h>

#include "config.h"
#include "Entity.h"
#include "Player.h"
#include "Solid.h"

#include <cassert>
#include <iostream>

namespace ezg {


    class NodeGame : public sf::Drawable, public sf::Transformable
    {

        friend TileMap;
        friend Entity;

        NodeGame            (const NodeGame& _that)      = delete; //not saported
        NodeGame& operator= (const NodeGame&)            = delete; //not saported
        NodeGame            (NodeGame&& _that)           = delete; //not saported
        NodeGame& operator= (NodeGame&&)                 = delete; //not saported


    public:

        NodeGame () noexcept
            : m_map(nullptr)
            , m_hero(nullptr)
            , m_mod(GameMod::Game)
        {}
        ~NodeGame();


        //function of drawing the whole picture
        void draw   (sf::RenderTarget& target, sf::RenderStates states) const override;


        //////////////////////////////////////////////////////
        // loads a map and objects from a file in a format XML.
        // P.S: I get this file using TileMap editor.
        //////////////////////////////////////////////////////
        bool loadLevelXML (const std::string& _fileXML);


        void checkEvent    (const sf::Event& _event) noexcept;
        void checkKeyBoard ( );


        // updates the coordinates of elements and checks for interaction
        void update         (float time);


        inline const float   getPosHeroX () noexcept { return (m_hero == nullptr) ? 0 : m_hero->getPosX(); }
        inline const float   getPosHeroY () noexcept { return (m_hero == nullptr) ? 0 : m_hero->getPosY(); }

        inline const GameMod getMod      () noexcept { return m_mod; }


    private:

        ////////////////////////////////////////////////////////////////////////
        // to update the state of all elements from the m_entities array
        void upAllPosition (float time, Direction _dir);
        //
        void allColision   (Direction _dir);
        //
        void upSprite      ();
        ////////////////////////////////////////////////////////////////////////


        inline void addObject(std::unique_ptr<Entity>&& _elem) { m_entities.push_back(_elem.release()); }


    private:

        GameMod                                    m_mod;

        //sf::Clock                                  m_clock;
        //double                                     m_time;
        //double                                     m_time_period;

        std::unique_ptr < Hero >                   m_hero;            
        std::unique_ptr < TileMap >                m_map;
        std::list < gsl::not_null < Entity* > >    m_entities; // an array with other elements of the game (enemies, bullets, etc.)


    }; // class NodeGame


} //namespace ezg