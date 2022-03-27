#include <cmath>

#include <sstream>
#include <iomanip>

#include "Bee.h"

#include "../../Bullets/Bullets.h"
#include "../../Hero/Player.h"

namespace ezg {

	///////////////////////////////////////Bee///////////////////////////////////////////


	Bee::Bee(float place_x, float place_y, sf::IntRect _area, float _radius, const sf::Texture& _texture)
		: Entity(Type::Bee, place_x, place_y, 8, 8)
		, m_direction(Direction::Right)
		, m_damage(30)
		, m_acceleration(60.f)
		, m_time_effect(0)
		, m_hp(40)
		, m_area(_area)
		, m_goto(0, 0)
		, m_radius(_radius)
	{
		is_gravity = false;

		m_animation.setTexture(_texture);
		_setAnimations_();
		m_animation.activate(static_cast<int>(Animation::Walk));

		m_goto.x = std::rand() % static_cast<int>(m_area.width) + static_cast<int>(m_area.left);
		m_goto.y = std::rand() % static_cast<int>(m_area.height) + static_cast<int>(m_area.top);
	}


	void Bee::draw(sf::RenderTarget& target, sf::RenderStates states) const noexcept /*override*/ {

		sf::Sprite _sprite = m_animation.getSprite();

		//expand the sprite if necessary
		if (m_direction == Direction::Left) {
			sf::IntRect _rec = _sprite.getTextureRect();

			_rec.left += _rec.width;
			_rec.width = -_rec.width;

			_sprite.setTextureRect(_rec);
		}

		//painted Red if the hero is injured
		if (_effectIsActive_(Effect::Type::Wounded)) {
			_sprite.setColor(sf::Color::Red);
		}

		_sprite.setPosition(getPosX(), getPosY());
		target.draw(_sprite, states);
	}


	std::unique_ptr<Entity> Bee::colision(Entity* _lhs, Direction _dir) /*final override*/ {

		std::unique_ptr<Entity> result = nullptr;

		if (_lhs->getType() == Entity::Type::Bullet) {

			if (m_hit_box.intersects(_lhs->getHitBox())) {
				Bullet* bl = dynamic_cast<Bullet*>(_lhs);
				getHit(bl->getHit());
			}

		}
		else if (_lhs->getType() == Entity::Type::Hero) {
			Hero* _hr = dynamic_cast<Hero*>(_lhs);
			_hr->getHit(attack(_hr->getHitBox()));
			
		}

		return result;
	}


	Entity::Hit Bee::attack(sf::FloatRect _rec) {

		sf::CircleShape _circle(countRadius());

		_circle.setPosition(m_hit_box.left, m_hit_box.top);

		const float _x = _rec.left;
		const float _y = _rec.top;

		if (intersection(_circle, sf::Vector2f(_x, _y))) {
			setEffect(Effect(Effect::Type::Aggression, 0.f, 1.8f, 4.f));
			setEffect(Effect(Effect::Type::Attack, 0.f, 0.f, 1.5f));

			m_goto = sf::Vector2f(_x, _y);

			if (_rec.intersects(m_hit_box)) {
				speed_x /= -1.3f;
				speed_y /= -1.3f;
				return Hit{ m_damage };
			}
		}

		return Hit();
	}


	void Bee::getHit(Hit _hit) noexcept {

		if (!_effectIsActive_(Effect::Type::Wounded)) {

			m_hp -= _hit._damage;

			setEffect(Effect(Effect::Type::Wounded, 0.f, 1.f + _hit._damage / 200.f, 0.2f));
			setEffect(Effect(Effect::Type::Aggression, 0.f, 2.f, 4.f));

			for (int i = 0; i < 4; i++) {
				setEffect(_hit._effect[i]);
			}
		}

	}


