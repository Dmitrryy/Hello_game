#include "../Header/Player.h"


namespace ezg {

    void 
    Hero::load(sf::Texture& texture, int texture_loc_x, int texture_loc_y, float pos_x, float pos_y, float _width, float _height) {

        m_hit_box = sf::FloatRect(pos_x, pos_y, _width, _height);
        speed_x = (0);
        speed_y = (0);
        on_ground = (false);
        frame_stat = (Stat::Stay);


        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(texture_loc_x, texture_loc_y, _width, _height));
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
    Hero::colision(ezg::Entity* _entity, Direction _dir) /* override */ {

        if (m_hit_box.intersects(_entity->m_hit_box)) {

            switch (_entity->m_tipe)
            {

            case TipeEntity::Solid: {

                if (_dir == Direction::Horixontal) {

                    if (speed_x > 0) {
                        m_hit_box.left = _entity->m_hit_box.left - m_hit_box.width;
                    }
                    if (speed_x < 0) {
                        m_hit_box.left = _entity->m_hit_box.left + _entity->m_hit_box.width;
                    }
                }

                else if (_dir == Direction::Vertical) {

                    if (speed_y > 0) {
                        m_hit_box.top = _entity->m_hit_box.top - m_hit_box.height;

                        on_ground = true;
                        speed_y = 0;
                    }
                    if (speed_y < 0) {
                        m_hit_box.top = _entity->m_hit_box.top + m_hit_box.height;

                        speed_y = 0;
                    }
                }

                break;
            } // case Solid

            default:
                break;
            }
        }
    } // Hero::colision

} //namespace ezg