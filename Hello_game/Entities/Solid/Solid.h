#pragma once

#include "../../config.h"
#include "../Entity.h"


namespace ezg {


	class Landscape : public ezg::Entity
	{
	public:

		enum class Type {
			  Solid
			, SolidAbove
			, Stairs
		};

	protected:
		Landscape(Type _type, float place_x, float place_y, float width, float height)
			: Entity(Entity::Type::Landscape, place_x, place_y, width, height)
			, m_type(_type)
		{}

	public:

		Type getType() { return m_type; }

	private:

		Type m_type;
	};

	///////////////////////////////////////////////
	//
	//regular rectangl that you cannot go through
	//
	///////////////////////////////////////////////
	class Solid final : public Landscape
	{
	public:

		Solid(float place_x, float place_y, float width, float height) noexcept
			: Landscape(Landscape::Type::Solid, place_x, place_y, width, height)
		{
			is_gravity = false;
		}

		void draw (sf::RenderTarget&, sf::RenderStates) const noexcept override { /* nop */	}

		std::unique_ptr<Entity> colision(Entity*, Direction) noexcept override { return nullptr; }

		void otherUpdate (float) noexcept override { /* nop */ }

	}; // class Solid



	////////////////////////////////////////////////////////////////////////////////////////////



	class SolidAbove final : public Landscape
	{

	public:

		SolidAbove(float place_x, float place_y, float width, float height) noexcept
			: Landscape(Landscape::Type::SolidAbove, place_x, place_y, width, height)
		{
			is_gravity = false;
		}

		void draw (sf::RenderTarget&, sf::RenderStates) const noexcept override { /* nop */ }

		std::unique_ptr<Entity> colision(Entity*, Direction) noexcept override { return nullptr; }

		void otherUpdate (float) noexcept override { /* nop */ }
	};



	////////////////////////////////////////////////////////////////////////////////////////////



	class Stairs final : public Landscape
	{
	public:

		Stairs(float place_x, float place_y, float width, float height) noexcept
			: Landscape(Landscape::Type::Stairs, place_x, place_y, width, height)
		{
			is_gravity = false;
		}

		void draw (sf::RenderTarget&, sf::RenderStates) const noexcept override { /* nop */ }

		std::unique_ptr<Entity> colision(Entity*, Direction) noexcept override { return nullptr; }

		void otherUpdate (float) noexcept override { /* nop */ }
	};

} // namespace ezg