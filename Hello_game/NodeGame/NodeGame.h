#pragma once

#include <cassert>
#include <iostream>

#include "../config.h"


#include "../Entities/Hero/Player.h"
#include "../Entities/Entity.h"

#include "../Menu/Menu.h"
#include "../TileMap/TileMap.h"


namespace ezg {

    class Game
    {
    public:

        enum class IDtexture {
            e
        };

        enum class Mood;

        Game            (const Game& _that)      = delete; //not saported
        Game& operator= (const Game&)            = delete; //not saported
        Game            (Game&& _that)           = delete; //not saported
        Game& operator= (Game&&)                 = delete; //not saported


    public:

        Game();
        ~Game();


        void run();

        //function of drawing the whole picture
        void draw   ();


        //////////////////////////////////////////////////////
        // loads a map and objects from a file in a format XML.
        // P.S: I get this file using TileMap editor.
        bool loadLevelXML (const std::string& _fileXML);
        //////////////////////////////////////////////////////

        void checkEvents    ();
        void checkKeyBoard  ();


        // updates the coordinates of elements and checks for interaction and etc
        void update         (float _time);


        inline const float   getPosHeroX () const noexcept { return m_hero.getPosX(); }
        inline const float   getPosHeroY () const noexcept { return m_hero.getPosY(); }


        inline const Mood getMood    () const noexcept { return m_mood; }

        
        std::string localDebugString();


        void changeMood(Mood _new);

    private:

        ////////////////////////////////////////////////////////////////////////
        // to update the state of all elements from the m_entities array
        void upAllPosition (float _time, Direction _dir);
        //
        void allColision   (Direction _dir);
        //
        void allOtherUpdate(float _time);
        //
        //void upAnimation      (float _time);
        ////////////////////////////////////////////////////////////////////////


        void addObject(std::unique_ptr<Entity>&& _elem) { 
            if (_elem != nullptr) {
                m_entities.push_back(_elem.release());
            }
        }


        void clear();

        void _initialize_();


        ////////////////////////////////////////////////////////////////////////
        void _setMainMenu_();
        void _setPauseMenu_();
        void _setDeathMenu_();
        ////////////////////////////////////////////////////////////////////////


        ////////////////////////////////////////////////////////////////////////
        enum ConsoleType {
              Main
            , Hero
            , Entities
        };
        enum ConsoleButton {
              toMain
            , toHero
            , toEntities
            , Next
            , Pref
        };
        void checkEventDbConsole(const sf::Event& _event);
        void updateDbConsole(float _time);
        void _setDbConsole_();
        ////////////////////////////////////////////////////////////////////////


    private:

        Mood                   m_mood;

        sf::Clock              m_clock;
        double                 m_time;
        float                  m_period;
        size_t                 m_update_VS_render;

        sf::View               m_view;
        sf::RenderWindow       m_window;

        ezg::Hero              m_hero;            
        TileMap                m_map;

        std::list < Entity* >  m_entities; // an array with other elements of the game (enemies, bullets, etc.)

        menu::MenuManager      m_menus;

        menu::MenuManager      m_debug_cnsl;
        bool                   m_cnslIsActive;
        ////1 - every screen update
        //size_t                 m_refresh_rate_cnsl;

        sf::Texture            m_enemy_texture;
    }; // class NodeGame


    enum class Game::Mood {
        NotInitialized
        , Loading
        , MainMenu
        , Pause
        , Restart
        , Game
        , Death
        , Exit
    };
    std::string enumName(Game::Mood _en);
    std::ostream& operator<<(std::ostream& _stream, Game::Mood _en);

} //namespace ezg