﻿#pragma once

#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>
#include <functional>

namespace menu {

	class Item 
	{
	public:

		enum class Type {
			  Button
			, Image
			, Text
			, DataInt
			, Unknowm
		};

	protected:

		//service
		Item(Item::Type _type, sf::Vector2f _pos = sf::Vector2f(0.0, 0.0), sf::Vector2f _size = sf::Vector2f(0.0, 0.0));


	public:

		virtual ~Item() = default;

		//////////////////////////////////////////////////////////////////////////////
		//draw a item in _target, with view _view
		virtual void draw(sf::RenderTarget& _target) = 0;
		//////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////set/////////////////////////////////////////
		void setPosition (sf::Vector2f _pos)  noexcept { m_position = _pos; }
		void setSize     (sf::Vector2f _size) noexcept { m_size = _size; }
		//////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////get/////////////////////////////////////////
		sf::Vector2f  getPosition() const noexcept { return m_position; }
		sf::Vector2f  getSize    () const noexcept { return m_size;     }
		sf::FloatRect getRect    () const noexcept { return m_rec; }
		//
		Item::Type    getType    () const noexcept { return m_type;     }
		//////////////////////////////////////////////////////////////////////////////


	protected:

		Item::Type     m_type;

		sf::Vector2f   m_position;
		sf::Vector2f   m_size;

		sf::FloatRect  m_rec;

	}; //class Item


	class DataInt : public Item
	{
	public:

		DataInt(int _new = 0.f)
			: Item(Item::Type::DataInt)
			, m_data(_new)
		{}


		void draw(sf::RenderTarget& _target) override { /*nop*/ }

		void set(int _new) { m_data = _new; }
		int  get() { return m_data; }

	private:
		int m_data;
	};



	class Image : public Item
	{
	public:

		//default constructor
		Image(sf::Vector2f _position = sf::Vector2f(0.0, 0.0), sf::Vector2f _size = sf::Vector2f(0.0, 0.0)) noexcept
			: Item(Item::Type::Image, _position, _size)
		{

		}

	public:

		//////////////////////////////////////////////////////////////////////////////
		//draw a Image in _target
		void draw(sf::RenderTarget& _target) override;
		//////////////////////////////////////////////////////////////////////////////


		void setTextureRect(sf::IntRect _rec) { m_sprite.setTextureRect(_rec); }
		void setTexture(const sf::Texture& _tx) { m_sprite.setTexture(_tx); }


	private:

		sf::Sprite m_sprite;

	};//class Image



	class Text : public Item
	{
	public:

		//default constructor
		Text(const std::string& _str = std::string(), sf::Vector2f _position = sf::Vector2f(0.f, 0.f))
			: Item(Item::Type::Text, _position, sf::Vector2f(0.f, 0.f))
		{
			m_txt.setString(_str);
		}

		//////////////////////////////////////////////////////////////////////////////
		//draw a Text in _target
		void draw (sf::RenderTarget& _target) override;
		//////////////////////////////////////////////////////////////////////////////


		void setText (const std::string& _str) { m_txt.setString(_str); }
		void setText (const sf::Text& _txt)    { m_txt = _txt; }


	private:

		sf::Text m_txt;

	};//class Text



	class Button : public Item
	{
	public:
		enum class Stat {
			  Default
			, HighLight
			, Pressed
		};
		enum class Script {
			HighLight
			, UnHightlight
			, Pressed
			, Released
		};


	public:
		//////////////////////////////////////////////////////////////////////////////
		//defoult constructor
		//_position - button center position, but not in coordinates (pixels): [0.f, 1.f]
		//_size - what part of the screen it occupies. also from 0 to 1 (see the picture)
		//
		// 0.0f------------------------------------------------------------1.0f→x
		//  :   _____________________________________________________________ 
		//  :   |                                                           |
		//  :   |                                                           |
		//  :   |                          Window                           |
		//  :   |                                                           |
		// 1.0f _____________________________________________________________
		//  ↓ y
		Button(sf::Vector2f _position = sf::Vector2f(0.0, 0.0), sf::Vector2f _size = sf::Vector2f(0.0, 0.0)) noexcept;
		//////////////////////////////////////////////////////////////////////////////


	public:

		void hightlight() {
			if (m_stat != Button::Stat::Pressed && m_stat != Button::Stat::HighLight) {
				setStat(Button::Stat::HighLight);
				m_hl_script();
			}
		}
		void unHightlight() {
			if (m_stat != Button::Stat::Default) {
				setStat(Button::Stat::Default);
				m_unhl_script();
			}
		}
		void press() {
			setStat(Button::Stat::Pressed);
			m_press_script();
		}
		void release() {
			setStat(Button::Stat::Default);
			m_repress_script();
		}
		//////////////////////////////////////////////////////////////////////////////
		//draw a button in _target
		void draw(sf::RenderTarget& _target) override;
		//////////////////////////////////////////////////////////////////////////////


