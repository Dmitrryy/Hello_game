#include <iomanip>

#include "Mushroom.h"

#include "../../Bullets/Bullets.h"
#include "../../Hero/Player.h"

namespace ezg {

	Mushroom::Mushroom(Type _tipe, float place_x, float place_y, sf::IntRect _area, const sf::Texture& _texture)
		: Entity(_tipe, place_x, place_y, 6, 6)
		, m_direction(Direction::Right)
		, m_damage(20)
		, m_time(0.f)
		, speed(1.f)
		, m_time_effect(0.f)
		, m_hp(50)
		, m_area_attack(_area)
	{
		is_gravity = false;

		m_animation.setTexture(_texture);
	}



	void Mushroom::draw(sf::RenderTarget& target, sf::RenderStates states) const /*override*/ {

		sf::Sprite _sprite = m_animation.getSprite();

		//expand the sprite if necessary
		if (m_direction == Direction::Left) {
			sf::IntRect _rec = _sprite.getTextureRect();

			_rec.left += _rec.width;
			_rec.width = -_rec.width;

			_sprite.setTextureRect(_rec);
		}

		//painted blue if the hero is injured
		if (_effectIsActive_(Effect::Type::Wounded)) {
			_sprite.setColor(sf::Color::Blue);
		}

		_sprite.setPosition(getPosX(), getPosY());
		target.draw(_sprite, states);
	}


	std::unique_ptr<Entity> Mushroom::attack(float _x, float _y) {

		std::unique_ptr<Entity> res = nullptr;
		if (!_effectIsActive_(Effect::Type::CantFire)) {

			//if first
			if (m_effects[Effect::Type::CantFire]._property == 0.f) {
				setEffect(Effect(Effect::Type::Attack, 0.f, 0.f, 1.4f));
				setEffect(Effect(Effect::Type::CantFire, 1.f, 0.f, 0.7f));
			}
			else if (m_effects[Effect::Type::CantFire]._property == 1.f) {
				res = fire(_x, _y);
				setEffect(Effect(Effect::Type::CantFire, 0.f, 0.f, 0.7f));
			}

		}
		return res;
	}


	std::unique_ptr<Entity> Mushroom::colision(Entity* _lhs, Direction _dir) /*final override*/ {

		if (_lhs == nullptr) { return nullptr; }
		std::unique_ptr<Entity> result = nullptr;

		if (_lhs->getType() == Type::HeroBullet || _lhs->getType() == Type::BlueBullet) {

			if (m_hit_box.intersects(_lhs->getHitBox())) {
				const gsl::not_null<Bullet*> bl = dynamic_cast<Bullet*>(_lhs);
				getHit(bl->getHit());
			}

		}
		else if (_lhs->getType() == Type::Hero) {
			if (m_area_attack.intersects(_lhs->getHitBox())) {
				const gsl::not_null<Hero*> _hr = dynamic_cast<Hero*>(_lhs);
				result = attack(_hr->getPosX(), _hr->getPosY());
			}
		}
		return result;
	}


	void Mushroom::getHit(Hit _hit) noexcept {

		if (!_effectIsActive_(Effect::Type::Wounded)) {

			if (_hit._damage > 0.f) {
				m_hp -= _hit._damage;

				m_effects[Effect::Type::Wounded]._time_effect = 0.8f;
			}

			for (int i = 0; i < 4; i++) {
				setEffect(_hit._effect[i]);
			}
		}

	}


	void Mushroom::setEffect(const Effect& _eff) {

		using ET = Effect::Type;

		if (_eff._type == ET::Aggression) {
			m_effects[ET::Aggression]._power = std::max(m_effects[ET::Aggression]._power, _eff._power);
			m_effects[ET::Aggression]._time_effect = std::max(m_effects[ET::Aggression]._time_effect, _eff._time_effect);
		}
		else if (_eff._type == ET::Attack) {
			m_animation.activate(static_cast<int>(Animation::Attack));
			m_effects[ET::Attack] = _eff;
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


#define _eff_ eff.second
	void Mushroom::upEffect(float _time) noexcept {

		for (auto& eff : m_effects) {
			if (eff.second._time_effect > 0.f) {

				if (eff.first == Effect::Type::Attack && _eff_._time_effect - _time < 0.00000001f) {
					m_animation.activate(static_cast<int>(Animation::Idle));
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


	void Mushroom::otherUpdate(float _time) noexcept /*override*/ {

		m_time += _time;


		upEffect(_time);

		m_animation.update(_time);
		_upHitBox_();


		if (m_hp <= 0) {
			m_alive = false;
		}

	}


	//void Mushroom::setStat(EntityAnimation _stat) {

	//	if (static_cast<int>(_stat) != m_animation.getActive()) {
	//		m_time = 0.7f;
	//		m_animation.activate(static_cast<int>(_stat));
	//	}

	//}


	void Mushroom::_upHitBox_() {

		sf::IntRect _rec = m_animation.getSprite().getTextureRect();

		moveIt(0, m_hit_box.height - _rec.height);
		m_hit_box.height = _rec.height;

	}


	std::string Mushroom::DebugStr() {

		using std::setw;
		using std::endl;
		using std::setfill;

		std::ostringstream out;
		out.setf(std::ios::left | std::ios::boolalpha);

		out << setw(13) << setfill('\t') << "Type" << "Mushroom" << endl
			<< setw(12) << "ID" << m_id << endl
			<< setw(14) << "alive" << m_alive << std::endl
			<< setw(12) << "Hp" << m_hp << std::endl
			<< setw(18) << "is gravity " << is_gravity << std::endl
			<< setw(17) << "coordinates" << "(" << m_hit_box.left << ", " << m_hit_box.top << ")\n"
			<< setw(13) << "size" << "  w: " << m_hit_box.width << ", h: " << m_hit_box.height << '\n'
			<< setw(14) << "damage" << m_damage << std::endl
			<< setw(13) << "area" << "  x:" << m_area_attack.left << ", y:" << m_area_attack.top
			<< "w:" << m_area_attack.width << ", h:" << m_area_attack.height << std::endl
			<< setw(16) << "animation" << m_animation.getActive() << std::endl
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