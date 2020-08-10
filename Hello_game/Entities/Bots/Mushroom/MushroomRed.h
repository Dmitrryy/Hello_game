#pragma once

#include "Mushroom.h"


namespace ezg {

	class MushroomRed final : public ezg::Mushroom
	{
	public:

		MushroomRed(float place_x, float place_y, sf::IntRect _area, const sf::Texture& _texture)
			: Mushroom(EntityType::MushroomRed, place_x, place_y, _area, _texture)
		{
			_setAnimations_();
		}

		/////////////////////////////////////////////////////////
		//shoots red bullets
		std::unique_ptr<Entity> fire(float _x, float _y) override;
		/////////////////////////////////////////////////////////


	private:

		void _setAnimations_() override;

	};

} // namespace ezg