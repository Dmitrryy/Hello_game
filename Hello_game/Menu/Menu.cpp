#include "Menu.h"


namespace menu {
	////////////////////////////////////Item//////////////////////////////////////////

	Item::Item(Item::Type _type, sf::Vector2f _pos, sf::Vector2f _size)
		: m_type(_type)
		, m_position(_pos)
		, m_size(_size)
	{

	}

	////////////////////////////////////Image/////////////////////////////////////////


	void Image::draw(sf::RenderTarget& _target) /*override*/ {
		const sf::View _view = _target.getView();

		const sf::Vector2f _view_center = _view.getCenter();
		const sf::Vector2f _view_size = _view.getSize();

		const sf::Vector2f bt_pos(_view_center.x - _view_size.x * (0.5f - m_position.x),
			_view_center.y - _view_size.y * (0.5f - m_position.y));

		m_rec = sf::FloatRect(sf::Vector2f(bt_pos.x - m_size.x * _view_size.x / 2.f, bt_pos.y - m_size.y * _view_size.y / 2.f)
			, sf::Vector2f(m_size.x * _view_size.x, m_size.y * _view_size.y));

		m_sprite.setPosition(bt_pos);
		m_sprite.setScale(m_rec.width / m_sprite.getTextureRect().width, m_rec.height / m_sprite.getTextureRect().height);

		_target.draw(m_sprite);
	}


	////////////////////////////////////Text//////////////////////////////////////////

	void Text::draw(sf::RenderTarget& _target) /*override*/ {

		const sf::View _view = _target.getView();

		const sf::Vector2f _view_center = _view.getCenter();
		const sf::Vector2f _view_size = _view.getSize();

		const sf::Vector2f txt_pos(_view_center.x - _view_size.x * (0.5f - m_position.x),
			_view_center.y - _view_size.y * (0.5f - m_position.y));

		m_txt.setPosition(txt_pos);

		_target.draw(m_txt);
	}

	///////////////////////////////////Button/////////////////////////////////////////


	Button::Button(sf::Vector2f _position /*= sf::Vector2f(0.0, 0.0)*/, sf::Vector2f _size /*= sf::Vector2f(0.0, 0.0)*/) noexcept
		: Item(Item::Type::Button, _position, _size)
		, m_stat(Button::Stat::Default)
		, m_pos_Txt(0.0, 0.0)
		, activeTx(false)
		, m_hl_script([]() noexcept {})
		, m_unhl_script([]() noexcept {})
		, m_press_script([]() noexcept {})
		, m_repress_script([]() noexcept {})
	{

	}


	void Button::draw(sf::RenderTarget& _target) 
	{

		const sf::View _view = _target.getView();

		const sf::Vector2f _view_center = _view.getCenter();
		const sf::Vector2f _view_size = _view.getSize();

		const sf::Vector2f bt_pos(_view_center.x - _view_size.x * (0.5f - m_position.x),
			_view_center.y - _view_size.y * (0.5f - m_position.y));

		m_rec = sf::FloatRect(sf::Vector2f(bt_pos.x - m_size.x * _view_size.x / 2.f, bt_pos.y - m_size.y * _view_size.y / 2.f)
			                , sf::Vector2f(m_size.x * _view_size.x, m_size.y * _view_size.y));

		const sf::Vector2f txt_pos(m_rec.left + m_rec.width * m_pos_Txt.x, m_rec.top + m_rec.height * m_pos_Txt.y);

		sf::Sprite* tmp_sp  = &m_def_BG;
		sf::Text*   tmp_txt = &m_defTxt;
		switch (m_stat)
		{
		case Button::Stat::Default:
			//nop
			break;

		case Button::Stat::HighLight:
			tmp_sp  = &m_highlighting_BG;
			tmp_txt = &m_highlightingTxt;
			break;

		case Button::Stat::Pressed:
			tmp_sp  = &m_pressed_BG;
			tmp_txt = &m_pressedTxt;
			break;

		default:
			assert(0);
			break;
		}

		//sf::RectangleShape _b;
		//_b.setSize(sf::Vector2f(m_rec.width, m_rec.height));
		//_b.setPosition(sf::Vector2f(m_rec.left, m_rec.top));
		//_b.setFillColor(sf::Color::Red);
		//_target.draw(_b);

		//sf::RectangleShape _t;
		//_t.setSize(sf::Vector2f(tmp_txt->getGlobalBounds().width, tmp_txt->getGlobalBounds().height));
		//_t.setPosition(sf::Vector2f(tmp_txt->getGlobalBounds().left, tmp_txt->getGlobalBounds().top));
		//_t.setFillColor(sf::Color::Green);
		//_target.draw(_t);


		if (activeTx) {
			tmp_sp->setPosition(bt_pos);
			tmp_sp->setOrigin(tmp_sp->getTextureRect().width / 2.f, tmp_sp->getTextureRect().height / 2.f);
			tmp_sp->setScale(m_rec.width / tmp_sp->getTextureRect().width, m_rec.height / tmp_sp->getTextureRect().height);
			_target.draw(*tmp_sp);
		}

		tmp_txt->setPosition(txt_pos);
		//nothing unusual about that... just a fit... scroll on.
		tmp_txt->setOrigin (tmp_txt->getGlobalBounds().width / 2.f / tmp_txt->getScale().x + tmp_txt->getLocalBounds().left,
			tmp_txt->getGlobalBounds().height / 2.f / tmp_txt->getScale().y + tmp_txt->getLocalBounds().top);

		_target.draw(*tmp_txt);
	}


