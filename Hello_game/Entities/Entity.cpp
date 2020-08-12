#include "Entity.h"
#include "Solid/Solid.h"
#include "Bots/Mushroom/MushroomRed.h"
#include "Bots/Mushroom/MushroomBlue.h"

namespace ezg {


	Entity::Entity (Type _tipe, int _id) noexcept
		: m_tipe     (_tipe)
		, m_id       (0)
		, is_gravity (true)
		, m_alive    (true)
	{
	}


	Entity::Entity (Type _tipe, float pos_x, float pos_y, float _width, float _height) noexcept
		: m_hit_box  (pos_x, pos_y, _width, _height) //-_-
		, is_gravity (true)
		, m_id       (0)
		, m_tipe     (_tipe)
		, m_alive    (true)
	{
	}


	void 
	Entity::moveIt (float _x, float _y) noexcept {

		m_hit_box.left += _x;
		m_hit_box.top += _y;

	}


	///*static*/ std::unique_ptr<Entity> loadFromXML(tinyxml2::XMLElement* _elem) {
	//	if (_elem == nullptr) { return nullptr; }

	//	std::unique_ptr<Entity> res = nullptr;
	//	
	//	const int64_t id = _elem->Int64Attribute("id");
	//	const int64_t x = _elem->Int64Attribute("x");
	//	const int64_t y = _elem->Int64Attribute("y");

	//	const int64_t width = _elem->Int64Attribute("width");
	//	const int64_t height = _elem->Int64Attribute("height");

	//	if (_elem->Attribute("type") == nullptr) {
	//		
	//		std::cout << "unknown tipe object with id: " << id << std::endl;
	//		std::cout << "line num: " << _elem->GetLineNum() << std::endl;
	//	}
	//	else if (0 == std::strcmp(_elem->Attribute("type"), "solid")) {

	//		res = std::move(std::make_unique <Solid>(x, y, width, height));
	//		res->setID(id);
	//	}
	//	else if (0 == std::strcmp(_elem->Attribute("type"), "solidabove")) {

	//		res = std::move(std::make_unique <SolidAbove>(x, y, width, height));
	//		res->setID(id);
	//	}
	//	else if (0 == std::strcmp(_elem->Attribute("type"), "stairs")) {

	//		res = std::move(std::make_unique <Stairs>(x, y, width, height));
	//		res->setID(id);
	//	}
	//	else if (0 == std::strcmp(_elem->Attribute("type"), "MushroomRed")) {

	//		int64_t pos_x = 0;
	//		int64_t pos_y = 0;
	//		int64_t hp    = 0;
	//		int64_t speed = 0;

	//		tinyxml2::XMLElement* pr = _elem->FirstChildElement("properties");
	//		if (pr != nullptr) {
	//			pr = pr->FirstChildElement("property");
	//			while (pr != nullptr) {

	//				if (0 == std::strcmp(pr->Attribute("name"), "spawn_x")) {
	//					pos_x = pr->Int64Attribute("value");
	//				}
	//				else if (0 == std::strcmp(pr->Attribute("name"), "spawn_y")) {
	//					pos_y = pr->Int64Attribute("value");
	//				}
	//				else if (0 == std::strcmp(pr->Attribute("name"), "hp")) {
	//					hp = pr->Int64Attribute("value");
	//				}
	//				else if (0 == std::strcmp(pr->Attribute("name"), "speed")) {
	//					speed = pr->Int64Attribute("value");
	//				}

	//				pr = pr->NextSiblingElement();
	//			}
	//		}


	//		res = std::move(std::make_unique<MushroomRed>(pos_x, pos_y, sf::IntRect(x, y, width, height), m_enemy_texture)));
	//	}

	//	return res;
	//}


	/*static*/ bool Entity::intersection(sf::CircleShape _circle, sf::Vector2f _point) {

		bool res = true;

		const float _x = _circle.getPosition().x - _point.x;
		const float _y = _circle.getPosition().y - _point.y;

		res = res && std::fabs(_x) <= _circle.getRadius();//in case _x and _y are 
		res = res && std::fabs(_y) <= _circle.getRadius();//very large
		res = res && _x * _x + _y * _y < _circle.getRadius()* _circle.getRadius();

		return res;
	}




#define EScase(a) case Entity::Stat::a: \
 res = #a; \
 break;
	std::string enumName(Entity::Stat _en) {

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

#define EAcase(a) case Entity::Animation::a: \
 res = #a; \
 break;
	std::string enumName(Entity::Animation _en) {

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

#define Ecase(a) case Entity::Type::a: \
 res = #a; \
 break;
	std::string enumName(Entity::Type _en) {

		std::string res;

		switch (_en)
		{
			Ecase(Landscape);
			Ecase(Needle);
			Ecase(Bullet);

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

#define ETcase(a) case Entity::Effect::Type::a: \
 res = #a; \
 break;
	std::string enumName(Entity::Effect::Type _en) {

		std::string res;

		switch (_en)
		{
			ETcase(Nop);
			ETcase(Attack);
			ETcase(CantFire);
			ETcase(Aggression);
			ETcase(Discarding);
			ETcase(Freezing);
			ETcase(OnFire);
			ETcase(Poisoning);
			ETcase(CantJump);
			ETcase(Walking);
			ETcase(Wounded);

		default:
			res = std::to_string(static_cast<int>(_en));
			break;
		}
		return res;
	}



	std::ostream& operator<<(std::ostream& _stream, Entity::Stat _en) {
		_stream << enumName(_en);
		return _stream;
	}
	std::ostream& operator<<(std::ostream& _stream, Entity::Animation _en) {
		_stream << enumName(_en);
		return _stream;
	}
	std::ostream& operator<<(std::ostream& _stream, Entity::Type _en) {
		_stream << enumName(_en);
		return _stream;
	}
	std::ostream& operator<<(std::ostream& _stream, Entity::Effect::Type _en) {
		_stream << enumName(_en);
		return _stream;
	}
} // namespace ezg