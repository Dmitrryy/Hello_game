#pragma once



#include "../config.h"


#include "../Entities/Hero/Player.h"
#include "../Entities/Entity.h"

#include "../Menu/Menu.h"
#include "../TileMap/TileMap.h"

#include <cassert>
#include <iostream>

namespace ezg {

    enum class GameMood {
        NotInitialized
        , Loading
        , MainMenu
        , Pause
        , Restart
        , Game
        , Death
        , Exit
    };

    std::string enumName(GameMood _en);
    std::ostream& operator<<(std::ostream& _stream, GameMood _en);


    class NodeGame
    {
    public:

        NodeGame            (const NodeGame& _that)      = delete; //not saported
        NodeGame& operator= (const NodeGame&)            = delete; //not saported
        NodeGame            (NodeGame&& _that)           = delete; //not saported
        NodeGame& operator= (NodeGame&&)                 = delete; //not saported


    public:

        NodeGame();
        ~NodeGame();


        //function of drawing the whole picture
        void draw   (sf::RenderStates states);


        //////////////////////////////////////////////////////
        // loads a map and objects from a file in a format XML.
        // P.S: I get this file using TileMap editor.
        bool loadLevelXML (const std::string& _fileXML);
        //////////////////////////////////////////////////////

        void checkEvents    ();
        void checkKeyBoard  ();


        // updates the coordinates of elements and checks for interaction and etc
        void update         ();


        inline const float   getPosHeroX () const noexcept { return m_hero.getPosX(); }
        inline const float   getPosHeroY () const noexcept { return m_hero.getPosY(); }


        inline const GameMood getMood    () const noexcept { return m_mood; }

        
        std::string localDebugString();


        void changeMood(GameMood _new);

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


        void _addMainMenu_();
        void _addPauseMenu_();
        void _addDeathMenu_();


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


    private:

        GameMood               m_mood;

        sf::Clock              m_clock;
        double                 m_time;
        float                  m_period;

        sf::View               m_view;
        sf::RenderWindow       m_window;

        ezg::Hero              m_hero;            
        TileMap                m_map;

        std::list < Entity* >  m_entities; // an array with other elements of the game (enemies, bullets, etc.)

        menu::MenuManager      m_menus;
        menu::MenuManager      m_debug_cnsl;
        bool                   m_cnslIsActive;

        sf::Texture            m_enemy_texture;
    }; // class NodeGame


} //namespace ezg