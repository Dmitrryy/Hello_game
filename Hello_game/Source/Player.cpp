#include "../Header/Player.h"


namespace ezg {


    void 
    Hero::draw(sf::RenderTarget& target, sf::RenderStates states) const /* override */ {

        target.draw(m_sprite, states);

    }


    void Hero::jump() noexcept {

        if (on_ground) {
            setSpeedY(-0.25);
            on_ground = false;
        }

    }

    
    void 
    Hero::colision(gsl::not_null <Entity*> _entity, Direction _dir) /* override */ {

        if (m_hit_box.intersects(_entity->getHitBox())) {

            switch (_entity->getTipe())
            {

            case TipeEntity::Solid: {

                if (_dir == Direction::Horixontal) {

                    if (speed_x > 0) {
                        m_hit_box.left = _entity->getPosX() - m_hit_box.width;
                    }
                    if (speed_x < 0) {
                        m_hit_box.left = _entity->getPosX() + _entity->getHitBox().width;
                    }
                }

                else if (_dir == Direction::Vertical) {

                    if (speed_y > 0) {
                        m_hit_box.top = _entity->getPosY() - m_hit_box.height;

                        on_ground = true;
                        speed_y = 0;
                    }
                    if (speed_y < 0) {
                        m_hit_box.top = _entity->getPosY() + m_hit_box.height;

                        speed_y = 0;
                    }
                }

                break;
            } // case Solid

            case TipeEntity::SolidAbove: {

                if (_dir == Direction::Vertical) {

                    if (speed_y > 0 && std::fabs(_entity->getPosY() - (m_hit_box.top + m_hit_box.height)) < 1) {
                        m_hit_box.top = _entity->getPosY() - m_hit_box.height;

                        on_ground = true;
                        speed_y = 0;
                    }

                }

            } // case SolidAbove

            default:
                break;
            }
        }
    } // Hero::colision


} //namespace ezg