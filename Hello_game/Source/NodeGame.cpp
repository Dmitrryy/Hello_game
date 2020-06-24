#include "../Header/NodeGame.h"


namespace ezg {

    /* virtual */ void 
    NodeGame::draw      (sf::RenderTarget& target, sf::RenderStates states) const /* override */{

        target.draw(m_map, states);
        target.draw(m_hero, states);

    }


    bool 
    NodeGame::loadLevel (const std::string& tileset, sf::Vector2u tileSize/*, const int* tiles*/, unsigned int width, unsigned int height) {

        return m_map.load(tileset, tileSize/*, tiles*/, width, height);

    }


    void 
    NodeGame::loadHero(sf::Texture& texture, int texture_loc_x, int texture_loc_y, float pos_x, float pos_y, float _width, float _height) {

        m_hero.load(texture, texture_loc_x, texture_loc_y, pos_x, pos_y, _width, _height);

    }


    void 
    NodeGame::update(float time) {

//************************************************************
// ��������(��� ������� ��������):
//      1. ������� ��� �������� �� � (������������� on_ground = false �.�. �������� ����� ������)
//      2. ��������� �� �������������� � ������
//      3. ������� ��� �������� �� �
//      4. ��������� �� �������������� � ������
//      6. ������������� ������ � ����� �������
// ��� � ���� � ��������� �� d������������� ��������� ����� �����
//************************************************************

        upAllPosition(time, Direction::Horixontal);
        allColision(Direction::Horixontal);

        upAllPosition(time, Direction::Vertical);
        allColision(Direction::Vertical);

        upSprite();

    } // update

    void 
    NodeGame::upAllPosition(float time, Direction _dir) {

        m_hero.upPosition(time, _dir);

        for (Entity* elem : m_entities) {
            elem->upPosition(time, _dir);
        }

    }

    void 
    NodeGame::allColision(Direction _dir) {

        for (Entity* elem : m_entities) {
            m_hero.colision(elem, _dir);
        }

    }

    void 
    NodeGame::upSprite() {

        m_hero.sprite.setPosition(m_hero.m_hit_box.left, m_hero.m_hit_box.top);

        for (auto* elem : m_entities) {
            elem->sprite.setPosition(elem->m_hit_box.left, elem->m_hit_box.top);
        }
    }

} //namecpace ezg