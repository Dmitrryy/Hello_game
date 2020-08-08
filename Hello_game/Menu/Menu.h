#pragma once

#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>

namespace menu {

	class Item 
	{
	public:

		enum class Type {
			  Button
			, Image
			, Text
			, Unknowm
		};

	protected:

		//service
		Item(Item::Type _type, sf::Vector2f _pos, sf::Vector2f _size);


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


	public:
		//////////////////////////////////////////////////////////////////////////////
		//befoult constructor
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
		//////////////////////////////////////////////////////////////////////////////
		//draw a button in _target
		void draw(sf::RenderTarget& _target) override;
		//////////////////////////////////////////////////////////////////////////////


		/////////////////////////////text settings////////////////////////////////////
		void setDefText          (const sf::Text& _new_text) { m_defTxt = _new_text; }
		void setHighlightingText (const sf::Text& _new_text) { m_highlightingTxt = _new_text; }
		void setPressedText      (const sf::Text& _new_text) { m_pressedTxt = _new_text; }
		//[0, 1]
		void setPositionTxt      (sf::Vector2f _pos) noexcept { m_pos_Txt = _pos; }
		//
		void setString(const std::string& _str);
		//
		void setFont(const sf::Font& _font);
		//////////////////////////////////////////////////////////////////////////////


		////////////////////////////texture settings//////////////////////////////////
		void texture    (bool _is) { activeTx = _is; }
		//
		//bool setTexture (const std::string& _new_tx) { return m_Tx.loadFromFile(_new_tx); }
		//dangerous function! possible white rectangle
		void setTexture(const sf::Texture& _new_tx);
		//
		void setDefTextureRect         (sf::IntRect _rec) { m_def_BG.setTextureRect(_rec); }
		void setHighlTextureRect(sf::IntRect _rec) { m_highlighting_BG.setTextureRect(_rec); }
		void setPressedTextureRect     (sf::IntRect _rec) { m_pressed_BG.setTextureRect(_rec); }
		//////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////
		void setStat(Button::Stat _new_stat) noexcept { m_stat = _new_stat; }
		//
		Button::Stat getStat() noexcept { return m_stat; }
		//////////////////////////////////////////////////////////////////////////////


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
		void addButton(int _id, const Button& _button);
		//
		void addOtherItem(std::unique_ptr<Item>&& _item) { m_other_items.push_back(_item.release()); }
		void addImage(std::unique_ptr<Image>&& _im) { _im->setTexture(m_texture); m_other_items.push_back(_im.release()); }
		//////////////////////////////////////////////////////////////////////////////
		Item& atItem(int _id) {
			
			if (_id < m_other_items.size() && _id >= 0) {
				return *(m_other_items.at(_id));
			}
			else { throw std::invalid_argument("invalid Item id: " + std::to_string(_id)); }
		}


		//////////////////////////////////////////////////////////////////////////////
		const sf::Font& getFont() { return m_font; }
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
		bool setTexture  (std::string _fname)        { return m_texture.loadFromFile(_fname);}
		void setTexture  (const sf::Texture& _tx)    { m_texture = _tx;}
		const sf::Texture& getTexture() { return m_texture; }
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
		sf::Texture				m_texture;
		std::map<int, Item*>	m_buttons;
		std::vector<Item*>      m_other_items;

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