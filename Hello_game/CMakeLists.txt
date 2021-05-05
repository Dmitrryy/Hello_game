#[[
/****************************************************************************************
*
*   CmakeLists.txt
*
*   Created by dmitry
*   05.05.2021
*
***/
]]

cmake_minimum_required(VERSION 3.10)



#
## copy_resource
##=======================================================================================
set(FB_TARGET copy_resource)
add_custom_target(${FB_TARGET}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resource ${CMAKE_CURRENT_BINARY_DIR}/Resource
        )
##=======================================================================================
##=======================================================================================
#


#
## HelloGame
##=======================================================================================
project(HelloGame)


# search SFML library
find_package(SFML REQUIRED network audio graphics window system)


# set executable name
set(EXECUTABLE_NAME "game")
add_executable(${EXECUTABLE_NAME}
        main.cpp

        Entities/Bots/Bee/Bee.cpp
        Entities/Bots/Mushroom/Mushroom.cpp Entities/Bots/Mushroom/MushroomBlue.cpp Entities/Bots/Mushroom/MushroomRed.cpp
        Entities/Bots/Snake/Snake.cpp
        Entities/Hero/Player.cpp
        Entities/Traps/Needle/Needle.cpp
        Entities/Animation.cpp Entities/Entity.cpp

        Menu/Menu.cpp

        NodeGame/NodeGame.cpp
        NodeGame/initializing_menu.cpp

        TileMap/TileMap.cpp
        )


# add other source files
target_sources(${EXECUTABLE_NAME} PRIVATE
        Libs/tinyxml2/tinyxml2.cpp
        )


# link libraries
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${SFML_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} PRIVATE sfml-graphics sfml-audio)


# see above
add_dependencies(${EXECUTABLE_NAME} ${FB_TARGET})
##=======================================================================================
##=======================================================================================