		/////////////////////////////text settings////////////////////////////////////
		void setText(const sf::Text& _txt, Button::Stat stat = Button::Stat::Default);
		//[0, 1]
		void setPositionTxt(sf::Vector2f _pos) noexcept { m_pos_Txt = _pos; }
		//
		void setString(const std::string& _str, Button::Stat stat = Button::Stat::Default);
		//
		void setFont(const sf::Font& _font);
		//
		void setScript(Button::Script _type, std::function<void() noexcept> _script);
		//////////////////////////////////////////////////////////////////////////////


		////////////////////////////texture settings//////////////////////////////////
		//bool setTexture (const std::string& _new_tx) { return m_Tx.loadFromFile(_new_tx); }
		//dangerous function! possible white rectangle
		void setTexture(const sf::Texture& _new_tx);
		//
		void setTextureRect(sf::IntRect _rec, Button::Stat _stat = Button::Stat::Default);
		//////////////////////////////////////////////////////////////////////////////

		Button::Stat getStat() noexcept { return m_stat; }

	private:

		void setStat(Button::Stat _new_stat) noexcept { m_stat = _new_stat; }


	private:

		Button::Stat m_stat;

		sf::Text     m_defTxt;
		sf::Text     m_highlightingTxt;
		sf::Text     m_pressedTxt;
		sf::Vector2f m_pos_Txt;

		bool activeTx;
		sf::Sprite   m_def_BG;
		sf::Sprite   m_highlighting_BG;
		sf::Sprite   m_pressed_BG;

		std::function<void() noexcept> m_hl_script;
		std::function<void() noexcept> m_unhl_script;
		std::function<void() noexcept> m_press_script;
		std::function<void() noexcept> m_repress_script;
	};




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
		//  _button - already configured button
		void addButton(const Button& _button);
		void addButton(std::unique_ptr<Button>&& _bt);
		//
		void addOtherItem(std::unique_ptr<Item>&& _item) { m_other_items.push_back(_item.release()); }
		void addImage(std::unique_ptr<Image>&& _im) { _im->setTexture(m_texture); m_other_items.push_back(_im.release()); }
		//////////////////////////////////////////////////////////////////////////////
		Item* atItem(int _id) {
			
			if (_id < m_other_items.size() && _id >= 0) {
				return m_other_items.at(_id);
			}
			else { throw std::invalid_argument("invalid Item id: " + std::to_string(_id)); }
		}


		//////////////////////////////////////////////////////////////////////////////
		const sf::Font& getFont() noexcept { return m_font; }
		//////////////////////////////////////////////////////////////////////////////


		/////////////////////////////////settings/////////////////////////////////////
		//set text color (default = RGB(255, 255, 255))
		void setTextColor (sf::Color _color) noexcept { m_def_color	= _color;                  }
		//set text size  (default = 25)
		void setTextSize  (int _size)        noexcept { m_def_size	= _size;                   }
		//set font
		bool setFont      (std::string _fname)        { return m_font.loadFromFile(_fname);    }
		void setFont      (sf::Font _font)            { m_font = _font; }
		//set BackGround for ALL window
		bool setTexture   (std::string _fname)        { return m_texture.loadFromFile(_fname);}
		void setTexture   (const sf::Texture& _tx)    { m_texture = _tx;}
		const sf::Texture& getTexture() noexcept      { return m_texture; }
		///////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////
		//removes all buttons, 
		//sets the default colorand size of the text, 
		//removes the background.
		void clear () noexcept { m_buttons.clear(); }
		//////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////
		//menu interaction(deprecated)
		//  true - the button was pressed.
		//  _id_button - the number of the button pressed.
		//[[deprecated("use 'bool checkEvent(const sf::Event& _event, const sf::RenderWindow& _window)'")]]
		bool checkPressing (const sf::Event& _event, const sf::RenderWindow& _window, int& _id_button);
		//menu interaction.
		//  runs the corresponding script('Button::setScript(...)') when interacting with the button.
		bool checkEvent (const sf::Event& _event, const sf::RenderWindow& _window);
		//////////////////////////////////////////////////////////////////////////////


	private:


		sf::Font                m_font;
		sf::Texture             m_texture;
		std::vector<Button*>    m_buttons;
		size_t                  m_highlighted_button;
		std::vector<Item*>      m_other_items;

		int                     m_def_size;
		sf::Color               m_def_color;

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
		const sf::Font& getFont() noexcept { return m_font; }

		//////////////////////////////////////////////////////////////////////////////
		//activate less with number _id_menu
		bool activate(int _id_menu);
		//
		bool isActive(int& _id_menu);
		//////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////
		//menu interaction
		[[deprecated("use 'bool checkEvent(const sf::Event& _event, const sf::RenderWindow& _window)'")]]
		bool checkPressing(const sf::Event& _event, const sf::RenderWindow& _target,  int& _id_button);
		//
		bool checkEvent(const sf::Event& _event, const sf::RenderWindow& _window);
		//////////////////////////////////////////////////////////////////////////////

	private:

		bool                 is_active;
		int                  m_active;

		std::map <int, Menu> m_menus;
		
		sf::Font             m_font;
	};

} // namespace menu