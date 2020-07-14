#pragma once

#include "../../config.h"
#include "../Entity.h"


namespace ezg {

	///////////////////////////////////////////////
	//
	//regular rectangl that you cannot go through
	//
	///////////////////////////////////////////////
	class Solid final : public ezg::Entity 
	{

	public:

		Solid(float place_x, float place_y, float width, float height) noexcept
			: Entity(TypeEntity::Solid, place_x, place_y, width, height)
		{
			is_gravity = false;
		}

		void draw (sf::RenderTarget&, sf::RenderStates) const noexcept override { /* nop */	}

		void colision (gsl::not_null<Entity*>, Direction) noexcept override { /* nop */ }

		void otherUpdate (float) noexcept override { /* nop */ }

	}; // class Solid



	////////////////////////////////////////////////////////////////////////////////////////////



	class SolidAbove final : public ezg::Entity
	{

	public:

		SolidAbove(float place_x, float place_y, float width, float height) noexcept
			: Entity(TypeEntity::SolidAbove, place_x, place_y, width, height)
		{
			is_gravity = false;
		}

		void draw (sf::RenderTarget&, sf::RenderStates) const noexcept override { /* nop */ }

		void colision (gsl::not_null<Entity*>, Direction) noexcept override { /* nop */ }

		void otherUpdate (float) noexcept override { /* nop */ }
	};



	////////////////////////////////////////////////////////////////////////////////////////////



	class Stairs final : public ezg::Entity
	{
	public:

		Stairs(float place_x, float place_y, float width, float height) noexcept
			: Entity(TypeEntity::Stairs, place_x, place_y, width, height)
		{
			is_gravity = false;
		}

		void draw (sf::RenderTarget&, sf::RenderStates) const noexcept override { /* nop */ }

		void colision (gsl::not_null<Entity*>, Direction) noexcept override { /* nop */ }

		void otherUpdate (float) noexcept override { /* nop */ }
	};

} // namespace ezg