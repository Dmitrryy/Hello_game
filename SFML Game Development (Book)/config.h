#pragma once
#include <SFML/Graphics.hpp>

namespace Resource 
{
	namespace Textures
	{
		enum class ID
		{
			Eagle,
			Raptor,
			Desert,
		};
	}

	namespace Fonts
	{
		enum class ID
		{
			Main
		};
	}
}


template <typename Resource, typename Identifier>
class ResourceHolder;

using TextureHolder = ResourceHolder<sf::Texture, Resource::Textures::ID>;
using FonstHolder   = ResourceHolder<sf::Font, Resource::Fonts::ID>;