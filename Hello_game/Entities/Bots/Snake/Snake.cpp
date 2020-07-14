#include "Snake.h"

#include "../../Bullets/Bullets.h"

namespace ezg {

#define MAX_SPEED_SNAKE 0.04f

	Snake::Snake(float place_x, float place_y, float _distance_attack, const sf::Texture& _texture)
		: Entity(TypeEntity::Snake, place_x, place_y, 8, 8)
		, m_direction(Direction::Right)
		, m_damage(40)
		, m_time_effect(0)
		, m_hp(40)
		, m_goto(0, 0)
		, speed_x(0)
		, speed_y(0)
		, m_accelerationx(0.0001f)
		, m_distance_attack(_distance_attack)
		, m_effect(EffectType::Walking)
		, m_status(EntityStat::InAir)
	{
		is_gravity = true;

		m_animation.setTexture(_texture);
		_setAnimations_();
		m_animation.activate(static_cast<int>(EntityAnimation::Walk));

		m_goto.x = std::rand() % 80 + m_hit_box.left - 40;
		//m_goto.y = std::rand() % static_cast<int>(m_area.height) + static_cast<int>(m_area.top);
	}


	void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const noexcept /*override*/ {

		sf::Sprite _sprite = m_animation.getSprite();

		//expand the sprite if necessary
		if (m_direction == Direction::Left) {
			sf::IntRect _rec = _sprite.getTextureRect();

			_rec.left += _rec.width;
			_rec.width = -_rec.width;

			_sprite.setTextureRect(_rec);
		}

		//painted Red if the hero is injured
		if (m_effect == EffectType::Wounded) {
			_sprite.setColor(sf::Color::Red);
		}

		_sprite.setPosition(getPosX(), getPosY());
		target.draw(_sprite, states);
	}


	Hit Snake::attack(sf::FloatRect _rec) {

		sf::CircleShape _circle(m_distance_attack);
		if (m_effect == EffectType::Attack) {
			_circle.setRadius(m_distance_attack * 1.7f);
		}
		_circle.setPosition(m_hit_box.left + m_hit_box.width / 2, m_hit_box.top + m_hit_box.height / 2);

		const float _x = _rec.left;
		const float _y = _rec.top;
		//std::cout << (int)m_effect << std::endl;
		if (Entity::intersection(_circle, sf::Vector2f(_x, _y)) && m_effect != EffectType::Wounded) {
			setEffect(EffectType::Attack);

			//_goto_(_x, _y);
			m_goto.x = _x;
			m_goto.y = _y;

			if (_rec.intersects(sf::FloatRect(m_hit_box.left - 3, m_hit_box.top, m_hit_box.width + 6, m_hit_box.height))) {

				return Hit{ m_damage, Effect{EffectType::Poisoning, 10.f, 3000.f} };
			}
		}
		else if (m_effect != EffectType::Wounded && m_effect != EffectType::Walking) {
			setEffect(EffectType::Normal);
		}

		return Hit();
	}


	void Snake::upPosition(float time, Direction _dir) noexcept {

		if (_dir == Direction::Vertical) {

			if (is_gravity && m_status == EntityStat::InAir) {

				speed_y += 0.0005f * time;
				moveIt(0, speed_y * time);
			}
			if (!is_gravity || m_status == EntityStat::onStairs) {

				moveIt(0, speed_y * time);
			}
		}
		else if (_dir == Direction::Horixontal) {

			if (m_effect == EffectType::Walking) {
				_goto_(m_goto.x, m_goto.y, time);

				if (m_goto.x > m_hit_box.left && m_goto.x < m_hit_box.left + m_hit_box.width) {
					setEffect(EffectType::Normal);

					//m_goto.x = std::rand() % 80 + m_hit_box.left - 40;
				}
			}
			else if (m_effect == EffectType::Attack) {

				_goto_(m_goto.x, m_goto.y, time);

			}
			else if (m_effect == EffectType::Normal) {
				_goto_(m_hit_box.left, m_hit_box.top, time);

			}

			moveIt(speed_x * time, 0);

			if (is_gravity && speed_x != 0 && m_status != EntityStat::onStairs) {

				m_status = EntityStat::InAir;
			}
		}

	}


