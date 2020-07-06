#include "../Header/Player.h"
#include "../Header/Enemy.h"
#include "../Header/Bullets.h"

namespace ezg {


    void Hero::draw(sf::RenderTarget& target, sf::RenderStates states) {

        sf::Sprite _sprite = m_animation.getSprite();
        
        //expand the sprite if necessary
        if (m_direction == Direction::Left) {
            sf::IntRect _rec = _sprite.getTextureRect();
            
            _rec.left += _rec.width;
            _rec.width = -_rec.width;

            _sprite.setTextureRect(_rec);
        }

        //painted red if the hero is injured
        if (m_effect == EntityEffect::Wounded) {
            _sprite.setColor(sf::Color(220, 0, 0));
        }

        _sprite.setPosition(getPosX(), getPosY());

        target.draw(_sprite, states);
    }


    void Hero::checkKeyBoard() {
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) 
            && m_status == EntityStat::onStairs) {

            speed_y += -0.1f;

        }
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            && m_status == EntityStat::onStairs) {

            speed_y += 0.1f;

        }
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            && m_effect != EntityEffect::Wounded) {

            speed_x += -0.1f;
            m_direction = Direction::Left;
        }
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            && m_effect != EntityEffect::Wounded) {

            speed_x += 0.1f;
            m_direction = Direction::Right;
        }
    }


    void Hero::checkEvent(const sf::Event& _event) noexcept {

        switch (_event.type)
        {
        case sf::Event::KeyPressed:
            if (_event.key.code == sf::Keyboard::W || _event.key.code == sf::Keyboard::Up) {
                jump();
            }
            else if (_event.key.code == sf::Keyboard::S || _event.key.code == sf::Keyboard::Down) {
                jumpOff();
            }
            else if (_event.key.code == sf::Keyboard::Space) {
                if (m_status == EntityStat::onStairs) {
                    jumpOff();
                }
                else {
                    jump();
                }
            }

            break;

        }
    }


    void Hero::jump() noexcept {

        if (m_status == EntityStat::onSolid || m_status == EntityStat::onSolidAbove) {

            speed_y = -0.165f;
            setStat(EntityStat::InAir);
        }

    }


    void Hero::jumpOff() noexcept {

        if (m_status == EntityStat::onSolidAbove || m_status == EntityStat::onStairs) {

            move(0.f, 1.f);
            setStat(EntityStat::InAir);
        }

    }


    std::unique_ptr<Entity> Hero::fire() noexcept {

        std::unique_ptr<HeroBullet> button(std::make_unique<HeroBullet>());

        float _y = m_hit_box.top + 4;
        if (m_direction == Direction::Left) {
            button->setPosition(m_hit_box.left - 1, _y);
            button->setSpeed(-0.11f, 0);
        }
        else {
            button->setPosition(m_hit_box.left + 8, _y);
            button->setSpeed(0.11f, 0);
        }

        return button;
    }


    void Hero::getHit(float _damage) noexcept {
        if (m_effect != EntityEffect::Wounded && _damage > 0.f) {
            m_hp -= _damage;
            m_effect = EntityEffect::Wounded;
        }
    }


    void Hero::upPosition(float time, Direction _dir) noexcept {

        if (_dir == Direction::Vertical) {

            if (is_gravity && m_status == EntityStat::InAir) {

                speed_y += 0.0005f * time;
                move(0, speed_y * time);
            }
            if (!is_gravity || m_status == EntityStat::onStairs) {

                move(0, speed_y * time);
            }
        }
        else if (_dir == Direction::Horixontal) {

            move(speed_x * time, 0);

            if (is_gravity && speed_x != 0 && m_status != EntityStat::onStairs) {

                m_status = EntityStat::InAir;
            }
        }

    }


    void Hero::upEffect(float _time) noexcept {

        static float _duration = 0.f;

        if (m_effect == EntityEffect::Wounded) {

            _duration += _time;

            if (_duration > DURATION_WOUNDED) {

                m_effect = EntityEffect::Normal;
                _duration = 0.f;
            }

            if (m_effect == EntityEffect::Wounded) {

                if (m_direction == Direction::Left) {

                    speed_x = 0.1f - _duration / DURATION_WOUNDED / 10.f;
                }
                else {
                    speed_x = -0.1 + _duration / DURATION_WOUNDED / 10.f;
                }
            }

        }
        else {
            _duration = 0.f;
        }
    }


    void Hero::otherUpdate(float _time) {

        if (speed_x == 0.f && speed_y == 0.f && m_animation.getActive() != EntityAnimation::Idle) {
            m_animation.activate(EntityAnimation::Idle);
        }

        speed_x = 0;

        upEffect(_time);

        m_animation.update(_time);

        if (m_hp <= 0) {
            setStat(EntityStat::Death);
        }

    }


    void Hero::setStat(EntityStat _new) {

        if (_new == EntityStat::InAir) {

            if (m_animation.getActive() != EntityAnimation::Jump) {

                m_animation.activate(EntityAnimation::Jump);
            }
        }
        else if (_new == EntityStat::onSolid || _new == EntityStat::onSolidAbove) {

            if (m_animation.getActive() != EntityAnimation::Walk) {

                m_animation.activate(EntityAnimation::Walk);
            }
        }
        else if (_new == EntityStat::onStairs) {

            if (m_animation.getActive() != EntityAnimation::Walk) {

                m_animation.activate(EntityAnimation::Walk);
            }
        }

        m_status = _new;
    }

    
    std::unique_ptr<Entity> Hero::colision(Entity* const _entity, Direction _dir) {

        if (_entity == nullptr) {
            return nullptr;
        }

        std::unique_ptr<Entity> result = nullptr;
        if (m_hit_box.intersects(_entity->getHitBox())) {

            switch (_entity->getTipe())
            {
/////////////////////////////Solid
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

                        setStat(EntityStat::onSolid);
                        speed_y = 0;
                    }
                    if (speed_y < 0) {
                        m_hit_box.top = _entity->getPosY() + m_hit_box.height;

                        speed_y = 0;
                    }
                }


                break;
            } // case Solid


