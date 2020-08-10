#include "Needle.h"

namespace ezg {

	Needle::Needle(float place_x, float place_y, float width, float height) noexcept
		: Entity(EntityType::Needle, place_x, place_y, width, height)
		, m_damage(50)
	{
		is_gravity = false;
	}

} // namespace ezg