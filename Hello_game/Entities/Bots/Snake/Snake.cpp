#include <iomanip>

#include "Snake.h"

#include "../../Bullets/Bullets.h"

namespace ezg {

#define MAX_SPEED_SNAKE 55.f

	Snake::Snake(float place_x, float place_y, float _distance_attack, const sf::Texture& _texture)
		: Entity(EntityType::Snake, place_x, place_y, 8, 8)
		, m_direction(Direction::Right)
		, m_damage(40)
		, m_time_effect(0)
		, m_hp(4000)
		, m_goto(0, 0)
		, speed_x(0)
		, speed_y(0)
		, m_accelerationx(80.f)
		, m_distance_attack(_distance_attack)
		, m_status(EntityStat::InAir)
	{
		is_gravity = true;

		m_animation.setTexture(_texture);
		_setAnimations_();
		m_animation.activate(static_cast<int>(EntityAnimation::Walk));

		m_goto.x = std::rand() % 100 + m_hit_box.left - 50;
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
		if (_effectIsActive_(EffectType::Wounded)) {
			_sprite.setColor(sf::Color::Red);
		}

		_sprite.setPosition(getPosX(), getPosY());
		target.draw(_sprite, states);
	}


	Hit Snake::attack(sf::FloatRect _rec) {

		sf::CircleShape _circle(countRadius());
		_circle.setPosition(m_hit_box.left + m_hit_box.width / 2, m_hit_box.top + m_hit_box.height / 2);

		const float _x = _rec.left;
		const float _y = _rec.top;

		if (Entity::intersection(_circle, sf::Vector2f(_x, _y))) {
			setEffect(Effect(EffectType::Aggression, 0.f, 1.8f, 4.f));
			setEffect(Effect(EffectType::Attack, 0.f, 0.f, 1.5f));

			//_goto_(_x, _y);
			m_goto.x = _x;
			m_goto.y = _y;

			if (_rec.intersects(sf::FloatRect(m_hit_box.left - 3, m_hit_box.top, m_hit_box.width + 6, m_hit_box.height))) {
				const float diff_x = _x - m_hit_box.left;
				const float diff_y = _y - m_hit_box.top;


				float m_corner = std::atan(diff_y / diff_x);

				if (diff_y < 0.f && diff_x < 0.f) {
					m_corner -= 3.141592;
				}
				else if (diff_x < 0.f && diff_y > 0.f) {
					m_corner = 3.141592 + m_corner;
				}
				return Hit{ m_damage, Effect{EffectType::Discarding, m_corner, 100.f, 0.15f}, 
									  Effect{EffectType::Poisoning, 5, 10.f, 2.f} };
			}
		}

		return Hit();
	}


	void Snake::upPosition(float time, Direction _dir) noexcept {

		if (_dir == Direction::Vertical) {

			if (is_gravity && m_status == EntityStat::InAir) {

				speed_y += acceleration_of_gravity * time;

				m_hit_box.top += countSpeed().y * time;
			}
			if (!is_gravity || m_status == EntityStat::onStairs) {

				m_hit_box.top += countSpeed().y * time;
			}
		}
		else if (_dir == Direction::Horixontal) {

			_goto_(m_goto.x, m_goto.y, time);

			m_hit_box.left += countSpeed().x * time;

			if (is_gravity && speed_x != 0 && m_status != EntityStat::onStairs) {

				m_status = EntityStat::InAir;
			}
		}

	}


