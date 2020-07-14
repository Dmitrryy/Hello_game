#include "Animation.h"


namespace ezg {


	Animation::Animation() noexcept
		: is_active(false)
		, m_active(0)
		, m_time(0)
		, m_texture()
		, m_animations()
		, m_current_frame(0)
	{

	}



	sf::Sprite Animation::getSprite() const {
		sf::Sprite result;

		if (is_active) {

			result.setTexture(m_texture);
			result.setTextureRect(m_animations.at(m_active).at(m_current_frame).m_rect);

		}

		return result;
	}


	void Animation::addFrame(int _id_animation, sf::IntRect _rect, float _time) {

		m_animations.at(_id_animation).push_back(Frame(_time, _rect));

	}


	bool Animation::activate(int _id_animation) {

		const auto res = m_animations.find(_id_animation);

		if (res == m_animations.end() || res->second.size() == 0) {

			return false;
		}

		m_active = _id_animation;
		m_time = 0;
		m_current_frame = 0;

		is_active = true;
		return true;
	}


	void Animation::update(float _time) {

		if (is_active) {

			m_time += _time;

			if (m_time > m_animations.at(m_active).at(m_current_frame).m_time) {
				m_time -= m_animations.at(m_active).at(m_current_frame).m_time;

				m_current_frame++;

				m_current_frame %= m_animations.at(m_active).size();
			}
		}

	}


} // namespace ezg