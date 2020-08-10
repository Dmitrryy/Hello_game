#pragma once

#include "../../../config.h"
#include "../../Entity.h"


namespace ezg {

#define MUSHROOM_DURATION_WOUNDED 0.5f


	class Mushroom : public ezg::Entity
	{
	protected:

		/////////////////////////////////////////////////////////
		//constructor:
		// _tipe - tipe of mushroom (MushroomRed or MushroomBlue).
		// plece_x & place_y - the position of the upper left corner of the mushroom.
		// _area - attack zone.
		// _texture - texture... (its copy is kept. do not allow the destruction of the original)
		//
		//it is protected because constructor can only call derived classes
		Mushroom(EntityType _tipe, float place_x, float place_y, sf::IntRect _area, const sf::Texture& _texture);
		/////////////////////////////////////////////////////////


	public:

		/////////////////////////////////////////////////////////
		//draws a bot
		void draw(sf::RenderTarget& target, sf::RenderStates states) const noexcept override;
		/////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////
		//interaction with other objects
		void colision(gsl::not_null <Entity*> _lhs, Direction _dir) final override;
		/////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////
		//shooting function. returns an object to a bullet
		virtual std::unique_ptr<Entity> fire(float _x, float _y) = 0;
		/////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////
		//get hit...
		void getHit(Hit _hit) noexcept;
		/////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////
		//updates time for effects
		void upEffect(float _time) noexcept;
		/////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////
		// - updates the time of effects.
		// - updates the animation.
		// - checks if the object is alive.
		void otherUpdate(float _time) noexcept override;
		/////////////////////////////////////////////////////////



		void setDirection(Direction _dir) noexcept { m_direction = _dir; }
		void setStat(EntityAnimation _stat);
		//void setEffect(EffectType _effect) noexcept { m_effect = _effect; }


		sf::FloatRect getArea() const noexcept { return m_area_attack; }


		std::string DebugStr() override;


	private:


		virtual void _setAnimations_() = 0;


		/////////////////////////////////////////////////////////
		//updates the size of the hit box 
		//depending on the current frame of the animation
		void _upHitBox_();
		/////////////////////////////////////////////////////////


		bool _effectIsActive_(EffectType _eff) const {

			const auto res = m_effects.find(_eff);

			if (res != m_effects.end()) {
				return res->second._time_effect > 0.f;
			}
			return false;
		}


	protected:

		float m_time;
		float m_time_effect;

		float m_hp;

		Animation m_animation;
		std::map<EffectType, Effect>	m_effects;
		Direction m_direction;

		float m_damage;
		sf::FloatRect m_area_attack;
	};

} //namespace ezg;