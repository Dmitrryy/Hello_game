#include <gsl/gsl>
#define GSL_THROW_ON_CONTRACT_VIOLATION

#include "Needle.h"
#include "../../Hero/Player.h"
namespace ezg {

	Needle::Needle(float place_x, float place_y, float width, float height) noexcept
		: Entity(Type::Needle, place_x, place_y, width, height)
		, m_damage(50)
	{
		is_gravity = false;
	}


	std::unique_ptr<Entity> Needle::colision(Entity* _lhs, Direction _dir) noexcept /*override*/ {

		if (_lhs == nullptr) { return nullptr; }

		if (_lhs->getType() == Type::Hero) {
			if (m_hit_box.intersects(_lhs->getHitBox())) {
				gsl::not_null<Hero*> _hr = dynamic_cast<Hero*>(_lhs);
				_hr->getHit(Hit(getDamage()));
			}
		}

		return nullptr;
	}


} // namespace ezg