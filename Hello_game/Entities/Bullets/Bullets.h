#pragma once

#include "../../config.h"
#include "../Entity.h"

namespace ezg {


	class Bullet : public ezg::Entity
	{
	protected:

		Bullet(TypeEntity _tipe, float place_x, float place_y) noexcept
			: Entity(_tipe, place_x, place_y, 1, 1)
			, m_time(0)
			, m_damage(20)
			, speed_x(0)
			, speed_y(0)
		{
			is_gravity = false;
		}

	public:

		/////////////////////////////////////////////////////////
		//interaction with other objects
		void colision(gsl::not_null <Entity*> _lhs, Direction _dir) final {

			if (m_hit_box.intersects(_lhs->getHitBox())) {
				if (_lhs->getType() == TypeEntity::Solid) {
					setAlive(false);
				}
			}

		}
		/////////////////////////////////////////////////////////



		void setSpeed(float _x, float _y) noexcept { speed_x = _x; /**/ speed_y = _y; }


		virtual Hit getHit() const noexcept = 0;


		void upPosition(float time, Direction _dir) noexcept override {

			if (_dir == Direction::Horixontal) {
				moveIt(speed_x * time, 0);
			}
			else {
				moveIt(0, speed_y * time);
			}
		}


	protected :

		float m_time;

		float m_damage;

		float speed_x;
		float speed_y;
	};


	//////////////////////////////////////////////////////////////////////////////////////


	class RedBullet final : public Bullet
	{
	public:

		RedBullet() noexcept
			: Bullet(TypeEntity::RedBullet, 0, 0)
		{
			is_gravity = false;
		}

		RedBullet(float place_x, float place_y) noexcept
			: Bullet(TypeEntity::RedBullet, place_x, place_y)
		{
			is_gravity = false;
		}


		void draw(sf::RenderTarget& target, sf::RenderStates states) const override {

			static sf::RectangleShape rec(sf::Vector2f(m_hit_box.width, m_hit_box.height));

			rec.setPosition(sf::Vector2f(m_hit_box.left, m_hit_box.top));
			rec.setFillColor(sf::Color::Red);

			target.draw(rec, states);

		}


		Hit getHit() const noexcept override {

			return Hit( m_damage, Effect(EffectType::OnFire, 5.f, 6000.f) );
		}


		void otherUpdate(float _time) noexcept override {

			m_time += _time;

			//life time is limited
			if (m_time > 1100.f) {
				m_alive = false;
			}
		}

	};


	//////////////////////////////////////////////////////////////////////////////////////


	class BlueBullet final : public Bullet
	{
	public:

		BlueBullet() noexcept
			: Bullet(TypeEntity::BlueBullet, 0, 0)
		{
			is_gravity = false;
		}

		BlueBullet(float place_x, float place_y) noexcept
			: Bullet(TypeEntity::BlueBullet, place_x, place_y)
		{
			is_gravity = false;
		}


		void draw(sf::RenderTarget& target, sf::RenderStates states) const override {

			static sf::RectangleShape rec(sf::Vector2f(m_hit_box.width, m_hit_box.height));

			rec.setPosition(sf::Vector2f(m_hit_box.left, m_hit_box.top));
			rec.setFillColor(sf::Color::Blue);

			target.draw(rec, states);

		}


		Hit getHit() const noexcept override {

			return Hit( m_damage, Effect(EffectType::Freezing, 1.f, 8000.f) );
		}


		void otherUpdate(float _time) noexcept override {

			m_time += _time;

			//life time is limited
			if (m_time > 1000.f) {
				m_alive = false;
			}
		}

	};


	//////////////////////////////////////////////////////////////////////////////////////


	class HeroBullet final : public Bullet
	{
	public:

		HeroBullet() noexcept
			: Bullet(TypeEntity::HeroBullet, 0, 0)
		{
			is_gravity = false;
		}

		HeroBullet(float place_x, float place_y) noexcept
			: Bullet(TypeEntity::HeroBullet, place_x, place_y)

		{
			is_gravity = false;
		}


		void draw(sf::RenderTarget& target, sf::RenderStates states) const override {

			static sf::RectangleShape rec(sf::Vector2f(m_hit_box.width, m_hit_box.height));

			rec.setPosition(sf::Vector2f(m_hit_box.left, m_hit_box.top));
			rec.setFillColor(sf::Color::Cyan);

			target.draw(rec, states);

		}


		Hit getHit() const noexcept override {

			return Hit{ m_damage, Effect() };
		}


		void otherUpdate(float _time) noexcept override {

			m_time += _time;

			//life time is limited
			if (m_time > 1000.f) {
				m_alive = false;
			}
		}


	};

} // namespace ezg