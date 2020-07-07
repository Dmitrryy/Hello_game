#include <cmath>

#include "../Header/Enemy.h"
#include "../Header/Bullets.h"

#define PI 3.141592

namespace ezg {


	Mushroom::Mushroom(TipeEntity _tipe, float place_x, float place_y, sf::IntRect _area, const sf::Texture& _texture)
		: Entity(_tipe, place_x, place_y, 6, 6)
		, m_direction(Direction::Right)
		, m_damage(20)
		, m_time(0.f)
		, m_time_effect(0.f)
		, m_hp(50)
		, m_area_attack(_area)
	{
		is_gravity = false;
		m_animation.setTexture(_texture);
		m_animation.activate(EntityAnimation::Idle);
	}



	void Mushroom::draw(sf::RenderTarget& target, sf::RenderStates states) noexcept /*override*/ {

		sf::Sprite _sprite = m_animation.getSprite();

		//expand the sprite if necessary
		if (m_direction == Direction::Left) {
			sf::IntRect _rec = _sprite.getTextureRect();

			_rec.left += _rec.width;
			_rec.width = -_rec.width;

			_sprite.setTextureRect(_rec);
		}

		//painted blue if the hero is injured
		if (m_effect == EntityEffect::Wounded) {
			_sprite.setColor(sf::Color::Blue);
		}

		_sprite.setPosition(getPosX(), getPosY());
		target.draw(_sprite, states);
	}


	void Mushroom::getHit(float _damage) noexcept {
		if (m_effect != EntityEffect::Wounded && _damage > 0.f) {
			m_hp -= _damage;
			m_effect = EntityEffect::Wounded;
		}
	}


	void Mushroom::upEffect(float _time) noexcept {


		if (m_effect == EntityEffect::Wounded) {

			m_time_effect += _time;
			if (m_time_effect > DURATION_WOUNDED) {

				m_effect = EntityEffect::Normal;
				m_time_effect = 0.f;
			}

		}
		else {
			m_time_effect = 0.f;
		}

	}


	void Mushroom::otherUpdate(float _time) noexcept /*override*/ {

		m_time += _time;


		upEffect(_time);

		m_animation.update(_time);
		_upHitBox_();


		if (m_hp <= 0) {
			m_alive = false;
		}

	}


	void Mushroom::setStat(EntityAnimation _stat) {

		if (_stat != m_animation.getActive()) {
			m_time = 700.f;
			m_animation.activate(_stat);
		}

	}


	void Mushroom::_upHitBox_() {

		sf::IntRect _rec = m_animation.getSprite().getTextureRect();

		moveIt(0, m_hit_box.height - _rec.height);
		m_hit_box.height = _rec.height;


	}


	void MushroomRed::_setAnimations_() {

		m_animation.addAnimation(EntityAnimation::Idle);
		m_animation.addFrame(EntityAnimation::Idle, sf::IntRect(1, 30, 6, 2), 1000000.f);

		m_animation.addAnimation(EntityAnimation::Attack);
		m_animation.addFrame(EntityAnimation::Attack, sf::IntRect(33, 30, 6, 2), 100.f);
		m_animation.addFrame(EntityAnimation::Attack, sf::IntRect(41, 29, 6, 3), 200.f);
		m_animation.addFrame(EntityAnimation::Attack, sf::IntRect(49, 27, 6, 5), 200.f);
		m_animation.addFrame(EntityAnimation::Attack, sf::IntRect(57, 26, 6, 6), 400.f);
		m_animation.addFrame(EntityAnimation::Attack, sf::IntRect(49, 27, 6, 5), 200.f);
		m_animation.addFrame(EntityAnimation::Attack, sf::IntRect(41, 29, 6, 3), 200.f);
		m_animation.addFrame(EntityAnimation::Attack, sf::IntRect(33, 30, 6, 2), 100.f);
	}


	std::unique_ptr<Entity> MushroomRed::fire(float _x, float _y) {

		setStat(EntityAnimation::Attack);

		if (m_animation.getActive() == EntityAnimation::Attack && m_time >= 1400.f) {
			m_time = 0;

			if (_x >= m_hit_box.left) {
				m_direction = Direction::Right;
			}
			else {
				m_direction = Direction::Left;
			}

			std::unique_ptr<RedBullet> bullet(std::make_unique<RedBullet>());

			float _y = m_hit_box.top + 4;
			if (m_direction == Direction::Left) {
				bullet->setPosition(m_hit_box.left - 1, _y);
				bullet->setSpeed(-0.2f, 0);
			}
			else {
				bullet->setPosition(m_hit_box.left + 6, _y);
				bullet->setSpeed(0.2f, 0);
			}
			return bullet;
		}
		return nullptr;
	}
	///////////////////////////////////////Bee///////////////////////////////////////////


