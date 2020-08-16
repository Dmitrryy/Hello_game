#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "../Command.h"

namespace ezg {

	class SceneNode : public sf::Drawable, public sf::Transformable
	{
	public:

		using Ptr = std::unique_ptr<SceneNode>;

		SceneNode(const SceneNode& _that) = delete; //not saported
		SceneNode& operator= (const SceneNode&) = delete; //not saported
		SceneNode(SceneNode&& _that) = delete; //not saported
		SceneNode& operator= (SceneNode&&) = delete; //not saported

	public:

		SceneNode()
			: m_parent(nullptr)
		{}

	public:

		void attachChild(Ptr&& child);
		Ptr  detachChild(const SceneNode& node);

		virtual Category::Type getCategory() const noexcept { return Category::Type::Scene; }

		void onCommand(const Command& command, sf::Time dt);

	public:

		void update(sf::Time _time);

		sf::Vector2f  getWorldPosition() const;
		sf::Transform getWorldTransform() const;

	private:

		virtual void updateThis(sf::Time _time) { /*nop*/ }
		void updateChildren(sf::Time _time);


	private:

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
		virtual void drawThis(sf::RenderTarget& target, sf::RenderStates states) const {}
		void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

	private:

		SceneNode* m_parent;
		std::vector<Ptr> m_children;
	};
}