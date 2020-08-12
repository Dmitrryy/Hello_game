#pragma once

#include "../../../config.h"
#include "../../Entity.h"


namespace ezg {


	class Needle final : public ezg::Entity
	{
	public:

		Needle(float place_x, float place_y, float width, float height) noexcept;

		void draw(sf::RenderTarget&, sf::RenderStates) const noexcept override { /* nop */ }


		/////////////////////////////////////////////////////////
		//interaction with other objects
		std::unique_ptr<Entity> colision(Entity* _lhs, Direction _dir) noexcept override;
		/////////////////////////////////////////////////////////

		void setDamage(float _damage) { m_damage = _damage; }

		void otherUpdate(float _time)	noexcept override { /* nop */ }

		inline const Hit getDamage() const noexcept { return Hit{ m_damage, Effect() }; }


	private:

		float m_damage;
	};



} // namespace ezg