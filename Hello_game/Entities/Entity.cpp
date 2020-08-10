#include "Entity.h"

namespace ezg {

	int Entity::__id__ = 0;

	Entity::Entity (EntityType _tipe, int _id) noexcept
		: m_tipe     (_tipe)
		, m_id       (__id__)
		, is_gravity (true)
		, m_alive    (true)
	{
		__id__++;
	}


	Entity::Entity (EntityType _tipe, float pos_x, float pos_y, float _width, float _height) noexcept
		: m_hit_box  (pos_x, pos_y, _width, _height) //-_-
		, is_gravity (true)
		, m_id       (__id__)
		, m_tipe     (_tipe)
		, m_alive    (true)
	{
		__id__++;
	}


	void 
	Entity::moveIt (float _x, float _y) noexcept {

		m_hit_box.left += _x;
		m_hit_box.top += _y;

	}


	/*static*/ bool Entity::intersection(sf::CircleShape _circle, sf::Vector2f _point) {

		bool res = true;

		const float _x = _circle.getPosition().x - _point.x;
		const float _y = _circle.getPosition().y - _point.y;

		res = res && std::fabs(_x) <= _circle.getRadius();//in case _x and _y are 
		res = res && std::fabs(_y) <= _circle.getRadius();//very large
		res = res && _x * _x + _y * _y < _circle.getRadius()* _circle.getRadius();

		return res;
	}

#define EScase(a) case EntityStat::a: \
 res = #a; \
 break;
	std::string enumName(EntityStat _en) {

		std::string res;

		switch (_en)
		{
			EScase(Death);
			EScase(InAir);
			EScase(onSolid);
			EScase(onSolidAbove);
			EScase(onStairs);

		default:
			res = std::to_string(static_cast<int>(_en));
			break;
		}

		return res;
	}

#define EAcase(a) case EntityAnimation::a: \
 res = #a; \
 break;
	std::string enumName(EntityAnimation _en) {

		std::string res;

		switch (_en)
		{
			EAcase(Idle);
			EAcase(Jump);
			EAcase(Walk);
			EAcase(Wounded);
			EAcase(Attack);

		default:
			res = std::to_string(static_cast<int>(_en));
			break;
		}

		return res;
	}

#define Ecase(a) case EntityType::a: \
 res = #a; \
 break;
	std::string enumName(EntityType _en) {

		std::string res;

		switch (_en)
		{
			Ecase(Solid);
			Ecase(SolidAbove);
			Ecase(Stairs);

			Ecase(Needle);

			Ecase(Bullet);
			Ecase(HeroBullet);
			Ecase(RedBullet);
			Ecase(BlueBullet);

			Ecase(Bot);
			Ecase(Bee);
			Ecase(Snake);
			Ecase(MushroomRed);
			Ecase(MushroomBlue);

		default:
			res = std::to_string(static_cast<int>(_en));
			break;
		}

		return res;
	}

#define ETcase(a) case EffectType::a: \
 res = #a; \
 break;
	std::string enumName(EffectType _en) {

		std::string res;

		switch (_en)
		{
			ETcase(Nop);
			ETcase(Attack);
			ETcase(Aggression);
			ETcase(Discarding);
			ETcase(Freezing);
			ETcase(OnFire);
			ETcase(Poisoning);
			ETcase(Stop);
			ETcase(NoJump);
			ETcase(Walking);
			ETcase(Wounded);

		default:
			res = std::to_string(static_cast<int>(_en));
			break;
		}
		return res;
	}



	std::ostream& operator<<(std::ostream& _stream, EntityStat _en) {
		_stream << enumName(_en);
		return _stream;
	}
	std::ostream& operator<<(std::ostream& _stream, EntityAnimation _en) {
		_stream << enumName(_en);
		return _stream;
	}
	std::ostream& operator<<(std::ostream& _stream, EntityType _en) {
		_stream << enumName(_en);
		return _stream;
	}
	std::ostream& operator<<(std::ostream& _stream, EffectType _en) {
		_stream << enumName(_en);
		return _stream;
	}
} // namespace ezg