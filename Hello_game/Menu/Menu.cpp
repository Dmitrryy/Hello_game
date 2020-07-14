#include "Menu.h"


namespace menu {

	///////////////////////////////////Button/////////////////////////////////////////

	Button::Button() noexcept
		: m_text()
		, m_pos_x(0)
		, m_pos_y(0)
	{

	}


	Button::Button(const std::string& _text, const sf::Font& _font, float _pos_x /*= 0.f*/, float _pos_y /*= 0.f*/)
		: m_text()
		, m_pos_x(_pos_x)
		, m_pos_y(_pos_y)
	{
		m_text.setFont(_font);
		m_text.setString(_text);

	}


	void Button::draw(sf::RenderTarget& _target) {


		const float width = m_text.getGlobalBounds().width;
		const float height = m_text.getGlobalBounds().height;

		m_text.setOrigin(width / 2, height / 2);

		const sf::View _view = _target.getView();

		const sf::Vector2f _view_center = _view.getCenter();
		const sf::Vector2f _view_size = _view.getSize();

		m_text.setPosition(_view_center.x - _view_size.x * (0.5f - m_pos_x),
			_view_center.y - _view_size.y * (0.5f - m_pos_y));

		//printf("%f %f | %f %f\n", m_text.getPosition().x, m_text.getPosition().y, _view.getCenter().x, _view.getCenter().y);

		_target.draw(m_text);
	}



	////////////////////////////////////Menu//////////////////////////////////////////

	Menu::Menu() noexcept
		: m_def_size(25)
		, m_def_color(sf::Color::White)
		, m_highlighted_button(-1)
		, m_pressed_button(-1)
	{

	}


	void Menu::draw(sf::RenderTarget& _target) {

		static sf::Sprite sprite;

		sprite.setTexture(m_background);
	

		sprite.setTextureRect(sf::IntRect(0, 0, m_background.getSize().x, m_background.getSize().y));

		const sf::View _view = _target.getView();

		const float scale_x = _view.getSize().x / m_background.getSize().x;
		const float scale_y = _view.getSize().y / m_background.getSize().y;

		sprite.setPosition((_view.getCenter().x - _view.getSize().x / 2) / scale_x
			, (_view.getCenter().y - _view.getSize().y / 2) / scale_y);


		sf::Transform tr;
		tr.scale(scale_x, scale_y);

		_target.draw(sprite, tr);


		for (auto& button : m_buttons) {

			if (button.first == m_pressed_button) {
				//hover button state
				button.second.setTextColor(sf::Color::Black);
			}
			else if (button.first == m_highlighted_button) {
				//button state when pressed
				button.second.setTextColor(sf::Color::Blue);
			}
			else {
				//button default state
				button.second.setTextColor(m_def_color);
			}
			//printf("%d %d\n", m_pressed_button, m_highlighted_button);

			button.second.draw(_target);

		}
	}


	void Menu::addButton(int _id, const std::string& _text, float _pos_x, float _pos_y) {

		m_buttons[_id] = Button(_text, m_font, _pos_x, _pos_y);

		m_buttons[_id].setTextSize(m_def_size);
		m_buttons[_id].setTextColor(m_def_color);

	}


	bool Menu::checkPressing(const sf::Event& _event, const sf::RenderWindow& _window, int& _id_button) {

		_id_button = 0;
		bool res = false;

		if (_event.type == sf::Event::MouseButtonReleased) {

			if (_event.mouseButton.button == sf::Mouse::Left) {
				const sf::Vector2f world_pos = _window.mapPixelToCoords(sf::Vector2i(_event.mouseButton.x, _event.mouseButton.y));

				for (auto& button : m_buttons) {

					if (button.second.getRect().contains(world_pos)) {
						res = true;
						_id_button = button.first;
						
					}
				}
				m_pressed_button = -1;
			}

		}
		else if (_event.type == sf::Event::MouseButtonPressed) {

			if (_event.mouseButton.button == sf::Mouse::Left) {
				const sf::Vector2f world_pos = _window.mapPixelToCoords(sf::Vector2i(_event.mouseButton.x, _event.mouseButton.y));

				bool stat = false;
				for (auto& button : m_buttons) {

					if (button.second.getRect().contains(world_pos)) {
						//button state when pressed
						m_pressed_button = button.first;
						stat = true;
					}

				}
				if (!stat) {
					m_pressed_button = -1;
				}
			}

		}
		else if (_event.type == sf::Event::MouseMoved) {

			const sf::Vector2f world_pos = _window.mapPixelToCoords(sf::Vector2i(_event.mouseMove.x, _event.mouseMove.y));

			bool stat = false;
			for (auto& button : m_buttons) {

				if (button.second.getRect().contains(world_pos)) {
					//hover button state
					m_highlighted_button = button.first;
					stat = true;
				}

			}
			if (!stat) {
				m_highlighted_button = -1;
			}
			
		}
		else if (_event.type == sf::Event::KeyPressed) {

			if (_event.key.code == sf::Keyboard::Enter && m_highlighted_button != -1) {
				res = true;
				_id_button = m_highlighted_button;
			}
			else if (_event.key.code == sf::Keyboard::W || _event.key.code == sf::Keyboard::Up) {
				if (m_highlighted_button == -1) {
					auto _but = m_buttons.begin();

					if (_but != m_buttons.end()) {
						m_highlighted_button = _but->first;
					}

				}
				else {
					auto _but = m_buttons.find(m_highlighted_button);

					//it is assumed that the menu can be without buttons and therefore a check is required
					if (_but != m_buttons.end()) {
						if (_but != m_buttons.begin()) {
							_but--;
							m_highlighted_button = _but->first;
						}
						else {
							auto it = m_buttons.end();
							it--;
							m_highlighted_button = it->first;
						}
					}

				}
			}
			else if (_event.key.code == sf::Keyboard::S || _event.key.code == sf::Keyboard::Down) {
				if (m_highlighted_button == -1) {
					auto _but = m_buttons.begin();

					if (_but != m_buttons.end()) {
						m_highlighted_button = _but->first;
					}

				}
				else {
					auto _but = m_buttons.find(m_highlighted_button);

					//it is assumed that the menu can be without buttons and therefore a check is required
					if (_but != m_buttons.end()) {
						_but++;
						if (_but != m_buttons.end()) {
							m_highlighted_button = _but->first;
						}
						else {
							auto it = m_buttons.begin();
							m_highlighted_button = it->first;
						}
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

} //namespace menu