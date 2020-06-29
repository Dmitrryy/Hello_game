#pragma once

#include "config.h"
//#include "NodeGame.h"

namespace ezg {


	struct Button
	{
		Button()
		{

		}

		Button(MenuButton _tipe, const std::string& _text, const sf::Font& _font, sf::Vector2f _pos)
			: m_tipe(_tipe)
			, m_text()
		{
			m_text.setFont(_font);
			m_text.setPosition(_pos);
			m_text.setString(_text);
		}

		MenuButton m_tipe;

		sf::Text m_text;

	}; // struct Button



	class Menu
	{
		

	public:


		Menu(gsl::not_null<NodeGame*> _ngame)
			: m_nodegame(_ngame)
			, m_def_size(25)
			, m_def_color(255, 255, 255)
		{

		}

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;


		////////////////////////////////////////
		//adds a button.
		//	_id		- button identifier (key)
		//	_tipe	- button type (determines the behavior when pressed)
		//  _text	- display text
		//  _pos	- button position
		void addButton(int _id, MenuButton _tipe, const std::string& _text, sf::Vector2f _pos);
		////////////////////////////////////////


		////////////////////////////////////////
		//sets the desired menu type
		//
		// now supported: Main Menu; Pause
		void setMenu		(TipeMenu _tipe);
		////////////////////////////////////////


		//////////////////settings//////////////////////
		//set text color
		void setTextColor	(sf::Color _color)	noexcept { m_def_color	= _color;	}
		//set text size
		void setTextSize	(int _size)			noexcept { m_def_size	= _size;	}
		//set font
		bool setFont		(std::string _fname) { return m_font.loadFromFile(_fname);			}
		//set BackGround for ALL window
		bool setBackGround	(std::string _fname) { return m_background.loadFromFile(_fname);	}
		////////////////////////////////////////////////


		void clear			() noexcept { m_buttons.clear(); }


		////////////////////////////////////////
		//menu interaction
		void checkEvent		(const sf::Event& _event);
		void checkKeyBoard	();
		////////////////////////////////////////


	private:

		void _pressing_		(int _id);

	private:

		gsl::not_null<NodeGame*> m_nodegame;


		sf::Font				m_font;
		sf::Texture				m_background;
		std::map<int, Button>	m_buttons;

		int						m_def_size;
		sf::Color				m_def_color;

	}; // class Menu


} // namespace ezg