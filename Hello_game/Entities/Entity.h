#pragma once

#include "../config.h"
#include "../Libs/tinyxml2/tinyxml2.h"


namespace ezg {

	///////////////////////////////////////////
	//the main class of any object of the game
	class Entity
	{
	public:

		struct Effect;
		struct Hit;

		enum class Type;
		enum class Stat;
		enum class Animation;


	public:

		Entity            (const Entity& _that) = delete; //not saported
		Entity& operator= (const Entity&)       = delete; //not saported
		Entity            (Entity&& _that)      = delete; //not saported
		Entity& operator= (Entity&&)            = delete; //not saported

	public:

		virtual ~Entity() = default;

		Entity (Type _tipe, int _id = 0) noexcept;
		Entity (Type _tipe, float pos_x, float pos_y, float _width, float _height) noexcept;

	public:

		static std::unique_ptr<Entity> loadFromXML(tinyxml2::XMLElement* _elem);

	public:

		float         getPosX    () const noexcept { return m_hit_box.left;  }
		float         getPosY    () const noexcept { return m_hit_box.top;   }
		sf::Vector2f  getPosition() const noexcept { return sf::Vector2f(m_hit_box.left, m_hit_box.top); }
		sf::FloatRect getHitBox  () const noexcept { return m_hit_box;       }
		Type          getType    () const noexcept { return m_tipe;          }

		int           getID      () const noexcept { return m_id; }
		void          setID(int _id) noexcept { m_id = _id; }

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


		//set hit box...
		void setHitBox (sf::FloatRect _rect) noexcept { m_hit_box = _rect;          }
		void setWidth  (float _width)        noexcept { m_hit_box.width = _width;   }
		void setHeight (float _height)       noexcept { m_hit_box.height = _height; }


		//function for... move object
		void moveIt   (float _x, float _y)   noexcept;



	protected:

		Type		    m_tipe;

		int             m_id;

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

		Type  _type;
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

		float  _damage;
		Effect _effect[4];
	};


	enum class Entity::Type {
		Hero
		, Landscape
		, Bullet

		, Trap
		, Needle

		, Bot
		, Bee
		, Snake
		, MushroomRed
		, MushroomBlue
	};
	enum class Entity::Stat {
		Death
		, InAir
		, onSolid
		, onSolidAbove
		, onStairs
	};
	enum class Entity::Animation {
		Idle
		, Jump
		, Walk
		, Wounded
		, Attack
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