#include "SpriteNode.h"

SpriteNode::SpriteNode(const sf::Texture& texture) 
	: m_sprite(texture)
{

}
SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect& rect)
	: m_sprite(texture, rect)
{

}

void SpriteNode::drawThis(sf::RenderTarget& target, sf::RenderStates states) const /*final*/ {
	target.draw(m_sprite, states);
}