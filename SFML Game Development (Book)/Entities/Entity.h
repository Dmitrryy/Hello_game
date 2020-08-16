#pragma once

#include "../tools/SceneNode/SceneNode.h"

namespace ezg {

	class Entity : public SceneNode
	{
	public:

		void				setVelocity(sf::Vector2f velocity);
		void				setVelocity(float vx, float vy);
		sf::Vector2f		getVelocity() const;

	private:

		virtual void		updateThis(sf::Time dt) override;

	private:

		sf::Vector2f		mVelocity;
	};

}