#include "Mushroom.h"

#include "../../Bullets/Bullets.h"

namespace ezg {

	Mushroom::Mushroom(TypeEntity _tipe, float place_x, float place_y, sf::IntRect _area, const sf::Texture& _texture)
		: Entity(_tipe, place_x, place_y, 6, 6)
		, m_direction(Direction::Right)
		, m_damage(20)
		, m_time(0.f)
		, m_time_effect(0.f)
		, m_hp(50)
		, m_area_attack(_area)
	{
		is_gravity = false;

		m_animation.setTexture(_texture);
		m_animation.activate(static_cast<int>(EntityAnimation::Idle));
	}



	void Mushroom::draw(sf::RenderTarget& target, sf::RenderStates states) const  noexcept /*override*/ {

		sf::Sprite _sprite = m_animation.getSprite();

		//expand the sprite if necessary
		if (m_direction == Direction::Left) {
			sf::IntRect _rec = _sprite.getTextureRect();

			_rec.left += _rec.width;
			_rec.width = -_rec.width;

			_sprite.setTextureRect(_rec);
		}

		//painted blue if the hero is injured
		if (_effectIsActive_(EffectType::Wounded)) {
			_sprite.setColor(sf::Color::Blue);
		}

		_sprite.setPosition(getPosX(), getPosY());
		target.draw(_sprite, states);
	}


	void Mushroom::colision(gsl::not_null <Entity*> _lhs, Direction _dir) /*final override*/ {

		if (_lhs->getType() == TypeEntity::HeroBullet) {

			if (m_hit_box.intersects(_lhs->getHitBox())) {
				gsl::not_null<Bullet*> bl = dynamic_cast<Bullet*>(_lhs.get());
				getHit(bl->getHit());
			}

		}

	}


	void Mushroom::getHit(Hit _hit) noexcept {

		if (!_effectIsActive_(EffectType::Wounded)) {

			if (_hit._damage > 0.f) {
				m_hp -= _hit._damage;

				m_effects[EffectType::Wounded]._time_effect = 0.8f;
			}

			for (int i = 0; i < 4; i++) {
				m_effects[_hit._effect[i]._type] = _hit._effect[i];
			}
		}

	}


	void Mushroom::upEffect(float _time) noexcept {


		for (auto& ef : m_effects) {
			if (ef.second._time_effect > 0.f) {

				ef.second._time_effect -= _time;
			}
		}

	}


	void Mushroom::otherUpdate(float _time) noexcept /*override*/ {

		m_time += _time;


		upEffect(_time);

		m_animation.update(_time);
		_upHitBox_();


		if (m_hp <= 0) {
			m_alive = false;
		}

	}


	void Mushroom::setStat(EntityAnimation _stat) {

		if (static_cast<int>(_stat) != m_animation.getActive()) {
			m_time = 0.7f;
			m_animation.activate(static_cast<int>(_stat));
		}

	}


	void Mushroom::_upHitBox_() {

		sf::IntRect _rec = m_animation.getSprite().getTextureRect();

		moveIt(0, m_hit_box.height - _rec.height);
		m_hit_box.height = _rec.height;


	}

} // namespace ezg