#pragma once

#include "../SceneNode/SceneNode.h"

namespace ezg {

	class SpriteNode : public SceneNode
	{
	public:

		SpriteNode(const sf::Texture& texture);
		SpriteNode(const sf::Texture& texture, const sf::IntRect& rect);

	private:

		void drawThis(sf::RenderTarget& target, sf::RenderStates states) const final;

	private:

		sf::Sprite m_sprite;
	};
}