/////////////////////////////SolidAbove
            case TipeEntity::SolidAbove: {

                if (_dir == Direction::Vertical) {

                    if (speed_y > 0 && std::fabs(_entity->getPosY() - (m_hit_box.top + m_hit_box.height)) < 1) {
                        m_hit_box.top = _entity->getPosY() - m_hit_box.height;

                        speed_y = 0;

                        setStat(EntityStat::onSolidAbove);
                    }

                }
                break;
            } // case SolidAbove
                

/////////////////////////////Stairs
            case TipeEntity::Stairs: {

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    setStat(EntityStat::onStairs);
                }

                if (m_status == EntityStat::onStairs) {

                    float center_x = m_hit_box.left + m_hit_box.width / 2;
                    float center_y = m_hit_box.top + m_hit_box.height / 2;

                    if (!_entity->getHitBox().contains(center_x, center_y)) {
                        setStat(EntityStat::InAir);
                    }
                    else if (_dir == Direction::Vertical) {
                        speed_y = 0;
                    }
                }
                break;
            } // case Stairs


/////////////////////////////Needle
            case TipeEntity::Needle: {
                if (m_effect != EntityEffect::Wounded) {

                    //is it safe?!?
                    m_hp -= dynamic_cast<Needle*>(_entity)->getDamage();
                    m_effect = EntityEffect::Wounded;

                    if (speed_y >= 0) {
                        speed_y = -0.12;
                    }
                    else {
                        speed_y = 0.1;
                    }

                }
                break; // case Needle


/////////////////////////////RedBullet
            case TipeEntity::RedBullet: {
                if (m_effect != EntityEffect::Wounded) {

                    //is it safe?!?
                    m_hp -= dynamic_cast<RedBullet*>(_entity)->getDamage();
                    m_effect = EntityEffect::Wounded;

                    if (speed_y >= 0) {
                        speed_y = -0.12;
                    }
                    else {
                        speed_y = 0.1;
                    }

                }

                break;
            }
            }


            default:
                break;
            }
           
        }
        if (_entity->getTipe() == TipeEntity::MushroomRed && _dir == Direction::Horixontal) {
            MushroomRed* mr = dynamic_cast<MushroomRed*>(_entity);
            if (m_hit_box.intersects(mr->getArea())) {
                result = mr->fire(getPosX(), getPosY());
            }
            else {
                mr->setStat(EntityAnimation::Idle);
            }
        }
        else if (_entity->getTipe() == TipeEntity::Bee && _dir == Direction::Horixontal) {
            Bee* bee = dynamic_cast<Bee*>(_entity);

            getHit(bee->attack(m_hit_box));
        }

        return result;

    } // Hero::colision


    void Hero::restart() {

        setStat(EntityStat::InAir);

        m_effect = EntityEffect::Normal;
        m_direction = Direction::Right;
        is_gravity = true;

        speed_x = speed_y = 0;

        setPosition(0, 0);
        m_hp = 0;
    }
} //namespace ezg