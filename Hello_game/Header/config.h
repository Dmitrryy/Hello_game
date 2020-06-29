#pragma once


#include <tinyxml2.h>
#include <SFML/Graphics.hpp>


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
static int GAME_SPEED_CONSTROLLER   = 1200;
//
//display settings
static int SCALE_ALL_X              = 5;
static int SCALE_ALL_Y              = 5;
//
#define STYLE_WINDOW sf::Style::Fullscreen
//
//window sizes
static int WINDOW_WIDTH = 1920;
static int WINDOW_HEIGHT = 1080;
//
//font
#define FONT_FNAME              "Resource/19440.ttf"
#define FONT_DEFAULT_SIZE       50
#define FONT_DEFAULT_COLOR      sf::Color(255, 255, 255)
//
//hero settings
#define HERO_TEXTURE_FNAME      "Resource/Images/Player.png"
#define HERO_TEXTURE_LOC_X      0
#define HERO_TEXTURE_LOC_Y      8
#define HERO_WIDTH              8
#define HERO_HEIGHT             8
//
//menu settings
#define MENU_BACKGROUND_PAUSE_FNAME "Resource/Images/menu_pause.png"
#define MENU_BACKGROUND1_FNAME      "Resource/Images/menu1.png"
/////////////////////////////////////////////////////////////////////////////


namespace ezg {

    
    class NodeGame;

    class TileMap;
    class Layer;

    class Entity;
    class Player;
    class Enemy;
    class Solid;
    class SolidAbove;
    class Menu;


    enum class GameMod {
          NotInitialized
        , Loading
        , MainMenu
        , Pause
        , Game
        , Exit
    };


    enum class TipeMenu {
          Main
        , Pause
    };


    enum class MenuButton {
          toGame
        , toExit
        , toMainMenu
        , Continue
        , Restart
    };


    //todo
    enum class Stat {
          Stay
        , WalkLeft
        , WalkRight
    };


    enum class Direction {
          Left
        , Right
        , Up
        , Down
        , Vertical
        , Horixontal
    };


    //available game object types
    enum class TipeEntity {
          MainHero
        , Solid
        , SolidAbove
        , Stairs
        , Enemy
    };

} //namespace ezg