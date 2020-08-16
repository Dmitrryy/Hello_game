#pragma once
#include <map>

#include "CommandQueue.h"

namespace ezg {

	class Player
	{
	public:

		Player();

	public:

		enum class Action {
			MoveLeft
			, MoveRight
			, MoveUp
			, MoveDown
		};

		void assignKey(Action _action, sf::Keyboard::Key _key);
		sf::Keyboard::Key getAssignedKey(Action _action);

	private:

		static bool isRealtimeAction(Action _action);

	public:

		void handleEvent(const sf::Event& event, CommandQueue& commands);
		void handleRealtimeInput(CommandQueue& commands);

	private:

		std::map<sf::Keyboard::Key, Action> m_keyBinding;
		std::map<Action, Command>           m_actionBind;
	};
}