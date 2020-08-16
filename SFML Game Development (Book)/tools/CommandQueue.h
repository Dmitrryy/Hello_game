#pragma once

#include <queue>

#include "Command.h"

namespace ezg {

	class CommandQueue
	{
	public:
		void push(const Command& command) { m_queue.push(command); }
		Command pop() {
			Command result = m_queue.front();
			m_queue.pop();
			return result;
		}
		bool isEmpty() const { return m_queue.empty(); }

	private:
		std::queue<Command> m_queue;
	};
}