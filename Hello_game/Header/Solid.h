#pragma once

#include "config.h"
#include "Entity.h"


namespace ezg {

	///////////////////////////////////////////////
	//
	//regular rectangl that you cannot go through
	//
	///////////////////////////////////////////////
	class Solid : public ezg::Entity 
	{

	public:

		Solid(float place_x, float place_y, float width, float height)
			: Entity(TipeEntity::Solid, place_x, place_y, width, height)
		{
			is_gravity = false;
		}

		void draw		(sf::RenderTarget& , sf::RenderStates)	noexcept override { /* nop */

			/*sf::RectangleShape rec(sf::Vector2f(m_hit_box.width, m_hit_box.height));
			rec.setPosition(sf::Vector2f(m_hit_box.left, m_hit_box.top));
			rec.setFillColor(sf::Color::Red);
			target.draw(rec, states);*/
		}

		//void colision	(gsl::not_null<Entity*>, Direction)		noexcept override { /* nop */ }

		void otherUpdate(float _time)							noexcept override { /* nop */ }

	}; // class Solid


	/////////////////////////////////////////////////////////////////////////////


	class SolidAbove : public ezg::Entity
	{

	public:

		SolidAbove(float place_x, float place_y, float width, float height)
			: Entity(TipeEntity::SolidAbove, place_x, place_y, width, height)
		{
			is_gravity = false;
		}

		void draw		(sf::RenderTarget&, sf::RenderStates)	noexcept override { /* nop */ }

		//void colision	(gsl::not_null<Entity*>, Direction)		noexcept override { /* nop */ }

		void otherUpdate(float _time)							noexcept override { /* nop */ }
	};


	class Stairs : public ezg::Entity
	{
	public:

		Stairs(float place_x, float place_y, float width, float height)
			: Entity(TipeEntity::Stairs, place_x, place_y, width, height)
		{
			is_gravity = false;
		}

		void draw		(sf::RenderTarget&, sf::RenderStates)	noexcept override { /* nop */ }

		//void colision	(gsl::not_null<Entity*>, Direction)		noexcept override { /* nop */ }

		void otherUpdate(float _time)							noexcept override { /* nop */ }
	};

} // namespace ezg