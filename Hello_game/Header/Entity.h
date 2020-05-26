#pragma once

#include <SFML/Graphics.hpp>
#include "TileMap.h"
#include "config.h"


namespace ezg {

	class Entity : public sf::Drawable, public sf::Transformable
	{

	public:

		Entity(TipeEntity _tipe);

		Entity(TipeEntity _tipe, sf::Texture& texture, float pos_x, float pos_y, float _width, float _height);



	public:

		// выресовывает текущий элемент
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

		// осуществляет взаиможействие с картой
		virtual void colisionMap(Direction _dir, ezg::TileMap& _map) = 0;


		virtual void setSpeed(float _sp_x, float _sp_y) noexcept {

			speed_x = _sp_x;
			speed_y = _sp_y;

		}
		virtual void setSpeedX(float _sp_x) noexcept {

			speed_x = _sp_x;

		}
		virtual void addSpeedX(float _sp_x) noexcept {

			speed_x += _sp_x;
		}
		virtual void setSpeedY(float _sp_y) noexcept {

			speed_y = _sp_y;

		}
		virtual void addSpeedY(float _sp_y) noexcept {

			speed_y += _sp_y;

		}


	public:
		//void setPosition(float _x, float _y);

		void moveIt (float _x, float _y) noexcept;

		void upPosition (float time, Direction _dir) noexcept ;




		TipeEntity m_tipe;

		bool is_gravity;
		bool on_ground;

		float speed_x;
		float speed_y;

		sf::FloatRect m_hit_box;

		sf::Sprite sprite;

	}; // class Entity

} //namespace ezg