#pragma once

#include "../../config.h"
#include "../Entity.h"

#include "../Solid/Solid.h"

namespace ezg {


	class Bullet : public ezg::Entity
	{
	public:

		enum class Type {
			  HeroBullet
			, RedBullet
			, BlueBullet
		};

	protected:

		Bullet(Type _tipe, float place_x, float place_y) noexcept
			: Entity(Entity::Type::Bullet, place_x, place_y, 1, 1)
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
		std::unique_ptr<Entity> colision(Entity* _lhs, Direction _dir) final {

			if (_lhs == nullptr) { return nullptr; }
			std::unique_ptr<Entity> result = nullptr;

			if (m_hit_box.intersects(_lhs->getHitBox())) {
				if (_lhs->getType() == Entity::Type::Landscape) {

					const gsl::not_null<Landscape*> lndscp = dynamic_cast<Landscape*>(_lhs);
					if (lndscp->getType() == Landscape::Type::Solid) {
						setAlive(false);
					}

				}
			}
			return result;
		}
		/////////////////////////////////////////////////////////



		void setSpeed(float _x, float _y) noexcept { speed_x = _x; /**/ speed_y = _y; }


		virtual Hit getHit() noexcept = 0;


		void upPosition(float time, Direction _dir) noexcept override {

			if (_dir == Direction::Horixontal) {
				moveIt(speed_x * time, 0);
			}
			else {
				moveIt(0, speed_y * time);
			}
		}

		Type getType() { return m_type; }

	protected :

		Type  m_type;

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
			: Bullet(Type::RedBullet, 0, 0)
		{
			is_gravity = false;
		}

		RedBullet(float place_x, float place_y) noexcept
			: Bullet(Type::RedBullet, place_x, place_y)
		{
			is_gravity = false;
		}


		void draw(sf::RenderTarget& target, sf::RenderStates states) const override {

			static sf::RectangleShape rec(sf::Vector2f(m_hit_box.width, m_hit_box.height));

			rec.setPosition(sf::Vector2f(m_hit_box.left, m_hit_box.top));
			rec.setFillColor(sf::Color::Red);

			target.draw(rec, states);

		}


		Hit getHit() noexcept override {
			setAlive(false);
			return Hit( m_damage, Effect(Effect::Type::OnFire, 5.f, 10.f, 2.f) );
		}


		void otherUpdate(float _time) noexcept override {

			m_time += _time;

			//life time is limited
			if (m_time > 5.f) {
				m_alive = false;
			}
		}

	};


	//////////////////////////////////////////////////////////////////////////////////////


	class BlueBullet final : public Bullet
	{
	public:

		BlueBullet() noexcept
			: Bullet(Type::BlueBullet, 0, 0)
		{
			is_gravity = false;
		}

		BlueBullet(float place_x, float place_y) noexcept
			: Bullet(Type::BlueBullet, place_x, place_y)
		{
			is_gravity = false;
		}


		void draw(sf::RenderTarget& target, sf::RenderStates states) const override {

			static sf::RectangleShape rec(sf::Vector2f(m_hit_box.width, m_hit_box.height));

			rec.setPosition(sf::Vector2f(m_hit_box.left, m_hit_box.top));
			rec.setFillColor(sf::Color::Blue);

			target.draw(rec, states);

		}


		Hit getHit() noexcept override {
			setAlive(false);
			return Hit( m_damage, Effect(Effect::Type::Freezing, 0.f, 1.5f, 20.f) );
		}


		void otherUpdate(float _time) noexcept override {

			m_time += _time;

			//life time is limited
			if (m_time > 5.f) {
				m_alive = false;
			}
		}

	};


	//////////////////////////////////////////////////////////////////////////////////////


	class HeroBullet final : public Bullet
	{
	public:

		HeroBullet() noexcept
			: Bullet(Type::HeroBullet, 0, 0)
		{
			is_gravity = false;
		}

		HeroBullet(float place_x, float place_y) noexcept
			: Bullet(Type::HeroBullet, place_x, place_y)

		{
			is_gravity = false;
		}


		void draw(sf::RenderTarget& target, sf::RenderStates states) const override {

			static sf::RectangleShape rec(sf::Vector2f(m_hit_box.width, m_hit_box.height));

			rec.setPosition(sf::Vector2f(m_hit_box.left, m_hit_box.top));
			rec.setFillColor(sf::Color::Cyan);

			target.draw(rec, states);

		}


		Hit getHit() noexcept override {
			setAlive(false);
			return Hit{ m_damage, Effect() };
		}


		void otherUpdate(float _time) noexcept override {

			m_time += _time;

			//life time is limited
			if (m_time > 5.f) {
				m_alive = false;
			}
		}


	};

} // namespace ezg