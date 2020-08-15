#include "Aircraft.h"


#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

using namespace Resource;

Textures::ID toTextureID(Aircraft::Type type)
{
	switch (type)
	{
		case Aircraft::Eagle:
			return Textures::ID::Eagle;

		case Aircraft::Raptor:
			return Textures::ID::Raptor;
	}
	return Textures::ID::Eagle;
}

Aircraft::Aircraft(Type type, const TextureHolder& textures)
	: m_type(type)
	, m_sprite(textures.get(toTextureID(type)))
{
	sf::FloatRect bounds = m_sprite.getLocalBounds();
	m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Aircraft::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}
