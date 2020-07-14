#pragma once


#include "Libs/tinyxml2/tinyxml2.h"
#include <SFML/Graphics.hpp>



#include "Entities/Animation.h"

#include <vector>
#include <iostream>
#include <sstream>
#include <cassert>
#include <list>
#include <map>
#include <algorithm>


#include <memory>


#include <gsl/gsl>
#define GSL_THROW_ON_CONTRACT_VIOLATION


////////////////////////////////settings/////////////////////////////////////
//game speed controller
static float GAME_SPEED_CONTROLLER   = 800.f;
//
//display settings
static float SCALE_ALL_X              = 5;
static float SCALE_ALL_Y              = 5;
//
#define STYLE_WINDOW sf::Style::Fullscreen
//
//window sizes
static int WINDOW_WIDTH = 16 * 80;
static int WINDOW_HEIGHT = 9 * 80;
//
//font
#define FONT_FNAME              "Resource/19440.ttf"
//#define FONT_DEFAULT_SIZE       50
#define FONT_DEFAULT_COLOR      sf::Color(255, 255, 255)
//
//hero settings
#define HERO_TEXTURE_FNAME      "Resource/Images/Player.png"
#define HERO_WIDTH              8
#define HERO_HEIGHT             8
//
#define HERO_MAX_HP             100
//
//menu settings
    //pause
#define MENU_BACKGROUND_PAUSE_FNAME "Resource/Images/menu_pause.png"
#define MENU_PAUSE_FONT_SIZE        50
#define MENU_PAUSE_FONT_COLOR       sf::Color(175, 144, 64)
    //main menu
#define MENU_BACKGROUND1_FNAME      "Resource/Images/menu1.png"
#define MAIN_MENU_FONT_SIZE         55
#define MAIN_MENU_FONT_COLOR        sf::Color(175, 144, 64)
    //death menu
#define MENU_BACKGROUND_DEATH_FNAME "Resource/Images/menu_death.png"
#define MENU_DEATH_FONT_SIZE        55
#define MENU_DEATH_FONT_COLOR       sf::Color(175, 144, 64)
//
//
//enemy texture
#define ENEMY_TEXTURE_FNAME         "Resource/Images/Enemies.png"
//
//
#define LVL_FNAME "Resource/Levels/lvl2.tmx"
/////////////////////////////////////////////////////////////////////////////


namespace ezg {

    
    class NodeGame;

    class Hero;

    class TileMap;
    class Layer;

    class Entity;
    class Needle;
    class Enemy;
    class Solid;
    class SolidAbove;

    class HeroBullet;

    class MushroomRed;
    class Bee;
    class Snake;


    enum class EffectType {
        Normal
        //, Gravity
        , Wounded
        , Immunity
        , Walking
        , Stop
        , Attack
        , Poisoning
        , OnFire
        , Freezing
    };



    struct Effect {

        Effect(EffectType _tp = EffectType::Normal, float _p = 0.f, float _time = 0.f) noexcept
            : _type(_tp)
            , _power(_p)
            , _time_effect(_time)
        {}

        EffectType _type;
        float _power;
        float _time_effect;

    };


    struct Hit {

        Hit(float _dmg = 0.f, Effect _eff = Effect()) noexcept
            : _damage(_dmg)
            , _effect(_eff)
        {}

        float _damage;
        //float m_corner = 0.f;

        Effect _effect;
        
    };


    enum class GameMood {
          NotInitialized
        //, Loading
        , MainMenu
        , Pause
        , Restart
        , Game
        , Death
        , Exit
    };


    /////////////////////////////////////////////
    //existing menu pages
    enum TipeMenu {
          Main
        , Pause
        , Death
    };
    //the same id button numbers
    enum MenuButton {
          toGame
        , toExit
        , toMainMenu
        , Continue
        , Restart
    };
    /////////////////////////////////////////////



    enum class Bullets
    {
        HeroBullet
        , BlueBullet
        , RedBullet
    };


    enum class Direction {
          Non
        , Left
        , Right
        , Up
        , Down
        , Vertical
        , Horixontal
    };

} //namespace ezg