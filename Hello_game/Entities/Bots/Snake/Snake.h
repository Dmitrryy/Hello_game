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
		void colision(gsl::not_null <Entity*> _lhs, Direction _dir) final override;
		/////////////////////////////////////////////////////////


		Hit attack(sf::FloatRect _rec);


	private: float m_rallback_jump = 0.f;
	public: void  jump();


		  void jumpOff();


		  void setStat(EntityStat _new);


		  void setEffect(EffectType _new_effect);


		  void getHit(Hit _hit) noexcept;


		  void upEffect(float _time) noexcept;


		  void upPosition(float time, Direction _dir) noexcept;


		  void otherUpdate(float _time);


		  void Stop() { setEffect(EffectType::Normal); }


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

		Animation		m_animation;

		EntityStat		m_status;
		EffectType	m_effect;
		Direction		m_direction;

		float			m_damage;
		float			m_distance_attack;
	};

} // namespace ezg