#include "MushroomBlue.h"

#include "../../Bullets/Bullets.h"

namespace ezg {

	void MushroomBlue::_setAnimations_() /*override*/ {

		m_animation.addAnimation(static_cast<int>(EntityAnimation::Idle));
		m_animation.addFrame(static_cast<int>(EntityAnimation::Idle), sf::IntRect(1, 38, 6, 2), 1000000.f);

		m_animation.addAnimation(static_cast<int>(EntityAnimation::Attack));
		m_animation.addFrame(static_cast<int>(EntityAnimation::Attack), sf::IntRect(33, 38, 6, 2), 0.1f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Attack), sf::IntRect(41, 37, 6, 3), 0.2f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Attack), sf::IntRect(49, 35, 6, 5), 0.2f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Attack), sf::IntRect(57, 34, 6, 6), 0.4f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Attack), sf::IntRect(49, 35, 6, 5), 0.2f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Attack), sf::IntRect(41, 37, 6, 3), 0.2f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Attack), sf::IntRect(33, 38, 6, 2), 0.1f);

	}


	std::unique_ptr<Entity> MushroomBlue::fire(float _x, float _y) /*override*/ {

		setStat(EntityAnimation::Attack);

		if (m_animation.getActive() == static_cast<int>(EntityAnimation::Attack) && m_time >= 1.4f) {
			m_time = 0;

			if (_x >= m_hit_box.left) {
				m_direction = Direction::Right;
			}
			else {
				m_direction = Direction::Left;
			}

			std::unique_ptr<BlueBullet> bullet(std::make_unique<BlueBullet>());

			//float _y = m_hit_box.top + 4;


			const float diff_x = _x - m_hit_box.left;
			const float diff_y = _y - m_hit_box.top;


			float _corner = std::atan(diff_y / diff_x);

			if (diff_y < 0.f && diff_x < 0.f) {
				_corner -= 3.141592;
			}
			else if (diff_x < 0.f && diff_y > 0.f) {
				_corner = 3.141592 + _corner;
			}

			bullet->setSpeed(90.f * std::cos(_corner), 90.f * std::sin(_corner));

			if (m_direction == Direction::Left) {
				bullet->setPosition(m_hit_box.left - 1, m_hit_box.top + 4);
			}
			else {
				bullet->setPosition(m_hit_box.left + 6, m_hit_box.top + 4);
			}

			return bullet;
		}
		return nullptr;

	}

} // namespace ezg