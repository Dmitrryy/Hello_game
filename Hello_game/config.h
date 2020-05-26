#pragma once


#define WINDOW_WIDTH 16 * 16 * 4
#define WINDOW_HEIGHT 8 * 16 * 4

#define HERO_WIDTH 13
#define HERO_HEIGHT 16

#define HERO_STAY_FRAME 1
#define HERO_WALK_FRAME 3

#define SIZE_TILE 16

#define MAP_WIDTH 16 * 2
#define MAP_HEIGHT 8



namespace ezg {

    enum class Stat {
        Stay,
        WalkLeft,
        WalkRight
    };

    enum class Direction {
        Left,
        Right,
        Up,
        Down,
        Vertical,
        Horixontal
    };

    enum class TipeEntity {
        MainHero
    };

} //namespace ezg