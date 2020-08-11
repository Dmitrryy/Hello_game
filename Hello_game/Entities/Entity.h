#pragma once


#include "../config.h"


namespace ezg {

	///////////////////////////////////////////
	//the main class of any object of the game
	class Entity
	{
	public:
		enum class Type {
			Hero
			
			, Landscape
			, Solid
			, SolidAbove
			, Stairs

			//, Trap
			, Needle

			, Bullet
			, HeroBullet
			, RedBullet
			, BlueBullet

			, Bot
			, Bee
			, Snake
			, MushroomRed
			, MushroomBlue
		};
		enum class Stat {
			Death
			, InAir
			, onSolid
			, onSolidAbove
			, onStairs
		};
		//
		enum class Animation {
			Idle
			, Jump
			, Walk
			, Wounded
			, Attack
		};


		struct Effect;
		struct Hit;

		Entity            (const Entity& _that) = delete; //not saported
		Entity& operator= (const Entity&)       = delete; //not saported
		Entity            (Entity&& _that)      = delete; //not saported
		Entity& operator= (Entity&&)            = delete; //not saported

		virtual ~Entity() = default;

		Entity (Type _tipe, int _id = 0) noexcept;
		Entity (Type _tipe, float pos_x, float pos_y, float _width, float _height) noexcept;


	public:


		inline const float          getPosX    () const noexcept { return m_hit_box.left;  }
		inline const float          getPosY    () const noexcept { return m_hit_box.top;   }
		inline const sf::FloatRect  getHitBox  () const noexcept { return m_hit_box;       }
		inline const Type     getType    () const noexcept { return m_tipe;          }
		int getID() const noexcept { return m_id; }
		int setID() noexcept { return m_id; }

		//does gravity affect an object
		inline const bool isGravity () const noexcept { return is_gravity; }

		
		//to draw the object
		virtual void draw (sf::RenderTarget& target, sf::RenderStates states) const = 0;
	

		//check intersection with another object
		//and determine the interaction
		virtual std::unique_ptr<Entity> colision(Entity* _lhs, Direction _dir) = 0;


		//position update virtual function
		virtual void upPosition(float time, Direction _dir) noexcept { }


		void setPosition (float _x, float _y) noexcept { m_hit_box.left = _x; /**/ m_hit_box.top = _y; }


		//m_alive == false - the entity can be deleted
		inline const bool alive() const noexcept { return m_alive; }
		void setAlive(bool _new) noexcept { m_alive = _new; }


		//&@#
		virtual void otherUpdate(float _time) = 0;


		static bool intersection(sf::CircleShape _circle, sf::Vector2f _point);


		virtual std::string DebugStr() { return "empty. id: " + std::to_string(m_id); };


	protected:

		//set hit box...
		void setHitBox (sf::FloatRect _rect) noexcept { m_hit_box = _rect;          }
		void setWidth  (float _width)        noexcept { m_hit_box.width = _width;   }
		void setHeight (float _height)       noexcept { m_hit_box.height = _height; }


		//function for... move object
		void moveIt   (float _x, float _y)   noexcept;



	protected:

		Type		m_tipe;

		int             m_id;
		static int      __id__;

		bool			m_alive;

		bool			is_gravity;

		sf::FloatRect	m_hit_box;
		
	}; // class Entity


	struct Entity::Effect {

		enum class Type {
			Nop
			, Discarding
			, Wounded
			, Aggression
			, Walking
			, CantJump
			, CantFire
			, Stop
			, Attack
			, Poisoning
			, OnFire
			, Freezing
		};

		explicit Effect(Type _tp = Type::Nop, float _prop = 0.f, float _pow = 0.f, float _time = 0.f) noexcept
			: _type(_tp)
			, _power(_pow)
			, _property(_prop)
			, _time_effect(_time)
		{}

		Type _type;

		float _power;
		//Discording - corner; 
		//Poisoning && OnFire - number of gifts
		float _property;
		float _time_effect;

	};


	struct Entity::Hit {

		Hit(float _dmg = 0.f, Effect _eff1 = Effect(), Effect _eff2 = Effect(), Effect _eff3 = Effect(), Effect _eff4 = Effect()) noexcept
			: _damage(_dmg)
			, _effect{ _eff1, _eff2, _eff3, _eff4 }
		{}

		float _damage;

		Effect _effect[4];

	};


	std::string enumName(Entity::Stat _en);
	std::string enumName(Entity::Animation _en);
	std::string enumName(Entity::Type _en);
	std::string enumName(Entity::Effect::Type _en);

	std::ostream& operator<<(std::ostream& _stream, Entity::Stat _en);
	std::ostream& operator<<(std::ostream& _stream, Entity::Animation _en);
	std::ostream& operator<<(std::ostream& _stream, Entity::Type _en);
	std::ostream& operator<<(std::ostream& _stream, Entity::Effect::Type _en);


} //namespace ezg