	Bee::Bee(float place_x, float place_y, sf::IntRect _area, float _radius, const sf::Texture& _texture)
		: Entity(TipeEntity::Bee, place_x, place_y, 8, 8)
		, m_direction(Direction::Right)
		, m_damage(30)
		, m_acceleration(0.00008f)
		, m_time_effect(0)
		, m_hp(40)
		, m_area(_area)
		, m_goto(0, 0)
		, m_radius(_radius)
		, m_effect(EntityEffect::Walking)
	{
		is_gravity = false;

		m_animation.setTexture(_texture);
		_setAnimations_();
		m_animation.activate(EntityAnimation::Walk);

		m_goto.x = std::rand() % static_cast<int>(m_area.width) + static_cast<int>(m_area.left);
		m_goto.y = std::rand() % static_cast<int>(m_area.height) + static_cast<int>(m_area.top);
	}


	void Bee::draw(sf::RenderTarget& target, sf::RenderStates states) noexcept /*override*/ {

		sf::Sprite _sprite = m_animation.getSprite();

		//expand the sprite if necessary
		if (m_direction == Direction::Left) {
			sf::IntRect _rec = _sprite.getTextureRect();

			_rec.left += _rec.width;
			_rec.width = -_rec.width;

			_sprite.setTextureRect(_rec);
		}

		//painted Red if the hero is injured
		if (m_effect == EntityEffect::Wounded) {
			_sprite.setColor(sf::Color::Red);
		}

		_sprite.setPosition(getPosX(), getPosY());
		target.draw(_sprite, states);
	}


	float Bee::attack(sf::FloatRect _rec) {

		sf::CircleShape _circle(m_radius);
		_circle.setPosition(m_hit_box.left, m_hit_box.top);

		const float _x = _rec.left;
		const float _y = _rec.top;

		if (intersection(_circle, sf::Vector2f(_x, _y)) && m_effect != EntityEffect::Wounded) {
			setEffect(EntityEffect::Attack);

			_goto_(_x, _y);

			if (_rec.intersects(m_hit_box)) {
				speed_x /= -1.3;
				speed_y /= -1.3;
				return m_damage;
			}
		}
		else if (m_effect != EntityEffect::Wounded && m_effect != EntityEffect::Walking){
			setEffect(EntityEffect::Normal);
		}

		return 0.f;
	}


	void Bee::getHit(float _damage) noexcept {
		if (m_effect != EntityEffect::Wounded && _damage > 0.f) {
			m_hp -= _damage;
			m_effect = EntityEffect::Wounded;
		}
	}


	void Bee::upPosition(float time, Direction _dir) noexcept /*override*/ {

		//static int goto_x = std::rand() % static_cast<int>(m_area.width) + static_cast<int>(m_area.left);
		//static int goto_y = std::rand() % static_cast<int>(m_area.height) + static_cast<int>(m_area.top);

		//printf("%g || %d %d || %g %g || %g %g\n", m_corner, goto_x, goto_y, getPosX(), getPosY(), speed_x, speed_y);

		if (m_effect == EntityEffect::Walking) {

			if (_dir == Direction::Horixontal) {
				_goto_(m_goto.x, m_goto.y);

				if (m_hit_box.intersects(sf::FloatRect(m_goto.x, m_goto.y, 5, 5))) {
					m_effect = EntityEffect::Normal;

					m_goto.x = std::rand() % static_cast<int>(m_area.width) + static_cast<int>(m_area.left);
					m_goto.y = std::rand() % static_cast<int>(m_area.height) + static_cast<int>(m_area.top);
				}
			}

		}

		if (_dir == Direction::Horixontal) {
			moveIt(speed_x * time, 0);
		}
		else if (_dir == Direction::Vertical) {
			moveIt(0, speed_y * time);
		}

	}