	void Snake::colision(gsl::not_null <Entity*> _lhs, Direction _dir) /*final override*/ {

		if (_lhs->getType() == EntityType::Solid) {

			if (getHitBox().intersects(_lhs->getHitBox())) {

				if (_dir == Direction::Horixontal) {

					if (speed_x > 0) {
						m_hit_box.left = _lhs->getPosX() - m_hit_box.width;
						jump();
					}
					if (speed_x < 0) {
						m_hit_box.left = _lhs->getPosX() + _lhs->getHitBox().width;
						jump();
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
		else if (_lhs->getType() == EntityType::SolidAbove) {

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
		else if (_lhs->getType() == EntityType::HeroBullet || _lhs->getType() == EntityType::BlueBullet) {

			if (m_hit_box.intersects(_lhs->getHitBox())) {
				gsl::not_null<Bullet*> bl = dynamic_cast<Bullet*>(_lhs.get());
				getHit(bl->getHit());
			}
		}

	}


	void Snake::jump(float _factor /*= 1.f*/) {

		if (!_effectIsActive_(EffectType::NoJump)) {
			if (m_status == EntityStat::onSolid || m_status == EntityStat::onSolidAbove) {
				speed_y = -110.f * _factor;
				setStat(EntityStat::InAir);
				setEffect(Effect(EffectType::NoJump, 0.f, 0.f, 1.f));
			}
		}

	}


	void Snake::jumpOff() {

		if (m_status == EntityStat::onSolidAbove) {
			moveIt(0.f, 1.f);
			setStat(EntityStat::InAir);
		}

	}


#define _eff_ eff.second
	void Snake::upEffect(float _time) noexcept {

		for (auto& eff : m_effects) {
			if (eff.second._time_effect > 0.f) {

				if (eff.first == EffectType::Discarding) {
					speed_x += _eff_._power * std::cos(_eff_._property);
					speed_y = _eff_._power * std::sin(_eff_._property);
				}
				else if (eff.first == EffectType::Attack && _eff_._time_effect - _time <= 0.0000005f) {
					m_goto = sf::Vector2f(getPosX(), getPosY());
				}

				_eff_._time_effect -= _time;
			}
			else {

				if (eff.first == EffectType::Poisoning && static_cast<int>(_eff_._property) > 0) {
					getHit(Hit{ _eff_._power });

					_eff_._property--;
					_eff_._time_effect = 1.f;
				}
				else if (eff.first == EffectType::OnFire && static_cast<int>(_eff_._property) > 0) {
					getHit(Hit{ _eff_._power });

					_eff_._property--;
					_eff_._time_effect = 1.f;
				}
				else if (eff.first == EffectType::Aggression) {
					_eff_ = Effect();
				}

			}
		}
		std::cout << std::endl;
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


		{//walking script
			if (!_effectIsActive_(EffectType::Attack)) {
				//_property == 1 - snack walking
				//--//--    == 0 - stay
				if (!_effectIsActive_(EffectType::Walking)) {

					if (m_effects[EffectType::Walking]._property == 0.f) {
						setEffect(Effect(EffectType::Walking, 1.f, 0.f, 5.f));

						m_goto.x = std::rand() % 100 + m_hit_box.left - 50;
						m_goto.y = m_hit_box.top;
					}
					else {
						//rallback
						setEffect(Effect(EffectType::Walking, 0.f, 0.f, 2.f));
					}

				}
				else {
					if (m_effects[EffectType::Walking]._property == 0.f) {
						//stops
						m_goto.x = m_hit_box.left;
						m_goto.y = m_hit_box.top;
					}
					else if (m_hit_box.contains(m_goto)) {
						setEffect(Effect(EffectType::Walking, 0.f, 0.f, 2.f));
					}
				}
			}
		}

		//printf("%f | ", speed_x);
		upEffect(_time);
		//printf("%f\n", speed_x);


		m_animation.update(_time);

		if (m_hp <= 0) {
			m_alive = false;
		}

	}


	void Snake::getHit(Hit _hit) noexcept {

		if (!_effectIsActive_(EffectType::Wounded)) {
			m_hp -= _hit._damage;
			speed_y = -70.f;
			setEffect(Effect(EffectType::Wounded, 0.f, 1.f + _hit._damage / 200.f, 0.2f));
			setEffect(Effect(EffectType::Aggression, 0.f, 2.f, 4.f));

			for (int i = 0; i < 4; i++) {
				setEffect(_hit._effect[i]);
			}
		}

	}


	void Snake::setEffect(const Effect& _eff) {

		using ET = EffectType;

		if (_eff._type == ET::Aggression) {
			m_effects[ET::Aggression]._power = std::max(m_effects[ET::Aggression]._power, _eff._power);
			m_effects[ET::Aggression]._time_effect = std::max(m_effects[ET::Aggression]._time_effect, _eff._time_effect);
		}
		else if (_eff._type == ET::Attack) {
			m_effects[ET::Attack] = _eff;
			m_effects[ET::Walking] = Effect();
		}
		else if (_eff._type == ET::Freezing) {
			if (_effectIsActive_(ET::OnFire)) {
				m_effects[ET::OnFire] = Effect();
			}
			else {
				m_effects[ET::Freezing] = _eff;
			}
		}
		else if (_eff._type == ET::OnFire) {
			if (_effectIsActive_(ET::Freezing)) {
				m_effects[ET::Freezing] = Effect();
			}
			else {
				m_effects[ET::OnFire] = _eff;
			}
		}
		else if (_eff._type == ET::Poisoning) {
			m_effects[ET::Poisoning] = _eff;
		}
		else {
			m_effects[_eff._type] = _eff;
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

		float acceler = countAccelerationX();

		if (_x > m_hit_box.left) {
			speed_x += acceler * _time;

			if (speed_x > MAX_SPEED_SNAKE) {
				speed_x = MAX_SPEED_SNAKE;
			}
		}
		else if (_x < m_hit_box.left) {
			speed_x -= acceler * _time;

			if (speed_x < -MAX_SPEED_SNAKE) {
				speed_x = -MAX_SPEED_SNAKE;
			}
		}
		else {
			if (speed_x > 0) {
				speed_x -= acceler * _time;
				speed_x = (speed_x >= 0) ? speed_x : 0;
			}
			else if (speed_x < 0) {
				speed_x += acceler * _time;
				speed_x = (speed_x <= 0) ? speed_x : 0;
			}
		}


		if (_y < m_hit_box.top - 2) {
			jump();
		}
		else if (_y > m_hit_box.top + 2) {
			jumpOff();
		}

	}


	float Snake::countRadius() {
		float res = m_distance_attack;

		if (_effectIsActive_(EffectType::Aggression)) {
			res *= m_effects[EffectType::Aggression]._power;
		}

		return res;
	}
	float Snake::countAccelerationX() {
		float res = m_accelerationx;

		if (_effectIsActive_(EffectType::Freezing)) {
			res /= m_effects[EffectType::Freezing]._power;
		}

		return res;
	}
	sf::Vector2f Snake::countSpeed() {
		sf::Vector2f res(speed_x, speed_y);

		if (_effectIsActive_(EffectType::Freezing)) {
			res.x /= m_effects[EffectType::Freezing]._power;
		}

		return res;
	}


	bool Snake::_effectIsActive_(EffectType _eff) const noexcept {

		auto res = m_effects.find(_eff);

		if (res == m_effects.end()) {
			return false;
		}
		return res->second._time_effect > 0.f;
	}


	void Snake::_setAnimations_() {

		m_animation.addAnimation(static_cast<int>(EntityAnimation::Idle));
		m_animation.addFrame(static_cast<int>(EntityAnimation::Idle), sf::IntRect(0, 40, 8, 8), 0.2f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Idle), sf::IntRect(8, 40, 8, 8), 0.2f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Idle), sf::IntRect(16, 40, 8, 8), 0.2f);

		m_animation.addAnimation(static_cast<int>(EntityAnimation::Walk));
		m_animation.addFrame(static_cast<int>(EntityAnimation::Walk), sf::IntRect(32, 40, 8, 8), 0.16f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Walk), sf::IntRect(40, 40, 8, 8), 0.16f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Walk), sf::IntRect(48, 40, 8, 8), 0.16f);

		m_animation.addAnimation(static_cast<int>(EntityAnimation::Attack));
		m_animation.addFrame(static_cast<int>(EntityAnimation::Attack), sf::IntRect(88, 40, 8, 8), 0.1f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Attack), sf::IntRect(96, 40, 8, 8), 0.1f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Attack), sf::IntRect(104, 40, 8, 8), 0.1f);

	}