	void Bee::upPosition(float _time, Direction _dir) noexcept /*override*/ {
		if (_dir == Direction::Horixontal) {
			moveIt(countSpeed().x * _time, 0);
		}
		else if (_dir == Direction::Vertical) {

			_goto_(m_goto.x, m_goto.y, _time);

			moveIt(0, countSpeed().y * _time);
		}

	}


#define _eff_ eff.second
	void Bee::upEffect(float _time) noexcept {

		for (auto& eff : m_effects) {
			if (eff.second._time_effect > 0.f) {

				if (eff.first == Effect::Type::Discarding) {
					speed_x += _eff_._power * std::cos(_eff_._property);
					speed_y = _eff_._power * std::sin(_eff_._property);
				}
				else if (eff.first == Effect::Type::Attack && _eff_._time_effect - _time <= 0.0000005f) {
					m_goto = sf::Vector2f(getPosX(), getPosY());
				}

				_eff_._time_effect -= _time;
			}
			else {

				if (eff.first == Effect::Type::Poisoning && static_cast<int>(_eff_._property) > 0) {
					getHit(Hit{ _eff_._power });

					_eff_._property--;
					_eff_._time_effect = 1.f;
				}
				else if (eff.first == Effect::Type::OnFire && static_cast<int>(_eff_._property) > 0) {
					getHit(Hit{ _eff_._power });

					_eff_._property--;
					_eff_._time_effect = 1.f;
				}
				else if (eff.first == Effect::Type::Aggression) {
					_eff_ = Effect();
				}

			}
		}

	}
#undef _eff_