	void Bee::upEffect(float _time) noexcept {

		//static float _duration = 0.f;

		if (m_effect == EntityEffect::Wounded) {

			m_time_effect += _time;

			if (m_time_effect > DURATION_WOUNDED) {

				setEffect(EntityEffect::Normal);
				m_time_effect = _time = 0.f;
			}

		}
		else if (m_effect == EntityEffect::Normal) {

			m_time_effect += _time;

			if (m_time_effect < BEE_DURATION_NORMAL / 2.f) {

				speed_x -= speed_x / BEE_DURATION_NORMAL * 2.f;
				speed_y -= speed_y / BEE_DURATION_NORMAL * 2.f;
			}
			else {
				speed_x = speed_y = 0;
			}

			if (m_time_effect > BEE_DURATION_NORMAL) {

				setEffect(EntityEffect::Walking);
				m_time_effect = _time = 0.f;
			}
		}
		else {
			m_time_effect = 0.f;
		}
	}


	void Bee::otherUpdate(float _time) noexcept /*override*/ {

		//m_time_effect += _time;


		upEffect(_time);

		m_animation.update(_time);
		//_upHitBox_();

		if (m_acceleration * std::cos(m_corner) >= 0) {
			setDirection(Direction::Right);
		}
		else {
			setDirection(Direction::Left);
		}

		if (m_hp <= 0) {
			m_alive = false;
		}

	}


	void Bee::setEffect(EntityEffect _effect) noexcept {

		if (_effect == EntityEffect::Attack && m_effect != EntityEffect::Attack) {
			m_effect = _effect;
			m_radius *= 1.5f;
			m_acceleration *= 2;
		}
		else if (_effect == EntityEffect::Normal && m_effect != EntityEffect::Normal) {
			m_effect = _effect;
			m_radius /= 1.5f;
			m_acceleration /= 2;
		}
		else if (_effect == EntityEffect::Walking) {
			m_effect = _effect;

		}
	}


	void Bee::_goto_(float _x, float _y) noexcept {

		const float diff_x = _x - m_hit_box.left;
		const float diff_y = _y - m_hit_box.top;


		m_corner = std::atan(diff_y / diff_x);

		if (diff_y < 0.f && diff_x < 0.f) {
			m_corner -= 3.141592;
		}
		else if (diff_x < 0.f && diff_y > 0.f) {
			m_corner = 3.141592 + m_corner;
		}

		speed_x += m_acceleration * std::cos(m_corner);
		speed_y += m_acceleration * std::sin(m_corner);

		const float excess = std::sqrt(speed_x * speed_x + speed_y * speed_y - MAX_SPEED * MAX_SPEED);
		if (excess > 0) {
			speed_x -= excess * std::cos(m_corner);
			speed_y -= excess * std::sin(m_corner);
		}

	}


	void Bee::_setAnimations_() {

		m_animation.addAnimation(EntityAnimation::Walk);
		m_animation.addFrame(EntityAnimation::Walk, sf::IntRect(32, 16, 8, 8), 160.f);
		m_animation.addFrame(EntityAnimation::Walk, sf::IntRect(40, 16, 8, 8), 160.f);
		m_animation.addFrame(EntityAnimation::Walk, sf::IntRect(48, 16, 8, 8), 160.f);
		m_animation.addFrame(EntityAnimation::Walk, sf::IntRect(56, 16, 8, 8), 160.f);

	}


	/*static*/ bool Bee::intersection(sf::CircleShape _circle, sf::Vector2f _point) {

		bool res = true;

		const float _x = _circle.getPosition().x - _point.x;
		const float _y = _circle.getPosition().y - _point.y;

		res = res && std::fabs(_x) <= _circle.getRadius();//in case _x and _y are 
		res = res && std::fabs(_y) <= _circle.getRadius();//very large
		res = res && _x * _x + _y * _y < _circle.getRadius()* _circle.getRadius();

		return res;
	}



	///////////////////////////////////////Snake///////////////////////////////////////////

#define MAX_SPEED_SNAKE 0.04f

