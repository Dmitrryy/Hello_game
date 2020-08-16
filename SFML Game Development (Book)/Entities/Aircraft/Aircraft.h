#pragma once

#include "../Entity.h"
#include "../../tools/ResourceHolder/ResourceHolder.h"
#include "../../config.h"

#include <SFML/Graphics/Sprite.hpp>

namespace ezg {

	class Aircraft : public Entity
	{
	public:
		enum Type
		{
			Eagle,
			Raptor,
		};

	public:

		Aircraft(Type type, const TextureHolder& textures);

		Category::Type getCategory() const noexcept override;

		void accelerate(float sp_x, float sp_y) { setVelocity(getVelocity() + sf::Vector2f(sp_x, sp_y)); }
		void accelerate(sf::Vector2f sp) { setVelocity(getVelocity() + sp); }

	private:

		void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:

		Type       m_type;
		sf::Sprite m_sprite;
	};
}