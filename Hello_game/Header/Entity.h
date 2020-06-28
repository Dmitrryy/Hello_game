#pragma once


#include "TileMap.h"
#include "config.h"


namespace ezg {

	///////////////////////////////////////////
	//the main class of any object of the game
	///////////////////////////////////////////
	class Entity : public sf::Drawable, public sf::Transformable
	{
		friend NodeGame;


	public:

		Entity (TipeEntity _tipe) noexcept;
		Entity (TipeEntity _tipe, float pos_x, float pos_y, float _width, float _height);


	public:

		inline const float			getPosX		() const noexcept { return m_hit_box.left;	}
		inline const float			getPosY		() const noexcept { return m_hit_box.top;	}
		inline const sf::FloatRect	getHitBox	() const noexcept { return m_hit_box;		}
		inline const TipeEntity		getTipe		() const noexcept { return m_tipe;  		}

		inline const bool			onGround	() const noexcept { return on_ground;		}
		inline const bool			isGravity	() const noexcept { return is_gravity;		}

		
		virtual void draw (sf::RenderTarget& target, sf::RenderStates states) const = 0;
		

	protected:
		///////////////////////////////
		//change the speed of an object
		///////////////////////////////
		virtual void setSpeed  (float _sp_x, float _sp_y) noexcept;
		virtual void setSpeedX (float _sp_x)              noexcept {	speed_x =  _sp_x; }
		virtual void addSpeedX (float _sp_x)              noexcept {	speed_x += _sp_x; }
		virtual void setSpeedY (float _sp_y)              noexcept {	speed_y =  _sp_y; }
		virtual void addSpeedY (float _sp_y)              noexcept {	speed_y += _sp_y; }


	private:

		//check intersection with another object
		//and determine the interaction
		virtual void colision (gsl::not_null <Entity*> _entity, Direction _dir) = 0;
		

		//function for... move object
		void moveIt        (float _x, float _y)         noexcept;
		void upPosition    (float time, Direction _dir) noexcept;
		void setPositionHB (float _x, float _y)		  noexcept { m_hit_box.left = _x; /**/ m_hit_box.top = _y; }


		//sprite position is updated only once
		//after all coordinate updates, etc.
		void upPosSprite ()			{  m_sprite.setPosition(m_hit_box.left, m_hit_box.top);  }


		//set a sprite for an object
		void SetSprite   (const std::string& texture, int location_x, int location_y, int width, int height);


		//set hit box...
		void setHitBox   (sf::FloatRect _rect)	noexcept	{ m_hit_box = _rect;			}
		void setWidth    (float _width)			noexcept	{ m_hit_box.width = _width;		}
		void setHeight	 (float _height)		noexcept	{ m_hit_box.height = _height;	}

	protected:

		TipeEntity		m_tipe;

		bool			is_gravity;
		bool			on_ground;

		float			speed_x;
		float			speed_y;

		sf::FloatRect	m_hit_box;

		sf::Sprite		m_sprite;
		sf::Texture		m_texture;

	}; // class Entity

} //namespace ezg