	Snake::Snake(float place_x, float place_y, float _distance_attack, const sf::Texture& _texture)
		: Entity(TipeEntity::Snake, place_x, place_y, 8, 8)
		, m_direction(Direction::Right)
		, m_damage(40)
		, m_time_effect(0)
		, m_hp(40)
		, m_goto(0, 0)
		, speed_x(0)
		, speed_y(0)
		, m_accelerationx(0.0001f)
		, m_distance_attack(_distance_attack)
		, m_effect(EntityEffect::Walking)
		, m_status(EntityStat::InAir)
	{
		is_gravity = true;

		m_animation.setTexture(_texture);
		_setAnimations_();
		m_animation.activate(EntityAnimation::Walk);

		m_goto.x = std::rand() % 80 + m_hit_box.left - 40;
		//m_goto.y = std::rand() % static_cast<int>(m_area.height) + static_cast<int>(m_area.top);
	}


	void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) noexcept /*override*/ {

		sf::Sprite _sprite = m_animation.getSprite();

		//expand the sprite if necessary
		if (m_direction == Direction::Left) {
			sf::IntRect _rec = _sprite.getTextureRect();

			_rec.left += _rec.width;
			_rec.width = -_rec.width;

			_sprite.setTextureRect(_rec);
		}

		//painted Red if the hero is injured
		if (m_effect == EntityEffect::Wounded) {
			_sprite.setColor(sf::Color::Red);
		}

		_sprite.setPosition(getPosX(), getPosY());
		target.draw(_sprite, states);
	}


	float Snake::attack(sf::FloatRect _rec) {

		sf::CircleShape _circle(m_distance_attack);
		_circle.setPosition(m_hit_box.left + m_hit_box.width / 2, m_hit_box.top + m_hit_box.height / 2);

		const float _x = _rec.left;
		const float _y = _rec.top;
		//std::cout << (int)m_effect << std::endl;
		if (Bee::intersection(_circle, sf::Vector2f(_x, _y)) && m_effect != EntityEffect::Wounded) {
			setEffect(EntityEffect::Attack);

			//_goto_(_x, _y);
			m_goto.x = _x;
			m_goto.y = _y;

			if (_rec.intersects(sf::FloatRect(m_hit_box.left - 3, m_hit_box.top, m_hit_box.width + 6, m_hit_box.height))) {

				return m_damage;
			}
		}
		else if (m_effect != EntityEffect::Wounded && m_effect != EntityEffect::Walking) {
			setEffect(EntityEffect::Normal);
		}

		return 0.f;
	}
	

	void Snake::upPosition(float time, Direction _dir) noexcept {

		if (_dir == Direction::Vertical) {

			if (is_gravity && m_status == EntityStat::InAir) {

				speed_y += 0.0005f * time;
				moveIt(0, speed_y * time);
			}
			if (!is_gravity || m_status == EntityStat::onStairs) {

				moveIt(0, speed_y * time);
			}
		}
		else if (_dir == Direction::Horixontal) {

			if (m_effect == EntityEffect::Walking) {
				_goto_(m_goto.x, m_goto.y, time);

				if (m_goto.x > m_hit_box.left && m_goto.x < m_hit_box.left + m_hit_box.width) {
					setEffect(EntityEffect::Normal);

					//m_goto.x = std::rand() % 80 + m_hit_box.left - 40;
				}
			}
			else if (m_effect == EntityEffect::Attack) {

				_goto_(m_goto.x, m_goto.y, time);

			}
			else if (m_effect == EntityEffect::Normal) {
				_goto_(m_hit_box.left, m_hit_box.top, time);

			}

			moveIt(speed_x * time, 0);

			if (is_gravity && speed_x != 0 && m_status != EntityStat::onStairs) {

				m_status = EntityStat::InAir;
			}
		}

	}


	void Snake::upEffect(float _time) noexcept {


		if (m_effect == EntityEffect::Wounded) {

			m_time_effect += _time;

			if (m_time_effect > DURATION_WOUNDED) {

				setEffect(EntityEffect::Normal);
				m_time_effect = _time = 0.f;
			}

			if (m_effect == EntityEffect::Wounded) {

				if (m_direction == Direction::Left) {

					speed_x = 0.1f - m_time_effect / DURATION_WOUNDED / 10.f;
				}
				else {
					speed_x = -0.1 + m_time_effect / DURATION_WOUNDED / 10.f;
				}
			}

		}
		else if (m_effect == EntityEffect::Normal) {

			m_time_effect += _time;

			if (m_time_effect > 1000.f) {

				m_goto.x = std::rand() % 80 + m_hit_box.left - 40;
				m_goto.y = m_hit_box.top;

				setEffect(EntityEffect::Walking);
				m_time_effect = _time = 0.f;
			}

		}
		else {
			m_time_effect = 0.f;
		}
		//printf("%g %d\n", m_time_effect, (int)m_effect);
	}


	void Snake::otherUpdate(float _time) {

		if (speed_x == 0.f && speed_y == 0.f && m_animation.getActive() != EntityAnimation::Idle) {
			m_animation.activate(EntityAnimation::Idle);
		}
		if (speed_x > 0) {
			m_direction = Direction::Right;
		}
		else if (speed_x < 0) {
			m_direction = Direction::Left;
		}

		//speed_x = 0;

		upEffect(_time);

		m_animation.update(_time);

		if (m_hp <= 0) {
			m_alive = false;
		}

	}


	void Snake::getHit(float _damage) noexcept {
		if (m_effect != EntityEffect::Wounded && _damage > 0.f) {
			m_hp -= _damage;
			speed_y = -0.1f;
			m_effect = EntityEffect::Wounded;
		}
	}


	void Snake::setEffect(EntityEffect _new_effect) {


		if (_new_effect != m_effect) {
			m_time_effect = 0.f;
			m_effect = _new_effect;
		}

	}


	void Snake::setStat(EntityStat _new) {

		//if (_new == EntityStat::InAir) {

		//	if (m_animation.getActive() != EntityAnimation::Jump) {

		//		m_animation.activate(EntityAnimation::Jump);
		//	}
		//}
		if (_new == EntityStat::onSolid || _new == EntityStat::onSolidAbove) {

			if (m_animation.getActive() != EntityAnimation::Walk) {

				m_animation.activate(EntityAnimation::Walk);
			}
		}
		//else if (_new == EntityStat::onStairs) {

		//	if (m_animation.getActive() != EntityAnimation::Walk) {

		//		m_animation.activate(EntityAnimation::Walk);
		//	}
		//}

		m_status = _new;
	}


	void Snake::_goto_(float _x, float _y, float _time) {

		if (_x > m_hit_box.left) {
			speed_x += m_accelerationx * _time;

			if (speed_x > MAX_SPEED_SNAKE) {
				speed_x = MAX_SPEED_SNAKE;
			}
		}
		else if (_x < m_hit_box.left) {
			speed_x -= m_accelerationx * _time;

			if (speed_x < -MAX_SPEED_SNAKE) {
				speed_x = - MAX_SPEED_SNAKE;
			}
		}
		else {
			if (speed_x > 0) {
				speed_x -= m_accelerationx * _time;
				speed_x = (speed_x >= 0) ? speed_x : 0;
			}
			else if (speed_x < 0) {
				speed_x += m_accelerationx * _time;
				speed_x = (speed_x <= 0) ? speed_x : 0;
			}
		}


		if (_y < m_hit_box.top) {
			jump();
		}


	}


	void Snake::_setAnimations_() {

		m_animation.addAnimation(EntityAnimation::Idle);
		m_animation.addFrame(EntityAnimation::Idle, sf::IntRect(0, 40, 8, 8), 200.f);
		m_animation.addFrame(EntityAnimation::Idle, sf::IntRect(8, 40, 8, 8), 200.f);
		m_animation.addFrame(EntityAnimation::Idle, sf::IntRect(16, 40, 8, 8), 200.f);

		m_animation.addAnimation(EntityAnimation::Walk);
		m_animation.addFrame(EntityAnimation::Walk, sf::IntRect(32, 40, 8, 8), 160.f);
		m_animation.addFrame(EntityAnimation::Walk, sf::IntRect(40, 40, 8, 8), 160.f);
		m_animation.addFrame(EntityAnimation::Walk, sf::IntRect(48, 40, 8, 8), 160.f);

		m_animation.addAnimation(EntityAnimation::Attack);
		m_animation.addFrame(EntityAnimation::Attack, sf::IntRect(88, 40, 8, 8), 100.f);
		m_animation.addFrame(EntityAnimation::Attack, sf::IntRect(96, 40, 8, 8), 100.f);
		m_animation.addFrame(EntityAnimation::Attack, sf::IntRect(104, 40, 8, 8), 100.f);

	}


	///////////////////////////////////////Needle///////////////////////////////////////////


	Needle::Needle(float place_x, float place_y, float width, float height)
		: Entity(TipeEntity::Needle, place_x, place_y, width, height)
		, m_damage(50)
	{
		is_gravity = false;
	}

} // namespace ezg