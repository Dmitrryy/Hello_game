#include "Player.h"


namespace ezg {

    void 
    Hero::load(sf::Texture& texture, float pos_x, float pos_y, float _width, float _height) {

        m_hit_box = sf::FloatRect(pos_x, pos_y, _width, _height);
        speed_x = (0);
        speed_y = (0);
        on_ground = (false);
        frame_stat = (Stat::Stay);


        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(82, 144, _width, _height));
        sprite.setPosition(pos_x, pos_y);
    }


    /* virtual */ void 
    Hero::draw(sf::RenderTarget& target, sf::RenderStates states) const /* override */ {

        target.draw(sprite, states);
    }


    void 
    Hero::jump() {

        if (on_ground) {
            setSpeedY(-0.25);
            on_ground = false;
        }
    }


    /* virtual */ void 
    Hero::colisionMap(Direction _dir, TileMap& _map) /* override */ {

        for (int k = m_hit_box.top / SIZE_TILE; k < (m_hit_box.top + m_hit_box.height) / SIZE_TILE; k++) {
            for (int i = m_hit_box.left / SIZE_TILE; i < (m_hit_box.left + m_hit_box.width) / SIZE_TILE; i++) {


                if (_map.level[k * MAP_WIDTH + i] == 3) {

                    if (_dir == Direction::Horixontal) {

                        if (speed_x > 0) {
                            m_hit_box.left = i * SIZE_TILE - m_hit_box.width;
                        }
                        if (speed_x < 0) {
                            m_hit_box.left = (i + 1) * SIZE_TILE;
                        }
                    }
                    else if (_dir == Direction::Vertical) {

                        if (speed_y > 0) {
                            m_hit_box.top = k * SIZE_TILE - m_hit_box.height;

                            on_ground = true;
                            speed_y = 0;
                        }
                        if (speed_y < 0) {
                            m_hit_box.top = (k + 1) * SIZE_TILE;

                            speed_y = 0;
                        }
                    }
                } // if (_map.level[k * MAP_WIDTH + i] == 3)

                if (_map.level[k * MAP_WIDTH + i] == 2) {
                    if (speed_y < 0) {
                        _map.upTile(k * MAP_WIDTH + i, 3);

                        m_hit_box.top = (k + 1) * SIZE_TILE;

                        speed_y = 0;
                    }
                    else {
                        if (_dir == Direction::Horixontal) {

                            if (speed_x > 0) {
                                m_hit_box.left = i * SIZE_TILE - m_hit_box.width;
                            }
                            if (speed_x < 0) {
                                m_hit_box.left = (i + 1) * SIZE_TILE;
                            }
                        }
                        else if (_dir == Direction::Vertical) {

                            if (speed_y > 0) {
                                m_hit_box.top = k * SIZE_TILE - m_hit_box.height;

                                on_ground = true;
                                speed_y = 0;
                            }
                        }
                    }
                } // if (_map.level[k * MAP_WIDTH + i] == 2)

            } // for (i ...)
        } // for (k ...)
    } // colisionMap

} //namespace ezg