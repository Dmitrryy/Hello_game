#pragma once

#include <SFML/Graphics.hpp>

#include "World.h"
#include "Player.h"

namespace ezg {

	class Game : private sf::NonCopyable
	{
	public:
		Game();
		void run();


	private:
		void processInput();

		void update(sf::Time elapsedTime);
		void render();

		void updateStatistics(sf::Time elapsedTime);


	private:
		static const sf::Time	TimePerFrame;

		sf::RenderWindow		m_window;
		World					m_world;
		Player                  m_player;

		sf::Font				m_font;
		sf::Text				m_statisticsText;
		sf::Time				m_statisticsUpdateTime;
		std::size_t				m_statisticsNumFrames;

		bool m_isPaused;
	};
}