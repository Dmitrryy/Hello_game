#include <cmath>

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

				const float diff_x = _lhs->getPosX() - m_hit_box.left;
				const float diff_y = _lhs->getPosY() - m_hit_box.top;


				float m_corner = std::atan(diff_y / diff_x);

				if (diff_y < 0.f && diff_x < 0.f) {
					m_corner -= 3.141592;
				}
				else if (diff_x < 0.f && diff_y > 0.f) {
					m_corner = 3.141592 + m_corner;
				}

				Hero* _hr = dynamic_cast<Hero*>(_lhs);
				_hr->getHit(Hit(m_damage, Effect(Effect::Type::Discarding, m_corner, 100.f, 0.12f)));
			}
		}

		return nullptr;
	}


} // namespace ezg