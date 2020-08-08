#include "MushroomRed.h"

#include "../../Bullets/Bullets.h"

namespace ezg {

	void MushroomRed::_setAnimations_() /*override*/ {

		m_animation.addAnimation(static_cast<int>(EntityAnimation::Idle));
		m_animation.addFrame(static_cast<int>(EntityAnimation::Idle), sf::IntRect(1, 30, 6, 2), 1000000.f);

		m_animation.addAnimation(static_cast<int>(EntityAnimation::Attack));
		m_animation.addFrame(static_cast<int>(EntityAnimation::Attack), sf::IntRect(33, 30, 6, 2), 0.1f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Attack), sf::IntRect(41, 29, 6, 3), 0.2f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Attack), sf::IntRect(49, 27, 6, 5), 0.2f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Attack), sf::IntRect(57, 26, 6, 6), 0.4f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Attack), sf::IntRect(49, 27, 6, 5), 0.2f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Attack), sf::IntRect(41, 29, 6, 3), 0.2f);
		m_animation.addFrame(static_cast<int>(EntityAnimation::Attack), sf::IntRect(33, 30, 6, 2), 0.1f);
	}


	std::unique_ptr<Entity> MushroomRed::fire(float _x, float _y) /*override*/ {

		setStat(EntityAnimation::Attack);

		if (m_animation.getActive() == static_cast<int>(EntityAnimation::Attack) && m_time >= 1.4f) {
			m_time = 0;

			if (_x >= m_hit_box.left) {
				m_direction = Direction::Right;
			}
			else {
				m_direction = Direction::Left;
			}

			std::unique_ptr<RedBullet> bullet(std::make_unique<RedBullet>());

			const float _y = m_hit_box.top + 4;
			if (m_direction == Direction::Left) {
				bullet->setPosition(m_hit_box.left - 1, _y);
				bullet->setSpeed(-90.f, 0);
			}
			else {
				bullet->setPosition(m_hit_box.left + 6, _y);
				bullet->setSpeed(90.f, 0);
			}
			return bullet;
		}
		return nullptr;
	}

} // namespace ezg