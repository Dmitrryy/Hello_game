#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include "SceneNode/SceneNode.h"
#include "SpriteNode/SpriteNode.h"
#include "../Entities/Aircraft/Aircraft.h"
#include "CommandQueue.h"
#include "../config.h"

namespace ezg {

	class World
	{
	public:

		World(const World& _that) = delete; //not saported
		World& operator= (const World&) = delete; //not saported
		World(World&& _that) = delete; //not saported
		World& operator= (World&&) = delete; //not saported

	public:

		World(sf::RenderWindow& window);

	public:

		void update(sf::Time _time);
		void draw();

		CommandQueue& getCommands() { return m_commands; }

	private:

		void loadTextures();
		void buildScene();

	private:
		enum class Layer
		{
			Background,
			Air,
			LayerCount
		};

	private:

		sf::RenderWindow& m_window;
		sf::View                                       m_worldView;
		TextureHolder                                  m_textures;
		SceneNode                                      m_sceneGraph;
		std::array<SceneNode*, (int)Layer::LayerCount> m_sceneLayers;

		sf::FloatRect                                  m_worldBounds;
		sf::Vector2f                                   m_spawnPosition;
		float                                          m_scrollSpeed;
		Aircraft* m_playerAircraft;

		CommandQueue                                   m_commands;

	};
}