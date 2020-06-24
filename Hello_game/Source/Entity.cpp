#include "../Header/Entity.h"


namespace ezg {

	Entity::Entity (TipeEntity _tipe)
		: m_tipe(_tipe)
		, speed_x(0)
		, speed_y(0)
		, is_gravity(true)
		, on_ground(false)
	{

	}


	Entity::Entity (TipeEntity _tipe, float pos_x, float pos_y, float _width, float _height)
		: m_hit_box  (pos_x, pos_y, _width, _height)
		, speed_x    (0)
		, speed_y    (0)
		, is_gravity (true)
		, on_ground  (false)
		, m_tipe     (_tipe)
	{

	}

	void 
	Entity::SetSprite(sf::Texture& texture, int location_x/* = 0*/, int location_y/* = 0*/) {

		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(location_x, location_y, m_hit_box.width, m_hit_box.top));
		//sprite.setPosition(pos_x, pos_y);

	}


	void 
	Entity::moveIt (float _x, float _y) noexcept {

		m_hit_box.left += _x;
		m_hit_box.top += _y;

	}


	void 
	Entity::upPosition (float time, Direction _dir) noexcept {

		if (_dir == Direction::Vertical) {

			if (is_gravity && !on_ground) { //если под гравитацией и не на земле

				moveIt(0, speed_y * time);
				speed_y += 0.0005 * time;

			}
			if (!is_gravity) {
				moveIt(0, speed_y * time);
			}
		}

		else if (_dir == Direction::Horixontal) {

			moveIt(speed_x * time, 0);

			if (is_gravity) {
				on_ground = false;
			}

		}
	}

} // namespace ezg