#pragma once

#include "../Entity.h"
#include "../../tools/ResourceHolder/ResourceHolder.h"
#include "../../config.h"

#include <SFML/Graphics/Sprite.hpp>


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

private:

	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;

private:

	Type       m_type;
	sf::Sprite m_sprite;
};