#pragma once

#include <vector>


#include "config.h"
#include "Entity.h"
#include "Player.h"
#include "Solid.h"


namespace ezg {


    class NodeGame : public sf::Drawable, public sf::Transformable
    {
    public:

        NodeGame  ()
            : m_map()
            , m_hero()
        {}

        ~NodeGame () 
        {
            for (auto& elem : m_entities) {

                delete elem;       
            }
        }

        //функци€ рисовани€ всей картины
        virtual void draw   (sf::RenderTarget& target, sf::RenderStates states) const override;

        // загружает карту
        // todo: считывать карту и ее составл€ющие элементы (враги и тд) из файла (xml)
        bool loadLevel      (const std::string& tileset, sf::Vector2u tileSize/*, const int* tiles*/, unsigned int width, unsigned int height);

        // загружает информацию о главном герое 
        void loadHero       (sf::Texture& texture, int texture_loc_x, int texture_loc_y, float pos_x, float pos_y, float _width, float _height);
        
        //обновл€ет координаты элементов и провер€ет на взаимодействие
        void update         (float time);

    private:

        ////////////////////////////////////////////////////////////////////////
        // дл€ обновлени€ состо€ни€ всех элементов из массива m_entities (todo)
        void upAllPosition (float time, Direction _dir);

        void allColision   (Direction _dir);

        void upSprite      ();
        ////////////////////////////////////////////////////////////////////////



    public: //private!!!!!
        Hero                      m_hero;            // главный герой
        TileMap                   m_map;             // карта
        std::vector<Entity*>      m_entities;        // массив с остальными элемениами игры (враги, пули и тд)

    }; // class NodeGame

} //namespace ezg