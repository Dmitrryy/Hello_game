#pragma once

#include "Mushroom.h"

namespace ezg {

	class MushroomBlue final : public ezg::Mushroom
	{
	public:

		MushroomBlue(float place_x, float place_y, sf::IntRect _area, const sf::Texture& _texture)
			: Mushroom(TypeEntity::MushroomBlue, place_x, place_y, _area, _texture)
		{
			_setAnimations_();
		}

		/////////////////////////////////////////////////////////
		//shoots blue bullets
		std::unique_ptr<Entity> fire(float _x, float _y) override;
		/////////////////////////////////////////////////////////


	private:

		void _setAnimations_() override;

	};

} // namespace ezg