	void Bee::setEffect(const Effect& _eff) {

		using ET = Effect::Type;

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


	void Bee::otherUpdate(float _time) noexcept /*override*/ {


		m_animation.update(_time);
		if (m_acceleration * std::cos(m_corner) >= 0) {
			setDirection(Direction::Right);
		}
		else {
			setDirection(Direction::Left);
		}

		upEffect(_time);

		{
			if (!_effectIsActive_(Effect::Type::Attack)) {
				//_property == 1 - snack walking
				//--//--    == 0 - stay
				if (!_effectIsActive_(Effect::Type::Walking)) {

					if (m_effects[Effect::Type::Walking]._property == 0.f) {
						setEffect(Effect(Effect::Type::Walking, 1.f, 0.f, 10.f));
						
						m_goto.x = std::rand() % static_cast<int>(m_area.width) + static_cast<int>(m_area.left);
						m_goto.y = std::rand() % static_cast<int>(m_area.height) + static_cast<int>(m_area.top);
					}
					else {
						//rallback
						setEffect(Effect(Effect::Type::Walking, 0.f, 0.f, 5.f));
					}

				}
				else {
					if (m_effects[Effect::Type::Walking]._property == 0.f) {
						//stops
						m_goto.x = m_hit_box.left;
						m_goto.y = m_hit_box.top;
					}
					else if (m_hit_box.contains(m_goto)) {
						setEffect(Effect(Effect::Type::Walking, 0.f, 0.f, 5.f));
					}
				}
			}
		}

		if (m_hp <= 0) {
			m_alive = false;
		}

	}


	void Bee::_goto_(float _x, float _y, float _time) noexcept {

		//             @ . .|_y
		//             .\   |
		//             . \<-|__
		//             .  \ |  --- m_corner
		//             .   \|     ‾‾_
		//------------------|---------------------------→x
		//            _x   0|
		//                  |
		//                  |
		//                  ↓y
		float acl = countAcceleration();
		if (!m_hit_box.contains(sf::Vector2f(_x, _y))) {
			const float diff_x = _x - (m_hit_box.left + m_hit_box.width / 2.f);
			const float diff_y = _y - (m_hit_box.top + m_hit_box.height / 2.f);
			
			m_corner = std::atan(diff_y / diff_x);

			if (diff_y < 0.f && diff_x < 0.f) {
				m_corner -= 3.141592f;
			}
			else if (diff_x < 0.f && diff_y > 0.f) {
				m_corner = 3.141592f + m_corner;
			}

			speed_x += acl * std::cos(m_corner) * _time;
			speed_y += acl * std::sin(m_corner) * _time;

			//slow down at the goal level (by y).
			//otherwise it can spin like a planet around the sun.
			if (std::fabs(diff_y) < 4) {
				speed_y /= 1.09f;
				//speed_x *= 1.05f;
			}

			const float excess = std::sqrt(speed_x * speed_x + speed_y * speed_y - BEE_MAX_SPEED * BEE_MAX_SPEED);
			if (excess > 0) {
				speed_x -= excess * std::cos(m_corner);
				speed_y -= excess * std::sin(m_corner);
			}
		}
		else {
			//speed_x = speed_y = 0.f;
			if (speed_x > 0.f) {
				speed_x -= acl * _time * std::fabs(speed_x) / (std::fabs(speed_x) + std::fabs(speed_y));
				speed_x = (speed_x >= 0) ? speed_x : 0;
			}
			else {
				speed_x += acl * _time * std::fabs(speed_x) / (std::fabs(speed_x) + std::fabs(speed_y));
				speed_x = (speed_x <= 0) ? speed_x : 0;
			}
			if (speed_y > 0.f) {
				speed_y -= acl * _time * std::fabs(speed_y) / (std::fabs(speed_x) + std::fabs(speed_y));
				speed_y = (speed_y >= 0) ? speed_y : 0;
			}
			else {
				speed_y += acl * _time * std::fabs(speed_y) / (std::fabs(speed_x) + std::fabs(speed_y));
				speed_y = (speed_y <= 0) ? speed_y : 0;
			}
		}

	}


	float Bee::countRadius() {
		float res = m_radius;

		if (_effectIsActive_(Effect::Type::Aggression)) {
			res *= m_effects[Effect::Type::Aggression]._power;
		}

		return res;
	}
	float Bee::countAcceleration() {
		float res = m_acceleration;

		if (_effectIsActive_(Effect::Type::Freezing)) {
			res /= m_effects[Effect::Type::Freezing]._power;
		}

		return res;
	}
	sf::Vector2f Bee::countSpeed() {
		sf::Vector2f res(speed_x, speed_y);

		if (_effectIsActive_(Effect::Type::Freezing)) {
			res /= m_effects[Effect::Type::Freezing]._power;
		}

		return res;
	}


	void Bee::_setAnimations_() {

		m_animation.addAnimation(static_cast<int>(Animation::Walk));
		m_animation.addFrame(static_cast<int>(Animation::Walk), sf::IntRect(32, 16, 8, 8), 0.16f);
		m_animation.addFrame(static_cast<int>(Animation::Walk), sf::IntRect(40, 16, 8, 8), 0.16f);
		m_animation.addFrame(static_cast<int>(Animation::Walk), sf::IntRect(48, 16, 8, 8), 0.16f);
		m_animation.addFrame(static_cast<int>(Animation::Walk), sf::IntRect(56, 16, 8, 8), 0.16f);

	}


	std::string Bee::DebugStr() /*override*/ {

		using std::setw;
		using std::endl;
		using std::setfill;

		std::ostringstream out;
		out.setf(std::ios::left | std::ios::boolalpha);

		out << setw(13) << setfill('\t') << "Type" << "Bee" << endl
			<< setw(12) << "ID" << m_id << endl
			<< setw(14) << "alive" << m_alive << std::endl
			<< setw(12) << "Hp" << m_hp << std::endl
			<< setw(18) << "is gravity " << is_gravity << std::endl
			<< setw(17) << "coordinates" << "(" << m_hit_box.left << ", " << m_hit_box.top << ")\n"
			<< setw(13) << "size" << "  w: " << m_hit_box.width << ", h: " << m_hit_box.height << '\n'
			<< setw(13) << "speed" << "  (" << countSpeed().x << ", " << countSpeed().y << ")\n"
			<< setw(18) << "acceleration" << countAcceleration() << "(def: " << m_acceleration << ')' << std::endl
			<< setw(21) << "angle to the goal" << m_corner << std::endl
			<< setw(13) << "goto" << " (" << m_goto.x << ", " << m_goto.y << ")\n"
			<< setw(14) << "damage" << m_damage << std::endl
			<< setw(19) << "vision radius" << countRadius() << "(def: " << m_radius << ')' << std::endl
			<< setw(13) << "area" << "  x:" << m_area.left << ", y:" << m_area.top
			  << ", w:" << m_area.width << ", h:" << m_area.height << std::endl
			<< setw(16) << "animation" << static_cast<Entity::Animation>(m_animation.getActive()) << endl
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

} //namespace ezg