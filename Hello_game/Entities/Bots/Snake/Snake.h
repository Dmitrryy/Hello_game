#pragma once

#include "../../../config.h"
#include "../../Entity.h"


namespace ezg {



#define SNAKE_RALLBACK_JUMP 1.f
#define SNAKE_DURATION_WOUNDED 0.1f

	class Snake final : public ezg::Entity
	{
	public:

		Snake(float place_x, float place_y, float _distance_attack, const sf::Texture& _texture);


	public:

		void draw(sf::RenderTarget& target, sf::RenderStates states) const noexcept override;


		/////////////////////////////////////////////////////////
		//interaction with other objects
		std::unique_ptr<Entity> colision(Entity* _lhs, Direction _dir) final override;
		/////////////////////////////////////////////////////////


		Hit attack(sf::FloatRect _rec);
		float countRadius();
		float countAccelerationX();
		sf::Vector2f countSpeed();

		void  jump(float _factor = 1.f);


		void jumpOff();


		void setStat(Stat _new);


		void setEffect(const Effect& _new_effect);


		void getHit(Hit _hit) noexcept;


		void upEffect(float _time) noexcept;


		void upPosition(float time, Direction _dir) noexcept;


		void otherUpdate(float _time);


		bool _effectIsActive_(Effect::Type _eff) const noexcept;


		std::string DebugStr() override;


	private:

		void _setAnimations_();


		void _goto_(float _x, float _y, float _time);


	public:

		float			m_time_effect;

		float			m_hp;

		sf::Vector2f    m_goto;

		float			speed_x;
		float			speed_y;
		float			m_accelerationx;

		ezg::Animation		m_animation;

		Stat		m_status;
		std::map < Effect::Type, Effect >	    m_effects;
		Direction		m_direction;

		float			m_damage;
		float			m_distance_attack;
	};

} // namespace ezg