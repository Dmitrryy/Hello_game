#pragma once

#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>

namespace menu {


	struct Button
	{

		//default constructor
		Button() noexcept;

		//////////////////////////////////////////////////////////////////////////////
		//constructor
		//_text - text indoor button
		//_font - font...
		//_pos_x and _pos_y - button center position, but not in coordinates (pixels):
		//	
		// 0.0f------------------------------------------------------------1.0f→x
		//  :   _____________________________________________________________ 
		//  :   |                                                           |
		//  :   |                                                           |
		//  :   |                                                           |
		//  :   |                                                           |
		//  :   |                          Window                           |
		//  :   |                                                           |
		//  :   |                                                           |
		//  :   |                                                           |
		// 1.0f _____________________________________________________________
		//  ↓ y
		Button(const std::string& _text, const sf::Font& _font, float _pos_x = 0.f, float _pos_y = 0.f);
		//////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////
		//draw a button in _target, with view _view
		void draw(sf::RenderTarget& _target);
		//////////////////////////////////////////////////////////////////////////////


		////////////////////////////////settings//////////////////////////////////////
		//set text color
		void setTextColor	(sf::Color _color)		{ m_text.setFillColor(_color);		}
		//set text size
		void setTextSize	(unsigned int _size)	{ m_text.setCharacterSize(_size);	}
		//set position. Coordinates are NOT IN PIXELS (see image above).
		void setPosition(float _pos_x, float _pos_y) noexcept { m_pos_x = _pos_x; /**/ m_pos_y = _pos_y; }
		//////////////////////////////////////////////////////////////////////////////


		sf::FloatRect getRect() { return m_text.getGlobalBounds(); }

	private:


		sf::Text m_text;

		float m_pos_x;
		float m_pos_y;

	}; // struct Button



	class Menu
	{

	public:

		//high probability of mistake. 
		//sf::Texture does not tolerate copying or moving. 
		//as a result a white screen is drawn instead of images.
		Menu            (const Menu& _that) = delete;   //not saported
		Menu& operator= (const Menu&)       = delete;   //not saported
		Menu            (Menu&& _that)      = delete;   //not saported
		Menu& operator= (Menu&&)            = delete;   //not saported
		

	public:

		//default constructor
		Menu() noexcept;


		//////////////////////////////////////////////////////////////////////////////
		//draw a menu in _target, with view _view
		void draw(sf::RenderTarget& _target);
		//////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////
		//adds a button.
		//	_id		- button identifier (key)
		//  _text	- display text
		//  _pos_x and _pos_y	- button position (see image above)
		void addButton(int _id, const std::string& _text, float _pos_x, float _pos_y);
		//////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////
		//access button number _id_button
		Button& atButton(int _id_button) { return m_buttons.at(_id_button); }
		//////////////////////////////////////////////////////////////////////////////


		/////////////////////////////////settings/////////////////////////////////////
		//set text color (default = RGB(255, 255, 255))
		void setTextColor   (sf::Color _color)  noexcept{ m_def_color	= _color;                  }
		//set text size  (default = 25)
		void setTextSize    (int _size)         noexcept{ m_def_size	= _size;                   }
		//set font
		bool setFont        (std::string _fname)        { return m_font.loadFromFile(_fname);      }
		void setFont        (sf::Font _font)            { m_font = _font; }
		//set BackGround for ALL window
		bool setBackGround  (std::string _fname)        { return m_background.loadFromFile(_fname);}
		///////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////
		//removes all buttons, 
		//sets the default colorand size of the text, 
		//removes the background.
		void clear () noexcept { m_buttons.clear(); }
		//////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////
		//menu interaction
		//  true - the button was pressed.
		//  _id_button - the number of the button pressed.
		bool checkPressing (const sf::Event& _event, const sf::RenderWindow& _target, int& _id_button);
		//////////////////////////////////////////////////////////////////////////////


	private:


		sf::Font				m_font;
		sf::Texture				m_background;
		std::map<int, Button>	m_buttons;

		int						m_highlighted_button;
		int						m_pressed_button;

		int						m_def_size;
		sf::Color				m_def_color;

	}; // class Menu



	class MenuManager
	{
	public:

		//default constructor
		MenuManager() noexcept;

		//////////////////////////////////////////////////////////////////////////////
		//create menu with number _id_menu
		void create(int _id_menu);
		//rem0ve menu with number _id_menu
		void remove(int _id_menu);
		//////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////
		//access menu number _id_button
		Menu& atMenu(int _id_menu) { return m_menus.at(_id_menu); }
		//////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////
		//draw active menu in _target, with view _view
		void drawActive(sf::RenderTarget& _target);
		//////////////////////////////////////////////////////////////////////////////


		bool setFont(const std::string& _fname) { return m_font.loadFromFile(_fname); }


		//////////////////////////////////////////////////////////////////////////////
		//activate less with number _id_menu
		bool activate(int _id_menu);
		//////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////
		//menu interaction
		bool checkPressing(const sf::Event& _event, const sf::RenderWindow& _target,  int& _id_button);
		//////////////////////////////////////////////////////////////////////////////

	private:

		bool					is_active;
		int						m_active;

		std::map <int, Menu>	m_menus;
		
		sf::Font				m_font;
	};

} // namespace menu