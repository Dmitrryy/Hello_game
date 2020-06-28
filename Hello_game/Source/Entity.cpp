#include "../Header/Entity.h"


namespace ezg {

	Entity::Entity (TipeEntity _tipe) noexcept
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
	Entity::SetSprite(const std::string& texture, int location_x, int location_y, int width, int height) {

		if (!m_texture.loadFromFile(texture)) {
			std::cerr << "\n error loading hero texture \n";
		}

		m_sprite.setTexture(m_texture);
		m_sprite.setTextureRect(sf::IntRect(location_x, location_y, width, height));
	}


	void 
	Entity::moveIt (float _x, float _y) noexcept {

		m_hit_box.left += _x;
		m_hit_box.top += _y;

	}


	/* virtual */ void 
	Entity::setSpeed(float _sp_x, float _sp_y) noexcept {

		speed_x = _sp_x;
		speed_y = _sp_y;
	}


	void 
	Entity::upPosition (float time, Direction _dir) noexcept {

		if (_dir == Direction::Vertical) {

			if (is_gravity && !on_ground) { //если под гравитацией и не на земле

				speed_y += static_cast<float>(0.0005) * time;
				moveIt(0, speed_y * time);

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