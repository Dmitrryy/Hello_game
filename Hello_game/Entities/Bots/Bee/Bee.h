#pragma once



#include "../../Entity.h"


namespace ezg {


#define BEE_OBSERVETION_RADIUS  40
#define BEE_MAX_SPEED			70.f
#define BEE_DURATION_WOUNDED 0.5f
#define BEE_DURATION_NORMAL 10.f

	class Bee final : public ezg::Entity
	{
	public:

		/////////////////////////////////////////////////////////
		//constructor:
		// plece_x & place_y - the position of the upper left corner of the mushroom.
		// _area - free-walk area.
		// _radius - target capture radius.
		// _texture - texture... (its copy is kept. do not allow the destruction of the original)
		Bee(float place_x, float place_y, sf::IntRect _area, float _radius, const sf::Texture& _texture);
		/////////////////////////////////////////////////////////


	public:

		/////////////////////////////////////////////////////////
		//draws a bot
		void draw(sf::RenderTarget& target, sf::RenderStates states) const noexcept override;
		/////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////
		//checks if _rec is in scope, if so, it attacks.
		Hit attack(sf::FloatRect _rec);
		/////////////////////////////////////////////////////////
		float countRadius();
		float countAcceleration();
		sf::Vector2f countSpeed();

		/////////////////////////////////////////////////////////
		//interaction with other objects
		std::unique_ptr<Entity> colision(Entity* _lhs, Direction _dir) final override;
		/////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////
		//get hit...
		void getHit(Hit _hit) noexcept;
		/////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////
		//updates time for effects
		void upEffect(float _time) noexcept;
		/////////////////////////////////////////////////////////
		void setEffect(const Effect& _eff);


		/////////////////////////////////////////////////////////
		// - updates the time of effects.
		// - updates the animation.
		// - checks if the object is alive.
		// - checks direction (to draw the sprite in the desired orientation).
		void otherUpdate(float _time) noexcept override;
		/////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////
		//moves a bee
		void upPosition(float time, Direction _dir) noexcept override;
		/////////////////////////////////////////////////////////

		void setRadius(float _rad) noexcept { m_radius = _rad; }
		void setHP(float _hp) noexcept { m_hp = _hp; }
		void setDamage(float _damage) noexcept { m_damage = _damage; }


		void setDirection(Direction _dir) noexcept { m_direction = _dir; }


		std::string DebugStr() override;


	private:

		void _setAnimations_();


		/////////////////////////////////////////////////////////
		//directs acceleration to a point (_x, _y)
		void _goto_(float _x, float _y, float _time) noexcept;
		/////////////////////////////////////////////////////////


		bool _effectIsActive_(Effect::Type _eff) const {

			auto res = m_effects.find(_eff);

			if (res != m_effects.end()) {
				return res->second._time_effect > 0.f;
			}
			return false;
		}

	private:

		float			m_time_effect;

		float			m_hp;

		sf::Vector2f    m_goto;


		float			speed_x;
		float			speed_y;
		float			m_acceleration;
		float			m_corner;

		ezg::Animation	m_animation;
		std::map<Effect::Type, Effect>	m_effects;
		Direction		m_direction;

		float			m_damage;
		sf::FloatRect	m_area;
		float			m_radius;

	};

} // namespace ezg