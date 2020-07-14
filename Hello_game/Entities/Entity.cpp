#include "Entity.h"

namespace ezg {


	Entity::Entity (TypeEntity _tipe) noexcept
		: m_tipe     (_tipe)
		, is_gravity (true)
		, m_alive    (true)
	{

	}


	Entity::Entity (TypeEntity _tipe, float pos_x, float pos_y, float _width, float _height) noexcept
		: m_hit_box  (pos_x, pos_y, _width, _height) //-_-
		, is_gravity (true)
		, m_tipe     (_tipe)
		, m_alive    (true)
	{

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

} // namespace ezg