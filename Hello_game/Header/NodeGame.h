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

        //������� ��������� ���� �������
        virtual void draw   (sf::RenderTarget& target, sf::RenderStates states) const override;

        // ��������� �����
        // todo: ��������� ����� � �� ������������ �������� (����� � ��) �� ����� (xml)
        bool loadLevel      (const std::string& tileset, sf::Vector2u tileSize/*, const int* tiles*/, unsigned int width, unsigned int height);

        // ��������� ���������� � ������� ����� 
        void loadHero       (sf::Texture& texture, int texture_loc_x, int texture_loc_y, float pos_x, float pos_y, float _width, float _height);
        
        //��������� ���������� ��������� � ��������� �� ��������������
        void update         (float time);

    private:

        ////////////////////////////////////////////////////////////////////////
        // ��� ���������� ��������� ���� ��������� �� ������� m_entities (todo)
        void upAllPosition (float time, Direction _dir);

        void allColision   (Direction _dir);

        void upSprite      ();
        ////////////////////////////////////////////////////////////////////////



    public: //private!!!!!
        Hero                      m_hero;            // ������� �����
        TileMap                   m_map;             // �����
        std::vector<Entity*>      m_entities;        // ������ � ���������� ���������� ���� (�����, ���� � ��)

    }; // class NodeGame

} //namespace ezg