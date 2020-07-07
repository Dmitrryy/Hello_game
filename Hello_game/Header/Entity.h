#pragma once


#include "TileMap.h"
#include "config.h"


namespace ezg {


	////////////////////////////////////////////////
	enum class EntityStat {
		Death
		, InAir
		, onSolid
		, onSolidAbove
		, onStairs
	};
	//
	enum class EntityEffect {
		  Normal
		, Wounded
		, Walking
		, Attack
	};
#define DURATION_WOUNDED (2.f * 100.f)
#define BEE_DURATION_NORMAL (1000.f)
	//
	enum EntityAnimation {
		Idle
		, Jump
		, Walk
		, Wounded
		, Attack
	};
	//available game object types
	enum class TipeEntity {
		  Solid
		, SolidAbove
		, Stairs
		, Needle
		, HeroBullet
		, MushroomRed
		, RedBullet
		, MushroomBlue
		, BlueBullet
		, Bee
		, Snake
	};
	////////////////////////////////////////////////


	///////////////////////////////////////////
	//the main class of any object of the game
	class Entity
	{

	public:


		Entity (TipeEntity _tipe) noexcept;
		Entity (TipeEntity _tipe, float pos_x, float pos_y, float _width, float _height);

		//virtual ~Entity() {};

	public:

		inline const float			getPosX		() const noexcept { return m_hit_box.left;	}
		inline const float			getPosY		() const noexcept { return m_hit_box.top;	}
		inline const sf::FloatRect	getHitBox	() const noexcept { return m_hit_box;		}
		inline const TipeEntity		getTipe		() const noexcept { return m_tipe;  		}


		//does gravity affect an object
		inline const bool			isGravity	() const noexcept { return is_gravity;		}

		
		//to draw the object
		virtual void draw (sf::RenderTarget& target, sf::RenderStates states) = 0;
	

		//check intersection with another object
		//and determine the interaction
		static void colision(gsl::not_null <Entity*> _lhs_ent, gsl::not_null <Entity*> _rhs_ent, Direction _dir);
		static void _HeroBulletAndSolid_(HeroBullet* _bullet, Solid* _solid, Direction _dir);
		static void _HeroBulletAndMushroomRed_(HeroBullet* _bullet, MushroomRed* _mh, Direction _dir);
		static void _HeroBulletAndBee_(HeroBullet* _bullet, Bee* _bee, Direction _dir);

		static void _SnakeAndSolid_(Snake* _snake, Solid* _sol, Direction _dir);
		static void _SnakeAndHeroBullet_(Snake* _snake, HeroBullet* _bullet, Direction _dir);

		//position update virtual function
		virtual void upPosition(float time, Direction _dir) noexcept { }


		void setPosition (float _x, float _y)	noexcept { m_hit_box.left = _x; /**/ m_hit_box.top = _y; }


		inline const bool alive() const noexcept { return m_alive; }
		void setAlive(bool _new) noexcept { m_alive = _new; }

		//&@#
		virtual void otherUpdate(float _time) = 0;


	protected:

		//set hit box...
		void setHitBox   (sf::FloatRect _rect)	noexcept	{ m_hit_box = _rect;			}
		void setWidth    (float _width)			noexcept	{ m_hit_box.width = _width;		}
		void setHeight	 (float _height)		noexcept	{ m_hit_box.height = _height;	}


		//function for... move object
		void moveIt        (float _x, float _y)         noexcept;



	protected:

		TipeEntity		m_tipe;
		bool			m_alive;

		bool			is_gravity;

		sf::FloatRect	m_hit_box;
		
	}; // class Entity

} //namespace ezg