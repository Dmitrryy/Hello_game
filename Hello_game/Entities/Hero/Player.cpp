#include "Player.h"


#include "../Bullets/Bullets.h"
#include "../Bots/Mushroom/Mushroom.h"
#include "../Bots/Bee/Bee.h"
#include "../Bots/Snake/Snake.h"
#include "../Traps/Needle/Needle.h"

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
        if (_effectIsActive_(EffectType::Wounded)) {
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
            && !_effectIsActive_(EffectType::Wounded)) {

            speed_x += -0.1f;
            m_direction = Direction::Left;
        }
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            && !_effectIsActive_(EffectType::Wounded)) {

            speed_x += 0.1f;
            m_direction = Direction::Right;
        }
    }


    void Hero::checkEvent(const sf::Event& _event) {

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


    void Hero::jump() {

        if (m_status == EntityStat::onSolid || m_status == EntityStat::onSolidAbove) {

            speed_y = -0.14f;
            setStat(EntityStat::InAir);
        }

    }


    void Hero::jumpOff() {

        if (m_status == EntityStat::onSolidAbove || m_status == EntityStat::onStairs) {

            move(0.f, 1.5f);
            setStat(EntityStat::InAir);
        }

    }


    std::unique_ptr<Entity> Hero::fire() {

        std::unique_ptr<HeroBullet> button(std::make_unique<HeroBullet>());

        //according to the sprite
        const float _y = m_hit_box.top + 4;
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


    void Hero::getHit(Hit _hit) {

        if (!_effectIsActive_(EffectType::Wounded)) {
            
            if (_hit._damage > 0.f) {
                m_hp -= _hit._damage;

                speed_y = -0.1f;
                setStat(EntityStat::InAir);

                m_effects[EffectType::Wounded]._time_effect = 200.f;
                m_effects[EffectType::Wounded]._power = 1 + _hit._damage / 2000.f;
            }

            m_effects[_hit._effect._type] = _hit._effect;
        }
    }


    void Hero::upPosition(float time, Direction _dir) noexcept {

        if (_effectIsActive_(EffectType::Freezing)) {
            time /= 1.5;
        }
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


    void Hero::upEffect(float _time) {

        for (auto& ef : m_effects) {
            if (ef.second._time_effect > 0.f) {

                ef.second._time_effect -= _time;
            }
            else {
                ef.second._time_effect = 0;
            }
        }

        if (_effectIsActive_(EffectType::Wounded)) {

            if (m_direction == Direction::Left) {

                speed_x = 0.1f * m_effects.at(EffectType::Wounded)._power;
            }
            else {
                speed_x = -0.1f * m_effects.at(EffectType::Wounded)._power;
            }
        }
        else {

           /* if (_effectIsActive_(EffectType::Freezing)) {

                speed_x /= 1.5;
                speed_y /= 1.5;

            }*/
            if (_effectIsActive_(EffectType::OnFire) && !_effectIsActive_(EffectType::Immunity)) {

                getHit(Hit{ 5 * m_effects[EffectType::OnFire]._power, Effect{EffectType::Immunity, 1.f, 900.f} });
            }
            if (_effectIsActive_(EffectType::Poisoning) && !_effectIsActive_(EffectType::Immunity)) {

                getHit(Hit{ 5 * m_effects[EffectType::Poisoning]._power, Effect{EffectType::Immunity, 1.f, 900.f} });
            }
        }

    }


    void Hero::otherUpdate(float _time) {

        if (speed_x == 0.f && speed_y == 0.f && m_animation.getActive() != static_cast<int>(EntityAnimation::Idle)) {
            m_animation.activate(static_cast<int>(EntityAnimation::Idle));
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

            if (m_animation.getActive() != static_cast<int>(EntityAnimation::Jump)) {

                m_animation.activate(static_cast<int>(EntityAnimation::Jump));
            }
        }
        else if (_new == EntityStat::onSolid || _new == EntityStat::onSolidAbove) {

            if (m_animation.getActive() != static_cast<int>(EntityAnimation::Walk)) {

                m_animation.activate(static_cast<int>(EntityAnimation::Walk));
            }
        }
        else if (_new == EntityStat::onStairs) {

            if (m_animation.getActive() != static_cast<int>(EntityAnimation::Walk)) {

                m_animation.activate(static_cast<int>(EntityAnimation::Walk));
            }
        }

        m_status = _new;
    }


    
    std::unique_ptr<Entity> 
    Hero::colision(Entity* const _entity, Direction _dir) {

        if (_entity == nullptr) {
            return nullptr;
        }

        std::unique_ptr<Entity> result = nullptr;
        if (m_hit_box.intersects(_entity->getHitBox())) {

            switch (_entity->getType())
            {
/////////////////////////////Solid
            case TypeEntity::Solid: {

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
            case TypeEntity::SolidAbove: {

                if (_dir == Direction::Vertical) {

                    if (speed_y > 0 && 
                        (std::fabs(_entity->getPosY() - (m_hit_box.top + m_hit_box.height)) < 1.5f) || speed_y > 0.1f) 
                    {
                        m_hit_box.top = _entity->getPosY() - m_hit_box.height;

                        speed_y = 0;

                        setStat(EntityStat::onSolidAbove);
                    }

                }
                break;
            } // case SolidAbove
                

/////////////////////////////Stairs
            case TypeEntity::Stairs: {

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    setStat(EntityStat::onStairs);
                }

                if (m_status == EntityStat::onStairs) {

                    const float center_x = m_hit_box.left + m_hit_box.width / 2;
                    const float center_y = m_hit_box.top + m_hit_box.height / 2;

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
            case TypeEntity::Needle: 
            {
                //is it safe?!?
                const gsl::not_null<Needle*> bl = dynamic_cast<Needle*>(_entity);
                getHit(bl->getDamage());
                break; // case Needle

            }
/////////////////////////////RedBullet
            case TypeEntity::RedBullet:
            case TypeEntity::BlueBullet: 
            {
                //is it safe?!?
                const gsl::not_null<Bullet*> bl = dynamic_cast<Bullet*>(_entity);
                getHit(bl->getHit());
                break;
            
            }


            default:
                break;
            }
           
        }
        if ((_entity->getType() == TypeEntity::MushroomRed || _entity->getType() == TypeEntity::MushroomBlue) 
            && _dir == Direction::Horixontal) 
        {
            const gsl::not_null<Mushroom*> mr = dynamic_cast<Mushroom*>(_entity);
            if (m_hit_box.intersects(mr->getArea())) {
                result = mr->fire(getPosX(), getPosY());
            }
            else {
                mr->setStat(EntityAnimation::Idle);
            }
        }
        else if (_entity->getType() == TypeEntity::Bee && _dir == Direction::Horixontal) 
        {
            const gsl::not_null<Bee*> bee = dynamic_cast<Bee*>(_entity);

            getHit(bee->attack(m_hit_box));
        }
        else if (_entity->getType() == TypeEntity::Snake && _dir == Direction::Horixontal) 
        {
            const gsl::not_null<Snake*> snake = dynamic_cast<Snake*>(_entity);

            getHit(snake->attack(m_hit_box));
        }

        return result;

    } // Hero::colision


    void Hero::restart() {

        setStat(EntityStat::InAir);

        for (auto& ef : m_effects) {
            ef.second._power = ef.second._time_effect = 0.f;
        }

        m_direction = Direction::Right;
        is_gravity = true;

        speed_x = speed_y = 0;

        setPosition(0, 0);
        m_hp = 0;
    }


    bool Hero::_effectIsActive_(EffectType _eff) noexcept {

        auto res = m_effects.find(_eff);

        if (res == m_effects.end()) {
            return false;
        }
        return res->second._time_effect > 0.f;
    }

} //namespace ezg