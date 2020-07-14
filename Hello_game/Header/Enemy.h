#pragma once

#include "config.h"
#include "Entity.h"


namespace ezg {


	class Mushroom : public Entity
	{
	protected:

		Mushroom(TipeEntity _tipe, float place_x, float place_y, sf::IntRect _area, const sf::Texture& _texture);


	public:

		void draw(sf::RenderTarget& target, sf::RenderStates states) noexcept override;



		virtual std::unique_ptr<Entity> fire(float _x, float _y) = 0;


		void getHit(float _damage) noexcept;

		void upEffect(float _time) noexcept;


		void otherUpdate(float _time) noexcept override;


		void setDirection(Direction _dir) noexcept { m_direction = _dir; }
		void setStat(EntityAnimation _stat);
		void setEffect(EntityEffect _effect) noexcept { m_effect = _effect; }


		sf::FloatRect getArea() const noexcept { return m_area_attack; }


	private:

		virtual void _setAnimations_() = 0;


		void _upHitBox_();


	protected:

		float m_time;
		float m_time_effect;

		float m_hp;

		Animation m_animation;
		EntityEffect m_effect;
		Direction m_direction;

		float m_damage;
		sf::FloatRect m_area_attack;
	};



	////////////////////////////////////////////////////////////////////////////////////////////////


	class MushroomRed : public Mushroom
	{
	public:

		MushroomRed(float place_x, float place_y, sf::IntRect _area, const sf::Texture& _texture)
			: Mushroom(TipeEntity::MushroomRed, place_x, place_y, _area, _texture)
		{
			_setAnimations_();
		}


		std::unique_ptr<Entity> fire(float _x, float _y) override;

	private:


		void _setAnimations_() override;

	};

	///////////////////////////////////////////////////////////////////////////////////////



	class MushroomBlue : public Mushroom
	{
	public:

		MushroomBlue(float place_x, float place_y, sf::IntRect _area, const sf::Texture& _texture)
			: Mushroom(TipeEntity::MushroomBlue, place_x, place_y, _area, _texture)
		{
			_setAnimations_();
		}
			

		std::unique_ptr<Entity> fire(float _x, float _y) override;


	private:

		void _setAnimations_() override;

	};



	///////////////////////////////////////////////////////////////////////////////////////

#define OBSERVETION_RADIUS  40
#define MAX_SPEED			0.05f

	class Bee : public Entity
	{
	public:

		Bee(float place_x, float place_y, sf::IntRect _area, float _radius, const sf::Texture& _texture);


		void draw(sf::RenderTarget& target, sf::RenderStates states) noexcept override;


		float attack(sf::FloatRect _rec);


		void getHit(float _damage) noexcept;


		void upEffect(float _time) noexcept;


		void otherUpdate(float _time) noexcept override;


		void upPosition(float time, Direction _dir) noexcept override;



		void setDirection(Direction _dir) noexcept { m_direction = _dir; }


		void setEffect(EntityEffect _effect) noexcept;


		static bool intersection(sf::CircleShape _circle, sf::Vector2f _point);


	private:

		void _setAnimations_();

		void _goto_(float _x, float _y) noexcept;


	private:

		float			m_time_effect;

		float			m_hp;

		sf::Vector2f    m_goto;


		float			speed_x;
		float			speed_y;
		float			m_acceleration;
		float			m_corner;

		Animation		m_animation;
		EntityEffect	m_effect;
		Direction		m_direction;

		float			m_damage;
		sf::FloatRect	m_area;
		float			m_radius;

	};

	///////////////////////////////////////////////////////////////////////////////////////


	class Snake : public Entity
	{
	public:

		Snake(float place_x, float place_y, float _distance_attack, const sf::Texture& _texture);


		void draw(sf::RenderTarget& target, sf::RenderStates states) noexcept override;


		float attack(sf::FloatRect _rec);


	private: float m_rallback_jump = 0.f;
	public:
		void jump() {

			if (m_rallback_jump > 800.f) {
				if (m_status == EntityStat::onSolid || m_status == EntityStat::onSolidAbove) {
					speed_y = -0.13f;
					setStat(EntityStat::InAir);
					m_rallback_jump = 0.f;
				}
			}

		}

		void jumpOff() {

			if (m_status == EntityStat::onSolidAbove) {
				moveIt(0.f, 1.f);
				setStat(EntityStat::InAir);
			}

		}


		void setStat(EntityStat _new);

		void setEffect(EntityEffect _new_effect);


		void getHit(float _damage) noexcept;


		void upEffect(float _time) noexcept;


		void upPosition(float time, Direction _dir) noexcept;


		void otherUpdate(float _time);

		void Stop() { setEffect(EntityEffect::Normal); }


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
		EntityEffect	m_effect;
		Direction		m_direction;

		float			m_damage;
		float			m_distance_attack;
	};


	///////////////////////////////////////////////////////////////////////////////////////

	class Needle : public Entity
	{
	public:

		Needle (float place_x, float place_y, float width, float height);

		void draw		 (sf::RenderTarget&, sf::RenderStates)	noexcept override { /* nop */ }

		void otherUpdate (float _time)							noexcept override { /* nop */ }

		inline const float getDamage() const noexcept { return m_damage; }


	private:
		
		float m_damage;
	};



} // namespace ezg