	void Button::setScript(Button::Script _type, std::function<void() noexcept> _script) {
		switch (_type)
		{
		case Button::Script::HighLight:
			m_hl_script = _script;
			break;

		case Button::Script::UnHightlight:
			m_unhl_script = _script;
			break;

		case Button::Script::Pressed:
			m_press_script = _script;
			break;

		case Button::Script::Released:
			m_repress_script = _script;
			break;
		}
	}


	void Button::setText(const sf::Text& _txt, Button::Stat _stat /*= Button::Stat::Default*/) {
		switch (_stat)
		{
		case Button::Stat::Default:
			m_defTxt = _txt;
			m_highlightingTxt = _txt;
			m_pressedTxt = _txt;
			break;

		case Button::Stat::HighLight:
			m_highlightingTxt = _txt;
			break;

		case Button::Stat::Pressed:
			m_pressedTxt = _txt;
			break;
		}
	}


	void Button::setString(const std::string& _str, Button::Stat stat /*= Button::Stat::Default*/) {
		switch (stat)
		{
		case Button::Stat::Default:
			m_defTxt.setString(_str);
			m_highlightingTxt.setString(_str);
			m_pressedTxt.setString(_str);
			break;

		case Button::Stat::HighLight:
			m_highlightingTxt.setString(_str);
			break;

		case Button::Stat::Pressed:
			m_pressedTxt.setString(_str);
			break;
		}
	}


	void Button::setFont(const sf::Font& _font) {
		m_defTxt.setFont(_font);
		m_highlightingTxt.setFont(_font);
		m_pressedTxt.setFont(_font);
	}


	void Button::setTexture(const sf::Texture& _new_tx) {
		m_def_BG.setTexture(_new_tx);
		m_highlighting_BG.setTexture(_new_tx);
		m_pressed_BG.setTexture(_new_tx);
	}
	void Button::setTextureRect(sf::IntRect _rec, Button::Stat stat /*= Button::Stat::Default*/) {
		activeTx = true;
		switch (stat)
		{
		case Button::Stat::Default:
			m_def_BG.setTextureRect(_rec);
			m_highlighting_BG.setTextureRect(_rec);
			m_pressed_BG.setTextureRect(_rec);
			break;

		case Button::Stat::HighLight:
			m_highlighting_BG.setTextureRect(_rec);
			break;

		case Button::Stat::Pressed:
			m_pressed_BG.setTextureRect(_rec);
			break;
		}
	}


	////////////////////////////////////Menu//////////////////////////////////////////

	Menu::Menu() noexcept
		: m_def_size(25)
		, m_def_color(sf::Color::White)
		, m_highlighted_button(-1)
	{

	}


	void Menu::draw(sf::RenderTarget& _target) {

		for (auto& item : m_other_items) {
			item->draw(_target);
		}

		for (auto& button : m_buttons) {
			button->draw(_target);
		}
	}


	void Menu::addButton(const Button& _button) {

		Button* res = new Button(_button);
		res->setFont(m_font);
		res->setTexture(m_texture);

		m_buttons.push_back(res);
	}

	void Menu::addButton(std::unique_ptr<Button>&& _bt) {

		_bt->setFont(m_font);
		_bt->setTexture(m_texture);

		m_buttons.push_back(_bt.release());
	}

