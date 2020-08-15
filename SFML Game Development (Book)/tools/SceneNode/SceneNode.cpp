#include "SceneNode.h"
#include <cassert>

void SceneNode::attachChild(Ptr&& child)
{
	child->m_parent = this;
	m_children.push_back(std::move(child));
}

std::unique_ptr<SceneNode> SceneNode::detachChild(const SceneNode& node)
{
	auto found = std::find_if(m_children.begin(), m_children.end(),
		[&node](Ptr& p) -> bool { return p.get() == &node; });
	assert(found != m_children.end());

	Ptr result = std::move(*found);
	result->m_parent = nullptr;
	m_children.erase(found);

	return result;
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) /*final*/ const
{
	states.transform *= getTransform();

	drawThis(target, states);
	drawChildren(target, states);
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const {
	for (const Ptr& ch : m_children) {
		ch->draw(target, states);
	}
}

void SceneNode::update(sf::Time _time) {
	updateThis(_time);
	updateChildren(_time);
}

void SceneNode::updateChildren(sf::Time _time) {
	for (const Ptr& ch : m_children) {
		ch->update(_time);
	}
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;

	for (const SceneNode* node = this; node != nullptr; node = node->m_parent)
		transform = node->getTransform() * transform;

	return transform;
}