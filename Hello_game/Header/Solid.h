#pragma once

#include "config.h"
#include "Entity.h"


namespace ezg {

	///////////////////////////////////////////////
	//
	//regular rectangl that you cannot go through
	//
	///////////////////////////////////////////////
	class Solid : public ezg::Entity {

	public:
		Solid(float place_x, float place_y, float width, float height)
			: Entity(TipeEntity::Solid, place_x, place_y, width, height)
		{
			is_gravity = false;
		}

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
			//nop
		}

		virtual void colision(ezg::Entity*, Direction) override {
			//nop
		}

	};

} // namespace ezg