	bool Menu::checkEvent(const sf::Event& _event, const sf::RenderWindow& _window) {
		int id;
		return checkPressing(_event, _window, id);
	}
	bool Menu::checkPressing(const sf::Event& _event, const sf::RenderWindow& _window, int& _id_button) {

		_id_button = 0;
		bool res = false;

		if (_event.type == sf::Event::MouseButtonReleased) {

			if (_event.mouseButton.button == sf::Mouse::Left) {
				const sf::Vector2f world_pos = _window.mapPixelToCoords(sf::Vector2i(_event.mouseButton.x, _event.mouseButton.y));

				for (auto button : m_buttons) {

					if (button->getRect().contains(world_pos)) {
						res = true;
						button->release();
						break;
					}
				}
			}

		}
		else if (_event.type == sf::Event::MouseButtonPressed) {

			if (_event.mouseButton.button == sf::Mouse::Left) {
				const sf::Vector2f world_pos = _window.mapPixelToCoords(sf::Vector2i(_event.mouseButton.x, _event.mouseButton.y));

				//bool stat = false;
				for (auto& button : m_buttons) {

					if (button->getRect().contains(world_pos)) {
						//button state when pressed
						button->press();
						break;
					}

				}
			}

		}
		else if (_event.type == sf::Event::MouseMoved) {

			const sf::Vector2f world_pos = _window.mapPixelToCoords(sf::Vector2i(_event.mouseMove.x, _event.mouseMove.y));

			bool stat = false;
			size_t i = 0;
			for (auto& button : m_buttons) {
				if (!stat && button->getRect().contains(world_pos)) {
					//hover button state
					button->hightlight();
					stat = true;
					m_highlighted_button = i;
				}
				else {
					button->unHightlight();
				}
				i++;
			}
			if (!stat) {
				m_highlighted_button = -1;
			}
			
		}
		else if (_event.type == sf::Event::KeyPressed) {

			if (_event.key.code == sf::Keyboard::Enter && m_highlighted_button != -1) {
				m_buttons.at(m_highlighted_button)->release();
			}
			else if (_event.key.code == sf::Keyboard::W || _event.key.code == sf::Keyboard::Up) {
				if (m_buttons.size() > 0) {
					if (m_highlighted_button == -1) {
						m_highlighted_button = 0;
						m_buttons.at(0)->hightlight();
					}
					else {
						m_buttons.at(m_highlighted_button)->unHightlight();
						m_highlighted_button++;
						m_highlighted_button = (m_highlighted_button == m_buttons.size()) ? 0 : m_highlighted_button;
						m_buttons.at(m_highlighted_button)->hightlight();
					}
				}
			}
			else if (_event.key.code == sf::Keyboard::S || _event.key.code == sf::Keyboard::Down) {
				if (m_buttons.size() > 0) {
					if (m_highlighted_button == -1) {
						m_highlighted_button = m_buttons.size() - 1;
						m_buttons.at(m_highlighted_button)->hightlight();
					}
					else {
						m_buttons.at(m_highlighted_button)->unHightlight();
						m_highlighted_button = (m_highlighted_button == 0) ? m_buttons.size() - 1 : m_highlighted_button - 1;
						m_buttons.at(m_highlighted_button)->hightlight();
					}
				}
			}
		}

		return res;
	}



	////////////////////////////////////MenuManager//////////////////////////////////////////

	MenuManager::MenuManager() noexcept
		: m_active(0)
		, is_active(false)
	{

	}


	void MenuManager::create(int _id) {

		//printf("created: %d\n", _id);

		m_menus[_id].clear();

		m_menus.at(_id).setFont(m_font);
	}


	void MenuManager::remove(int _id_menu) {

		if (_id_menu == m_active) {
			is_active = false;
		}

		m_menus.erase(_id_menu); 
	}


	void MenuManager::drawActive(sf::RenderTarget& _target) {

		if (is_active) {
			auto res = m_menus.find(m_active);

			if (res != m_menus.end()) {

				res->second.draw(_target);

			}
			else {
				assert(0);
			}
		}
	}

	bool MenuManager::checkEvent(const sf::Event& _event, const sf::RenderWindow& _window) {
		bool res = false;
		if (is_active) {
			res = m_menus.at(m_active).checkEvent(_event, _window);
		}
		else {
			assert(0);
		}
		return res;
	}
	bool MenuManager::checkPressing(const sf::Event& _event, const sf::RenderWindow& _window, int& _id_button) {

		bool res = false;
		if (is_active) {
			res = m_menus.at(m_active).checkPressing(_event, _window, _id_button);
		}
		else {
			assert(0);
		}

		return res;
	}


	bool MenuManager::activate(int _id) {

		auto result = m_menus.find(_id);

		if (result == m_menus.end()) {
			return false;
		}

		m_active = _id;
		is_active = true;

		return true;

	}


	bool MenuManager::isActive(int& _id_menu) {

		_id_menu = (is_active) ? m_active : 0;
		return is_active;
	}

} //namespace menu