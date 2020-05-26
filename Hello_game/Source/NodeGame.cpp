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
    NodeGame::loadHero(sf::Texture& texture, float pos_x, float pos_y, float _width, float _height) {

        m_hero.load(texture, pos_x, pos_y, _width, _height);

    }


    void 
    NodeGame::update(float time) {

//************************************************************
// ��������(��� ������� ��������):
//      1. ������� ��� �������� �� � (������������� on_ground = false �.�. �������� ����� ������)
//      2. ��������� �� �������������� � ������
//      3. ������� ��� �������� �� �
//      4. ��������� �� �������������� � ������
//      5. �������� �������� �� x
//      6. ������������� ������ � ����� �������
// ���� � ��������� �� �������������� ��������� ����� �����
//************************************************************

        m_hero.upPosition(time, Direction::Horixontal);
        m_hero.colisionMap(Direction::Horixontal, m_map);
        m_hero.on_ground = false;

        m_hero.upPosition(time, Direction::Vertical);
        m_hero.colisionMap(Direction::Vertical, m_map);

        m_hero.setSpeedX(0);

        m_hero.sprite.setPosition(m_hero.m_hit_box.left, m_hero.m_hit_box.top);
    } // update


} //namecpace ezg