#pragma once


#include <tinyxml2.h>
#include <SFML/Graphics.hpp>


#include <vector>
#include <iostream>
#include <sstream>
#include <cassert>
#include <list>
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
//window sizes
static int WINDOW_WIDTH             = 16 * 16 * SCALE_ALL_X;
static int WINDOW_HEIGHT            = 8 * 16 * SCALE_ALL_Y;
//
//
//hero settings
#define HERO_TEXTURE_FNAME      "Resource/Images/Player.png"
#define HERO_TEXTURE_LOC_X      1
#define HERO_TEXTURE_LOC_Y      9
#define HERO_WIDTH              7
#define HERO_HEIGHT             7
/////////////////////////////////////////////////////////////////////////////


namespace ezg {

    
    class NodeGame;
    class TileMap;
    class Layer;
    class Entity;
    class Player;
    class Enemy;
    class Solid;


    enum class GameMod {
          Menu
        , Pause
        , Game
        , Exit
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