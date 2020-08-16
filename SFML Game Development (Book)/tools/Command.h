#pragma once

#include <functional>
#include <SFML/Graphics.hpp>
#include <cassert>
//#include "SceneNode/SceneNode.h"


#include "../categories.h"
#include "../include/Flag/Flag.h"



namespace ezg {

class SceneNode;

	struct Command
	{
		Command() noexcept
			: category(Category::Type::None)
			//, action([](SceneNode&, sf::Time) {})
		{}

		std::function<void(SceneNode&, sf::Time)> action;
		Flag<Category::Type> category;
	};

	template <typename GameObject, typename Function>
	std::function<void(SceneNode&, sf::Time)> derivedAction(Function fn)
	{
		return [fn](SceneNode& node, sf::Time dt)
		{
			// Check if cast is safe
			assert(dynamic_cast<GameObject*>(&node) != nullptr);
			// Downcast node and invoke function on it
			fn(static_cast<GameObject&>(node), dt);
		};
	}
}