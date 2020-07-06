#pragma once

#include "config.h"
#include "Entity.h"

namespace ezg {


	class Bullet : public Entity 
	{
	public:


		Bullet(TipeEntity _tipe, float place_x, float place_y)
			: Entity(_tipe, place_x, place_y, 1, 1)
			, m_time(0)
			, m_damage(20)
			, speed_x(0)
			, speed_y(0)
		{
			is_gravity = false;
		}


		void setSpeed(float _x, float _y) noexcept { speed_x = _x; /**/ speed_y = _y; }


		inline const float getDamage() const noexcept { return m_damage; }


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


	class RedBullet : public Bullet
	{
	public:

		RedBullet() noexcept
			: Bullet(TipeEntity::RedBullet, 0, 0)
		{
			is_gravity = false;
		}

		RedBullet(float place_x, float place_y)
			: Bullet(TipeEntity::RedBullet, place_x, place_y)
		{
			is_gravity = false;
		}


		void draw(sf::RenderTarget& target, sf::RenderStates states) override {

			static sf::RectangleShape rec(sf::Vector2f(m_hit_box.width, m_hit_box.height));

			rec.setPosition(sf::Vector2f(m_hit_box.left, m_hit_box.top));
			rec.setFillColor(sf::Color::Red);

			target.draw(rec, states);

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


	class HeroBullet : public Bullet
	{
	public:

		HeroBullet() noexcept
			: Bullet(TipeEntity::HeroBullet, 0, 0)
		{
			is_gravity = false;
		}

		HeroBullet(float place_x, float place_y)
			: Bullet(TipeEntity::HeroBullet, place_x, place_y)

		{
			is_gravity = false;
		}


		void draw(sf::RenderTarget& target, sf::RenderStates states) override {

			static sf::RectangleShape rec(sf::Vector2f(m_hit_box.width, m_hit_box.height));

			rec.setPosition(sf::Vector2f(m_hit_box.left, m_hit_box.top));
			rec.setFillColor(sf::Color::Cyan);

			target.draw(rec, states);

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