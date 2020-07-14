#pragma once

#include "../../config.h"
#include "../Entity.h"


namespace ezg {


	class Hero
	{

	public:

		//default constructor
		Hero() noexcept
			: m_hp(0)
			, is_gravity(true)
			, speed_x(0)
			, speed_y(0)
			, m_direction(Direction::Right)
			, m_status(EntityStat::InAir)
		{ /* nop */ }

		//draws a hero texture
		void draw (sf::RenderTarget& target, sf::RenderStates states);


		//makes the hero jump ...
		void jump   ();
		//jump off (for example, from a platform(SolidAbove))
		void jumpOff();


		//shooting function.
		//returns a pointer to the bullet.
		std::unique_ptr<Entity> fire();


		//take a hit.
		void getHit(Hit _damage);


		//determines the interaction of the hero with other objects
		std::unique_ptr<Entity> colision(Entity* const _entity, Direction _dir);


		//////////////////////////////////////////////////
		//hero control
		void checkKeyBoard();
		//hero control
		void checkEvent(const sf::Event& _event);
		//////////////////////////////////////////////////


		//////////////////////////////////////////////////
		//the function updates the state of the hero:
		//
		//updates the hero's position.
		//	_time - period of time (S = time * speed_dir)
		//	_dir  - direction (horizontally or vertically)
		void upPosition(float _time, Direction _dir) noexcept;
		//updates the current effect
		void upEffect(float _time);
		// - if the hero doesn't move, it sets the idle animation
		// - calls upEffect(float _time)
		// - calls update for m_animation
		// - checks if the hero is dead
		void otherUpdate(float _time);
		//////////////////////////////////////////////////


		//move the hero to _x and _y
		void move(float _x, float _y) noexcept { m_hit_box.left += _x; /**/ m_hit_box.top += _y; }


		//returns a reference to the class Animation. 
		//used to define the animation for the hero.
		inline Animation& atAnimation() noexcept { return m_animation; }


		//////////////////////////////////////////////////
		//set functions:
		//void setEffect(const Effect& _effect) noexcept { m_effects[_effect._type] = _effect; }
		//activates the desired animation when setting a new hero state
		void setStat    (EntityStat _new);
		//
		void setHp      (float _hp)          noexcept { m_hp = _hp; }
		//
		void setPosition(float _x, float _y) noexcept { m_hit_box.left = _x; /**/ m_hit_box.top = _y; }
		//
		void setWidth   (float _width)       noexcept { m_hit_box.width = _width;   }
		void setHeight  (float _height)      noexcept { m_hit_box.height = _height; }
		//////////////////////////////////////////////////


		//////////////////////////////////////////////////
		//get functions:
		inline const float getPosX() const noexcept { return m_hit_box.left; }
		inline const float getPosY() const noexcept { return m_hit_box.top;  }
		//
		inline const EntityStat getStat() const noexcept { return m_status; }
		//////////////////////////////////////////////////


		//the function returns the hero to its original state
		void restart();

	private:


		bool _effectIsActive_(EffectType _eff) noexcept;

	private:

		Animation		m_animation;

		EntityStat		m_status;
		std::map<EffectType, Effect> m_effects;
		Direction		m_direction;

		bool			is_gravity;

		float			speed_x;
		float			speed_y;

		sf::FloatRect	m_hit_box;

		float			m_hp;

	}; // class Hero


} //namespace ezg