	void Snake::colision(gsl::not_null <Entity*> _lhs, Direction _dir) /*final override*/ {

		if (_lhs->getType() == TypeEntity::Solid) {

			if (getHitBox().intersects(_lhs->getHitBox())) {

				if (_dir == Direction::Horixontal) {

					if (speed_x > 0) {
						m_hit_box.left = _lhs->getPosX() - m_hit_box.width;
						Stop();
					}
					if (speed_x < 0) {
						m_hit_box.left = _lhs->getPosX() + _lhs->getHitBox().width;
						Stop();
					}
				}

				else if (_dir == Direction::Vertical) {

					if (speed_y > 0) {
						m_hit_box.top = _lhs->getPosY() - m_hit_box.height;

						setStat(EntityStat::onSolid);
						speed_y = 0;
					}
					if (speed_y < 0) {
						m_hit_box.top = _lhs->getPosY() + m_hit_box.height;

						speed_y = 0;
					}
				}

			}
		}
		else if (_lhs->getType() == TypeEntity::SolidAbove) {

			if (m_hit_box.intersects(_lhs->getHitBox())) {
				if (_dir == Direction::Vertical) {

					if (speed_y > 0 && std::fabs(_lhs->getPosY() - (m_hit_box.top + m_hit_box.height)) < 1) {
						m_hit_box.top = _lhs->getPosY() - m_hit_box.height;

						speed_y = 0;

						setStat(EntityStat::onSolidAbove);
					}

				}
			}

		}
		else if (_lhs->getType() == TypeEntity::HeroBullet) {

			if (m_hit_box.intersects(_lhs->getHitBox())) {
				gsl::not_null<Bullet*> bl = dynamic_cast<Bullet*>(_lhs.get());
				getHit(bl->getHit());
			}
		}

	}


	void Snake::jump() {

		if (m_rallback_jump > SNAKE_RALLBACK_JUMP) {
			if (m_status == EntityStat::onSolid || m_status == EntityStat::onSolidAbove) {
				speed_y = -0.14f;
				setStat(EntityStat::InAir);
				m_rallback_jump = 0.f;
			}
		}

	}


	void Snake::jumpOff() {

		if (m_status == EntityStat::onSolidAbove) {
			moveIt(0.f, 1.f);
			setStat(EntityStat::InAir);
		}

	}


	void Snake::upEffect(float _time) noexcept {


		if (m_effect == EffectType::Wounded) {

			m_time_effect += _time;

			if (m_time_effect > SNAKE_DURATION_WOUNDED) {

				setEffect(EffectType::Normal);
				m_time_effect = _time = 0.f;
			}

			if (m_effect == EffectType::Wounded) {

				if (m_direction == Direction::Left) {

					speed_x = 0.1f - m_time_effect / SNAKE_DURATION_WOUNDED / 10.f;
				}
				else {
					speed_x = -0.1 + m_time_effect / SNAKE_DURATION_WOUNDED / 10.f;
				}
			}

		}
		else if (m_effect == EffectType::Normal) {

			m_time_effect += _time;

			if (m_time_effect > 1000.f) {

				m_goto.x = std::rand() % 80 + m_hit_box.left - 40;
				m_goto.y = m_hit_box.top;

				setEffect(EffectType::Walking);
				m_time_effect = _time = 0.f;
			}

		}
		else {
			m_time_effect = 0.f;
		}
		//printf("%g %d\n", m_time_effect, (int)m_effect);
	}


