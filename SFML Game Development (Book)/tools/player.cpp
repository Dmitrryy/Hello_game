#include <iostream>

#include "Player.h"
#include "../Entities/Aircraft/Aircraft.h"

namespace ezg {

	struct AircraftMover
	{
		AircraftMover(float vx, float vy)
			: velocity(vx, vy)
		{
		}
		void operator() (Aircraft& node, sf::Time) const
		{
			node.accelerate(velocity);
		}
		sf::Vector2f velocity;
	};

	Player::Player()
	{
		m_keyBinding[sf::Keyboard::Left] = Action::MoveLeft;
		m_keyBinding[sf::Keyboard::Right] = Action::MoveRight;
		m_keyBinding[sf::Keyboard::Up] = Action::MoveUp;
		m_keyBinding[sf::Keyboard::Down] = Action::MoveDown;

		constexpr float playerSpeed = 80.f;

		m_actionBind[Action::MoveLeft].action =
			[playerSpeed](SceneNode& node, sf::Time dt) {
			node.move(-playerSpeed * dt.asSeconds(), 0.f);
		};

		m_actionBind[Action::MoveRight].action =
			[playerSpeed](SceneNode& node, sf::Time dt) {
			node.move(playerSpeed * dt.asSeconds(), 0.f);
		};

		m_actionBind[Action::MoveUp].action =
			[playerSpeed](SceneNode& node, sf::Time dt) {
			node.move(0.f, -playerSpeed * dt.asSeconds());
		};

		m_actionBind[Action::MoveDown].action =
			[playerSpeed](SceneNode& node, sf::Time dt) {
			node.move(0.f, playerSpeed * dt.asSeconds());
		};

		for (auto& cmd : m_actionBind) {
			cmd.second.category = Category::Type::PlayerAircraft;
		}
	}

	void Player::handleRealtimeInput(CommandQueue& commands) {

		for (const auto& p : m_keyBinding) {
			if (sf::Keyboard::isKeyPressed(p.first) && isRealtimeAction(p.second)) {
				commands.push(m_actionBind[p.second]);
			}
		}
	}

	bool Player::isRealtimeAction(Action _action) {

		bool result = false;
		switch (_action)
		{
		case Action::MoveDown:
		case Action::MoveLeft:
		case Action::MoveRight:
		case Action::MoveUp:
			result = true;
			break;
		}

		return result;
	}

	void Player::handleEvent(const sf::Event& event, CommandQueue& commands) {
		if (event.type == sf::Event::KeyPressed	&& event.key.code == sf::Keyboard::P)
		{
			Command output;
			output.category = Category::Type::PlayerAircraft;
			output.action = [](SceneNode& s, sf::Time)
			{
				std::cout << s.getPosition().x << ","
					<< s.getPosition().y << "\n";
			};
			commands.push(output);
		}
	}

	void Player::assignKey(Action _action, sf::Keyboard::Key _key) {
		m_keyBinding.erase(getAssignedKey(_action));
		m_keyBinding.erase(_key);
		m_keyBinding.insert(std::make_pair(_key, _action));
	}

	sf::Keyboard::Key Player::getAssignedKey(Action _action) 
	{
		sf::Keyboard::Key result = sf::Keyboard::Key::Unknown;
		for (const auto& b : m_keyBinding) {
			if (b.second == _action) {
				result = b.first;
				break;
			}
		}
		return result;
	}
}