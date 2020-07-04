#pragma once

#include "config.h"
#include "Entity.h"


namespace ezg {


	class Needle : public Entity
	{
	public:

		Needle(float place_x, float place_y, float width, float height)
			: Entity(TipeEntity::Needle, place_x, place_y, width, height)
		{
			is_gravity = false;
		}

		void draw		(sf::RenderTarget&, sf::RenderStates)	noexcept override { /* nop */ }

		void colision	(gsl::not_null<Entity*>, Direction)		noexcept override { /* nop */ }

		void otherUpdate(float _time)							noexcept override { /* nop */ }
	};



} // namespace ezg