#include <iomanip>

#include "Player.h"


#include "../Bullets/Bullets.h"
#include "../Bots/Mushroom/Mushroom.h"
#include "../Bots/Bee/Bee.h"
#include "../Bots/Snake/Snake.h"
#include "../Traps/Needle/Needle.h"

namespace ezg {


    void Hero::draw(sf::RenderTarget& target, sf::RenderStates states) const {

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

            speed_y += -80.f;

        }
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            && m_status == EntityStat::onStairs) {

            speed_y += 80.f;

        }
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))) {

            speed_x += -80.f;
            m_direction = Direction::Left;
        }
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))) {

            speed_x += 80.f;
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

            speed_y = -115.f;
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
            button->setSpeed(-88.f, 0);
        }
        else {
            button->setPosition(m_hit_box.left + 8, _y);
            button->setSpeed(88.f, 0);
        }

        return button;
    }


    void Hero::getHit(Hit _hit) {

        if (!_effectIsActive_(EffectType::Wounded)) {
            
            if (_hit._damage > 0.f) {
                m_hp -= _hit._damage;

                speed_y = -80.f;
                setStat(EntityStat::InAir);

                m_effects[EffectType::Wounded]._time_effect = 0.15f;
                m_effects[EffectType::Wounded]._power = 1 + _hit._damage / 2000.f;
            }

            for (int i = 0; i < 4; i++) {
                m_effects[_hit._effect[i]._type] = _hit._effect[i];
            }

        }
    }


    void Hero::upPosition(float time, Direction _dir) noexcept {

        //if (_effectIsActive_(EffectType::Freezing)) {
        //   speed_x /= 1.5;
        //}
        if (_dir == Direction::Vertical) {

            if (is_gravity && m_status == EntityStat::InAir) {

                speed_y += acceleration_of_gravity * time;
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


#define _eff_ eff.second
    void Hero::upEffect(float _time) {

        for (auto& eff : m_effects) {
            if (eff.second._time_effect > 0.f) {
                
                if (_eff_._type == EffectType::Discarding) {
                    speed_x += _eff_._power * std::cos(_eff_._property);
                    speed_y = _eff_._power * std::sin(_eff_._property);
                }
                else if (_eff_._type == EffectType::Freezing) {
                    speed_x /= _eff_._power;
                }

                _eff_._time_effect -= _time;
            }
            else {

                if (_eff_._type == EffectType::Poisoning && static_cast<int>(_eff_._property) > 0) {
                    getHit(Hit{_eff_._power});

                    _eff_._property--;
                    _eff_._time_effect = 1.f;
                }
                else if (_eff_._type == EffectType::OnFire && static_cast<int>(_eff_._property) > 0) {
                    getHit(Hit{ _eff_._power });

                    _eff_._property--;
                    _eff_._time_effect = 1.f;
                }

            }
        }

    }


    void Hero::otherUpdate(float _time) {

        if (speed_x == 0.f && speed_y == 0.f && m_animation.getActive() != static_cast<int>(EntityAnimation::Idle)) {
            m_animation.activate(static_cast<int>(EntityAnimation::Idle));
        }

        //upEffect(_time);

        speed_x = 0;

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
            case EntityType::Solid: {

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
            case EntityType::SolidAbove: {

                if (_dir == Direction::Vertical) {

                    if (speed_y > 0 && 
                        (std::fabs(_entity->getPosY() - (m_hit_box.top + m_hit_box.height)) < 1.5f) || speed_y > 100.f) 
                    {
                        m_hit_box.top = _entity->getPosY() - m_hit_box.height;

                        speed_y = 0;

                        setStat(EntityStat::onSolidAbove);
                    }

                }
                break;
            } // case SolidAbove
                

/////////////////////////////Stairs
            case EntityType::Stairs: {

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
            case EntityType::Needle: 
            {
                //is it safe?!?
                const gsl::not_null<Needle*> bl = dynamic_cast<Needle*>(_entity);
                getHit(bl->getDamage());
                break; // case Needle

            }
/////////////////////////////RedBullet
            case EntityType::RedBullet:
            case EntityType::BlueBullet: 
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
        if ((_entity->getType() == EntityType::MushroomRed || _entity->getType() == EntityType::MushroomBlue) 
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
        else if (_entity->getType() == EntityType::Bee && _dir == Direction::Horixontal) 
        {
            const gsl::not_null<Bee*> bee = dynamic_cast<Bee*>(_entity);

            getHit(bee->attack(m_hit_box));
        }
        else if (_entity->getType() == EntityType::Snake && _dir == Direction::Horixontal) 
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


    bool Hero::_effectIsActive_(EffectType _eff) const noexcept {

        auto res = m_effects.find(_eff);

        if (res == m_effects.end()) {
            return false;
        }
        return res->second._time_effect > 0.f;
    }


    std::string Hero::debugString() {

        using std::setw;
        using std::endl;
        using std::setfill;

        std::ostringstream out;
        out.setf(std::ios::left | std::ios::boolalpha);

        out << setw(13) << setfill('\t') << "Type" << "Hero" << endl
            << setw(12) << "Hp" << m_hp << endl
            << setw(14) << "status" << "  " << m_status << endl
            << setw(18) << "is gravity " << is_gravity << endl
            << setw(17) << "coordinates" << "(" << m_hit_box.left << ", " << m_hit_box.top << ")\n"
            << setw(13) << "size" << "  w: " << m_hit_box.width << ", h: " << m_hit_box.height << '\n'
            << setw(13) << "speed" << "  (" << speed_x << ", " << speed_y << ")\n"
            << "effects:" << endl;
        {   //effects
            bool is_one = false;
            for (const auto& eff : m_effects) {
                if (eff.second._time_effect > 0.f) {
                    is_one = true;
                    out << "    " << eff.first << "(time: " << std::setprecision(4) << eff.second._time_effect
                        << " power: " << eff.second._power
                        << " property: " << eff.second._property << ')' << endl;
                }
            }
            if (!is_one) {
                out << "    nop" << std::endl;
            }
        }
        //std::cout << out.str().size() << endl;
        return out.str();
    }

} //namespace ezg