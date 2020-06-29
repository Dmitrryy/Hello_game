#pragma once



#include <tinyxml2.h>

#include "config.h"
#include "Entity.h"
#include "Player.h"
#include "Solid.h"
#include "Menu.h"

#include <cassert>
#include <iostream>

namespace ezg {


    class NodeGame
    {

        friend TileMap;
        friend Entity;
        friend Menu;

        NodeGame            (const NodeGame& _that)      = delete; //not saported
        NodeGame& operator= (const NodeGame&)            = delete; //not saported
        NodeGame            (NodeGame&& _that)           = delete; //not saported
        NodeGame& operator= (NodeGame&&)                 = delete; //not saported


    public:

        NodeGame () 
            : m_map(nullptr)
            , m_hero(nullptr)
            , m_menu(this)
            , m_mod(GameMod::NotInitialized)
            , m_view(sf::FloatRect(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT))
            , m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Hello Game (ezg)", STYLE_WINDOW)
        {
            m_window.setKeyRepeatEnabled(false);
        }
        ~NodeGame();


        //function of drawing the whole picture
        void draw   (sf::RenderStates states);

        void WindowClear() { m_window.clear(); }

        //////////////////////////////////////////////////////
        // loads a map and objects from a file in a format XML.
        // P.S: I get this file using TileMap editor.
        //////////////////////////////////////////////////////
        bool loadLevelXML (const std::string& _fileXML);


        void checkEvents    () noexcept;
        void checkKeyBoard ( );


        // updates the coordinates of elements and checks for interaction
        void update         (float time);


        void restart() { loadLevelXML("Resource/Levels/lvl2.tmx"); }


        inline const float   getPosHeroX () const noexcept { return (m_hero == nullptr) ? 0 : m_hero->getPosX(); }
        inline const float   getPosHeroY () const noexcept { return (m_hero == nullptr) ? 0 : m_hero->getPosY(); }

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

        sf::View                                   m_view;
        sf::RenderWindow                           m_window;

        std::unique_ptr < Hero >                   m_hero;            
        std::unique_ptr < TileMap >                m_map;
        std::list < gsl::not_null < Entity* > >    m_entities; // an array with other elements of the game (enemies, bullets, etc.)

        Menu m_menu;
    }; // class NodeGame


} //namespace ezg