#pragma once


#include "TileMap.h"
#include "config.h"


namespace ezg {

	class Entity : public sf::Drawable, public sf::Transformable
	{

	public:

		Entity(TipeEntity _tipe);

		Entity(TipeEntity _tipe, float pos_x, float pos_y, float _width, float _height);



	public:

		// выресовывает текущий элемент
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

		// осуществляет взаиможействие с картой
		//virtual void colisionMap(Direction _dir, ezg::TileMap& _map) = 0;

		virtual void colision(ezg::Entity* _entity, Direction _dir) = 0;

		void SetSprite(sf::Texture& texture, int location_x = 0, int location_y = 0);

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

		void moveIt (float _x, float _y) noexcept;

		void upPosition (float time, Direction _dir) noexcept ;



	private:

		TipeEntity m_tipe;

		bool is_gravity;
		bool on_ground;

		float speed_x;
		float speed_y;

		sf::FloatRect m_hit_box;

		sf::Sprite sprite;

	}; // class Entity

} //namespace ezg