	void Snake::otherUpdate(float _time) {

		if (speed_x == 0.f && speed_y == 0.f && m_animation.getActive() != static_cast<int>(EntityAnimation::Idle)) {
			m_animation.activate(static_cast<int>(EntityAnimation::Idle));
		}
		if (speed_x > 0) {
			m_direction = Direction::Right;
		}
		else if (speed_x < 0) {
			m_direction = Direction::Left;
		}

		m_rallback_jump += _time;

		//speed_x = 0;

		upEffect(_time);

		m_animation.update(_time);

		if (m_hp <= 0) {
			m_alive = false;
		}

	}


	void Snake::getHit(Hit _hit) noexcept {
		if (m_effect != EffectType::Wounded && _hit._damage > 0.f) {
			m_hp -= _hit._damage;
			speed_y = -0.1f;
			m_effect = EffectType::Wounded;
		}
	}


	void Snake::setEffect(EffectType _new_effect) {


		if (_new_effect != m_effect) {
			m_time_effect = 0.f;
			m_effect = _new_effect;
		}

	}


	void Snake::setStat(EntityStat _new) {

		//if (_new == EntityStat::InAir) {

		//	if (m_animation.getActive() != EntityAnimation::Jump) {

		//		m_animation.activate(EntityAnimation::Jump);
		//	}
		//}
		if (_new == EntityStat::onSolid || _new == EntityStat::onSolidAbove) {

			if (m_animation.getActive() != static_cast<int>(EntityAnimation::Walk)) {

				m_animation.activate(static_cast<int>(EntityAnimation::Walk));
			}
		}
		//else if (_new == EntityStat::onStairs) {

		//	if (m_animation.getActive() != EntityAnimation::Walk) {

		//		m_animation.activate(EntityAnimation::Walk);
		//	}
		//}

		m_status = _new;
	}


	void Snake::_goto_(float _x, float _y, float _time) {

		if (_x > m_hit_box.left) {
			speed_x += m_accelerationx * _time;

			if (speed_x > MAX_SPEED_SNAKE) {
				speed_x = MAX_SPEED_SNAKE;
			}
		}
		else if (_x < m_hit_box.left) {
			speed_x -= m_accelerationx * _time;

			if (speed_x < -MAX_SPEED_SNAKE) {
				speed_x = -MAX_SPEED_SNAKE;
			}
		}
		else {
			if (speed_x > 0) {
				speed_x -= m_accelerationx * _time;
				speed_x = (speed_x >= 0) ? speed_x : 0;
			}
			else if (speed_x < 0) {
				speed_x += m_accelerationx * _time;
				speed_x = (speed_x <= 0) ? speed_x : 0;
			}
		}


		if (_y < m_hit_box.top - 8) {
			jump();
		}
		else if (_y > m_hit_box.top + 8) {
			jumpOff();
		}

	}


	void Snake::_setAnimations_() {

		m_animation.addAnimation(static_cast<int>(EntityAnimation::Idle));
		m_animation.addFrame(static_cast<int>(EntityAnimation::Idle), sf::IntRect(0, 40, 8, 8), 200.f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Idle), sf::IntRect(8, 40, 8, 8), 200.f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Idle), sf::IntRect(16, 40, 8, 8), 200.f);

		m_animation.addAnimation(static_cast<int>(EntityAnimation::Walk));
		m_animation.addFrame(static_cast<int>(EntityAnimation::Walk), sf::IntRect(32, 40, 8, 8), 160.f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Walk), sf::IntRect(40, 40, 8, 8), 160.f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Walk), sf::IntRect(48, 40, 8, 8), 160.f);

		m_animation.addAnimation(static_cast<int>(EntityAnimation::Attack));
		m_animation.addFrame(static_cast<int>(EntityAnimation::Attack), sf::IntRect(88, 40, 8, 8), 100.f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Attack), sf::IntRect(96, 40, 8, 8), 100.f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Attack), sf::IntRect(104, 40, 8, 8), 100.f);

	}

} // namespace ezg