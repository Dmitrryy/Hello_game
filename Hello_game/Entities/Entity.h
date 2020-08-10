#pragma once


#include "../config.h"


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
	enum class EntityAnimation {
		Idle
		, Jump
		, Walk
		, Wounded
		, Attack
	};
	//available game object types
	enum class EntityType {
		  Solid
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

	enum class EffectType {
		  Nop
		//, Gravity
		, Discarding
		, Wounded
		, Aggression
		//, Immunity
		, Walking
		, NoJump
		, Stop
		, Attack
		, Poisoning
		, OnFire
		, Freezing
	};

	std::string enumName(EntityStat _en);
	std::string enumName(EntityAnimation _en);
	std::string enumName(EntityType _en);
	std::string enumName(EffectType _en);

	std::ostream& operator<<(std::ostream& _stream, EntityStat _en);
	std::ostream& operator<<(std::ostream& _stream, EntityAnimation _en);
	std::ostream& operator<<(std::ostream& _stream, EntityType _en);
	std::ostream& operator<<(std::ostream& _stream, EffectType _en);
	////////////////////////////////////////////////


	struct Effect {

		explicit Effect(EffectType _tp = EffectType::Nop, float _prop = 0.f, float _pow = 0.f, float _time = 0.f) noexcept
			: _type(_tp)
			, _power(_pow)
			, _property(_prop)
			, _time_effect(_time)
		{}

		EffectType _type;

		float _power;
		//Discording - corner; 
		//Poisoning && OnFire - number of gifts
		float _property;
		float _time_effect;

	};


	struct Hit {

		Hit(float _dmg = 0.f, Effect _eff1 = Effect(), Effect _eff2 = Effect(), Effect _eff3 = Effect(), Effect _eff4 = Effect()) noexcept
			: _damage(_dmg)
			//, _dir(Direction::Non)
			, _effect{ _eff1, _eff2, _eff3, _eff4 }
		{}

		float _damage;
		//Direction _dir;

		Effect _effect[4];

	};


	///////////////////////////////////////////
	//the main class of any object of the game
	class Entity
	{

	public:

		Entity            (const Entity& _that) = delete; //not saported
		Entity& operator= (const Entity&)       = delete; //not saported
		Entity            (Entity&& _that)      = delete; //not saported
		Entity& operator= (Entity&&)            = delete; //not saported

		virtual ~Entity() = default;

		Entity (EntityType _tipe, int _id = 0) noexcept;
		Entity (EntityType _tipe, float pos_x, float pos_y, float _width, float _height) noexcept;


	public:


		inline const float          getPosX    () const noexcept { return m_hit_box.left;  }
		inline const float          getPosY    () const noexcept { return m_hit_box.top;   }
		inline const sf::FloatRect  getHitBox  () const noexcept { return m_hit_box;       }
		inline const EntityType     getType    () const noexcept { return m_tipe;          }
		int getID() const noexcept { return m_id; }
		int setID() noexcept { return m_id; }

		//does gravity affect an object
		inline const bool isGravity () const noexcept { return is_gravity; }

		
		//to draw the object
		virtual void draw (sf::RenderTarget& target, sf::RenderStates states) const = 0;
	

		//check intersection with another object
		//and determine the interaction
		virtual void colision(gsl::not_null <Entity*> _lhs, Direction _dir) = 0;


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

		EntityType		m_tipe;
		int             m_id;
		static int      __id__;

		bool			m_alive;

		bool			is_gravity;

		sf::FloatRect	m_hit_box;
		
	}; // class Entity



} //namespace ezg