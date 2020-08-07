#include "Bee.h"

#include "../../Bullets/Bullets.h"

namespace ezg {

	///////////////////////////////////////Bee///////////////////////////////////////////


	Bee::Bee(float place_x, float place_y, sf::IntRect _area, float _radius, const sf::Texture& _texture)
		: Entity(TypeEntity::Bee, place_x, place_y, 8, 8)
		, m_direction(Direction::Right)
		, m_damage(30)
		, m_acceleration(0.0003f)
		, m_time_effect(0)
		, m_hp(40)
		, m_area(_area)
		, m_goto(0, 0)
		, m_radius(_radius)
	{
		is_gravity = false;

		m_animation.setTexture(_texture);
		_setAnimations_();
		m_animation.activate(static_cast<int>(EntityAnimation::Walk));

		m_goto.x = std::rand() % static_cast<int>(m_area.width) + static_cast<int>(m_area.left);
		m_goto.y = std::rand() % static_cast<int>(m_area.height) + static_cast<int>(m_area.top);

		m_effects[EffectType::Walking]._time_effect = 5000.f;
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
		if (_effectIsActive_(EffectType::Wounded)) {
			_sprite.setColor(sf::Color::Red);
		}

		_sprite.setPosition(getPosX(), getPosY());
		target.draw(_sprite, states);
	}


	void Bee::colision(gsl::not_null <Entity*> _lhs, Direction _dir) /*final override*/ {


		if (_lhs->getType() == TypeEntity::HeroBullet) {

			if (m_hit_box.intersects(_lhs->getHitBox())) {
				gsl::not_null<Bullet*> bl = dynamic_cast<Bullet*>(_lhs.get());
				getHit(bl->getHit());
			}

		}

	}


	Hit Bee::attack(sf::FloatRect _rec) {

		sf::CircleShape _circle(m_radius);
		if (_effectIsActive_(EffectType::Attack)) {
			_circle.setRadius(_circle.getRadius() * 2);
		}
		_circle.setPosition(m_hit_box.left, m_hit_box.top);

		const float _x = _rec.left;
		const float _y = _rec.top;

		if (intersection(_circle, sf::Vector2f(_x, _y)) && !_effectIsActive_(EffectType::Wounded)) {
			m_effects[EffectType::Walking]._time_effect = 0.f;
			m_effects[EffectType::Attack]._time_effect = 100.f;

			_goto_(_x, _y);

			if (_rec.intersects(m_hit_box)) {
				speed_x /= -1.3;
				speed_y /= -1.3;
				return Hit{ m_damage, Effect() };
			}
		}
		else if (!_effectIsActive_(EffectType::Wounded) && !_effectIsActive_(EffectType::Walking)) {
			m_effects[EffectType::Walking]._time_effect = 100.f;
		}

		return Hit();
	}


	void Bee::getHit(Hit _hit) noexcept {

		if (!_effectIsActive_(EffectType::Wounded)) {

			if (_hit._damage > 0.f) {
				m_hp -= _hit._damage;


				m_effects[EffectType::Wounded]._time_effect = 600.f;
			}

			m_effects[_hit._effect._type] = _hit._effect;
		}

	}


	void Bee::upPosition(float _time, Direction _dir) noexcept /*override*/ {

		//printf("%d %d\n", _effectIsActive_(EffectType::Walking), _effectIsActive_(EffectType::Stop));

		if (_effectIsActive_(EffectType::Walking) && !_effectIsActive_(EffectType::Stop)) {

			if (_dir == Direction::Horixontal) {
				_goto_(m_goto.x, m_goto.y);

				if (m_hit_box.intersects(sf::FloatRect(m_goto.x, m_goto.y, 4, 4))) {
					m_effects[EffectType::Stop]._time_effect = 2000.f;

					m_goto.x = std::rand() % static_cast<int>(m_area.width) + static_cast<int>(m_area.left);
					m_goto.y = std::rand() % static_cast<int>(m_area.height) + static_cast<int>(m_area.top);
				}
			}

		}
		if (_effectIsActive_(EffectType::Attack)) {
			_time *= 1.f;
		}

		if (_dir == Direction::Horixontal) {
			moveIt(speed_x * _time, 0);
		}
		else if (_dir == Direction::Vertical) {
			moveIt(0, speed_y * _time);
		}

	}


	void Bee::upEffect(float _time) noexcept {

		for (auto& ef : m_effects) {
			if (ef.second._time_effect > 0.f) {

				ef.second._time_effect -= _time;
			}
		}


		if (_effectIsActive_(EffectType::Stop)) {

			speed_x -= m_acceleration * _time / 1.5f;
			if (speed_x < 0.f) { speed_x = 0.f; }

			speed_y -= m_acceleration * _time / 1.5f;
			if (speed_y < 0.f) { speed_y = 0.f; }
		}
		if (m_effects[EffectType::Attack]._time_effect < 10.f && _effectIsActive_(EffectType::Attack)) {
			m_effects[EffectType::Stop]._time_effect = 400.f;
		}

	}


	void Bee::otherUpdate(float _time) noexcept /*override*/ {


		upEffect(_time);

		m_animation.update(_time);

		if (m_acceleration * std::cos(m_corner) >= 0) {
			setDirection(Direction::Right);
		}
		else {
			setDirection(Direction::Left);
		}

		if (m_hp <= 0) {
			m_alive = false;
		}

	}


	void Bee::_goto_(float _x, float _y) noexcept {

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

		const float diff_x = _x - m_hit_box.left;
		const float diff_y = _y - m_hit_box.top;


		m_corner = std::atan(diff_y / diff_x);

		if (diff_y < 0.f && diff_x < 0.f) {
			m_corner -= 3.141592;
		}
		else if (diff_x < 0.f && diff_y > 0.f) {
			m_corner = 3.141592 + m_corner;
		}

		speed_x += m_acceleration * std::cos(m_corner);
		speed_y += m_acceleration * std::sin(m_corner);

		//slow down at the goal level (by y).
		//otherwise it can spin like a planet around the sun.
		if (std::fabs(diff_y) < 8) {
			speed_y /= 1.008f;
		}

		const float excess = std::sqrt(speed_x * speed_x + speed_y * speed_y - BEE_MAX_SPEED * BEE_MAX_SPEED);
		if (excess > 0) {
			speed_x -= excess * std::cos(m_corner);
			speed_y -= excess * std::sin(m_corner);
		}

	}


	void Bee::_setAnimations_() {

		m_animation.addAnimation(static_cast<int>(EntityAnimation::Walk));
		m_animation.addFrame(static_cast<int>(EntityAnimation::Walk), sf::IntRect(32, 16, 8, 8), 160.f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Walk), sf::IntRect(40, 16, 8, 8), 160.f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Walk), sf::IntRect(48, 16, 8, 8), 160.f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Walk), sf::IntRect(56, 16, 8, 8), 160.f);

	}

} //namespace ezg