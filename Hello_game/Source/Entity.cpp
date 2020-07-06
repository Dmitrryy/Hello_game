#include "../Header/Entity.h"
#include "../Header/Bullets.h"
#include "../Header/Solid.h"
#include "../Header/Enemy.h"

namespace ezg {


	Entity::Entity (TipeEntity _tipe) noexcept
		: m_tipe		(_tipe)
		, is_gravity	(true)
		, m_alive(true)
	{

	}


	Entity::Entity (TipeEntity _tipe, float pos_x, float pos_y, float _width, float _height)
		: m_hit_box  (pos_x, pos_y, _width, _height)
		, is_gravity (true)
		, m_tipe     (_tipe)
		, m_alive(true)
	{
		//std::cout << "created: " << (int)_tipe << std::endl;
	}


	void 
	Entity::moveIt (float _x, float _y) noexcept {

		m_hit_box.left += _x;
		m_hit_box.top += _y;

	}


	void Entity::colision(gsl::not_null <Entity*> _lhs_ent, gsl::not_null <Entity*> _rhs_ent, Direction _dir) {

		if (_lhs_ent->getTipe() == TipeEntity::HeroBullet && _rhs_ent->getTipe() == TipeEntity::Solid) {

			_HeroBulletAndSolid_(dynamic_cast<HeroBullet*>(_lhs_ent.get()), dynamic_cast<Solid*>(_rhs_ent.get()), _dir);
		}
		else if (_lhs_ent->getTipe() == TipeEntity::HeroBullet && _rhs_ent->getTipe() == TipeEntity::MushroomRed) {

			_HeroBulletAndMushroomRed_(dynamic_cast<HeroBullet*>(_lhs_ent.get()), dynamic_cast<MushroomRed*>(_rhs_ent.get()), _dir);
		}
		else if (_lhs_ent->getTipe() == TipeEntity::HeroBullet && _rhs_ent->getTipe() == TipeEntity::Bee) {

			_HeroBulletAndBee_(dynamic_cast<HeroBullet*>(_lhs_ent.get()), dynamic_cast<Bee*>(_rhs_ent.get()), _dir);
		}
		
	}


	void Entity::_HeroBulletAndSolid_(HeroBullet* _bullet, Solid* _solid, Direction _dir) {

		if (_bullet->getHitBox().intersects(_solid->getHitBox()) && _dir == Direction::Horixontal) {
			_bullet->setAlive(false);
		}

	}


	void Entity::_HeroBulletAndMushroomRed_(HeroBullet* _bullet, MushroomRed* _mr, Direction _dir) {

		if (_bullet->getHitBox().intersects(_mr->getHitBox()) && _dir == Direction::Horixontal) {

			_mr->getHit(_bullet->getDamage());

			_bullet->setAlive(false);
		}

	}


	void Entity::_HeroBulletAndBee_(HeroBullet* _bullet, Bee* _bee, Direction _dir) {

		if (_bullet->getHitBox().intersects(_bee->getHitBox()) && _dir == Direction::Horixontal) {

			_bee->getHit(_bullet->getDamage());

			_bullet->setAlive(false);
		}

	}


} // namespace ezg