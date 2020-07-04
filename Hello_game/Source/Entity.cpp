#include "../Header/Entity.h"


namespace ezg {


	Entity::Entity (TipeEntity _tipe) noexcept
		: m_tipe		(_tipe)
		, is_gravity	(true)
	{

	}


	Entity::Entity (TipeEntity _tipe, float pos_x, float pos_y, float _width, float _height)
		: m_hit_box  (pos_x, pos_y, _width, _height)
		, is_gravity (true)
		, m_tipe     (_tipe)
	{

	}


	void 
	Entity::moveIt (float _x, float _y) noexcept {

		m_hit_box.left += _x;
		m_hit_box.top += _y;

	}


} // namespace ezg