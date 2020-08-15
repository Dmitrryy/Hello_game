#include "World.h"

World::World(sf::RenderWindow& window)
	: m_window(window)
	, m_worldView(window.getDefaultView())
	, m_worldBounds(
		0.f, // left X position
		0.f, // top Y position
		m_worldView.getSize().x, // width
		2000.f) // height
	, m_spawnPosition(
		m_worldView.getSize().x / 2.f, // X
		m_worldBounds.height - m_worldView.getSize().y)
	, m_playerAircraft(nullptr) // Y
{
	loadTextures();
	buildScene();

	m_worldView.setCenter(m_spawnPosition);
}

void World::loadTextures()
{
	m_textures.load(Resource::Textures::ID::Eagle, "Resource/Textures/Eagle.png");
	m_textures.load(Resource::Textures::ID::Raptor, "Resource/Textures/Raptor.png");
	m_textures.load(Resource::Textures::ID::Desert, "Resource/Textures/Desert.png");
}

void World::buildScene() {

	for (size_t i = 0; i < static_cast<int>(Layer::LayerCount); i++) 
	{
		auto layer = std::make_unique<SceneNode>();
		m_sceneLayers[i] = layer.get();

		m_sceneGraph.attachChild(std::move(layer));
	}

	sf::Texture& texture = m_textures.get(Resource::Textures::ID::Desert);
	sf::IntRect textureRect(m_worldBounds);
	texture.setRepeated(true);
	
	{
		auto backgroundSprite = std::make_unique<SpriteNode>(texture, textureRect);
		backgroundSprite->setPosition(m_worldBounds.left, m_worldBounds.top);
		m_sceneLayers[(int)Layer::Background]->attachChild(std::move(backgroundSprite));
	}

	{
		auto leader = std::make_unique<Aircraft>(Aircraft::Eagle, m_textures);
		m_playerAircraft = leader.get();
		m_playerAircraft->setPosition(m_spawnPosition);
		m_playerAircraft->setVelocity(40.f, m_scrollSpeed);
		m_sceneLayers[(int)Layer::Air]->attachChild(std::move(leader));
	}

	{
		auto LEscort = std::make_unique<Aircraft>(Aircraft::Raptor, m_textures);
		LEscort->setPosition(-80.f, 50.f);
		m_playerAircraft->attachChild(std::move(LEscort));
	}
	{
		auto REscort = std::make_unique<Aircraft>(Aircraft::Raptor, m_textures);
		REscort->setPosition(80.f, 50.f);
		m_playerAircraft->attachChild(std::move(REscort));
	}
}

void World::draw()
{
	m_window.setView(m_worldView);
	m_window.draw(m_sceneGraph);
}

void World::update(sf::Time dt)
{
	m_worldView.move(0.f, m_scrollSpeed * dt.asSeconds());
	sf::Vector2f position = m_playerAircraft->getPosition();
	sf::Vector2f velocity = m_playerAircraft->getVelocity();
	if (position.x <= m_worldBounds.left + 150 || position.x >= m_worldBounds.left + m_worldBounds.width - 150)
	{
		velocity.x = -velocity.x;
		m_playerAircraft->setVelocity(velocity);
	}
	m_sceneGraph.update(dt);
}