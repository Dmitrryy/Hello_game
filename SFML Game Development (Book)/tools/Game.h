#pragma once

#include <SFML/Graphics.hpp>

#include "World.h"

class Game : private sf::NonCopyable
{
public:
	Game();
	void run();


private:
	void processEvents();
	void update(sf::Time elapsedTime);
	void render();

	void updateStatistics(sf::Time elapsedTime);
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);


private:
	static const sf::Time	TimePerFrame;

	sf::RenderWindow		m_window;
	World					m_world;

	sf::Font				m_font;
	sf::Text				m_statisticsText;
	sf::Time				m_statisticsUpdateTime;
	std::size_t				m_statisticsNumFrames;
};