	std::string Snake::DebugStr() /*override*/ {

		using std::setw;
		using std::endl;
		using std::setfill;

		std::ostringstream out;
		out.setf(std::ios::left | std::ios::boolalpha);

		out << setw(13) << setfill('\t') << "Type" << "Snake" << endl
			<< setw(12) << "ID" << m_id << endl
			<< setw(14) << "alive" << m_alive << std::endl
			<< setw(12) << "Hp" << m_hp << std::endl
			<< setw(14) << "status" << " " << m_status << std::endl
			<< setw(16) << "direction" << " " << static_cast<int>(m_direction) << std::endl
			<< setw(18) << "is gravity " << is_gravity << std::endl
			<< setw(17) << "coordinates" << "(" << m_hit_box.left << ", " << m_hit_box.top << ")\n"
			<< setw(13) << "size" << "  w: " << m_hit_box.width << ", h: " << m_hit_box.height << '\n'
			<< setw(13) << "speed" << "  (" << countSpeed().x << ", " << countSpeed().y << ")\n"
			<< setw(18) << "acceleration" << countAccelerationX() << "(def: " << m_accelerationx << ')' << std::endl
			<< setw(13) << "goto" << " (" << m_goto.x << ", " << m_goto.y << ")\n"
			<< setw(14) << "damage" << m_damage << std::endl
			<< setw(19) << "vision radius" << countRadius() << "(def: " << m_distance_attack << ')' << std::endl
			<< "effects:" << std::endl;
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

		return out.str();
	}

} // namespace ezg