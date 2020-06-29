#include "../Header/Menu.h"


#include "../Header/NodeGame.h"



namespace ezg {


	void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const {

		static sf::Sprite sprite;


		sprite.setTexture(m_background);

		sprite.setTextureRect(sf::IntRect(0, 0, m_background.getSize().x, m_background.getSize().y));

		float scale_x = m_nodegame->m_view.getSize().x / m_background.getSize().x;
		float scale_y = m_nodegame->m_view.getSize().y / m_background.getSize().y;

		sprite.setPosition((m_nodegame->m_view.getCenter().x - m_nodegame->m_view.getSize().x / 2) / scale_x
			, (m_nodegame->m_view.getCenter().y - m_nodegame->m_view.getSize().y / 2) / scale_y);

		sf::Transform tr;
		tr.scale(scale_x, scale_y);

		target.draw(sprite, tr);


		for (const auto& t : m_buttons) {

			target.draw(t.second.m_text);

		}
	}


	void Menu::addButton(int _id, MenuButton _tipe, const std::string& _text, sf::Vector2f _pos) {

		m_buttons[_id] = Button(_tipe, _text, m_font, _pos);

		m_buttons[_id].m_text.setCharacterSize(m_def_size);
		m_buttons[_id].m_text.setFillColor(m_def_color);

	}


	void Menu::checkEvent(const sf::Event& _event) {

		const sf::Vector2i locPos = sf::Mouse::getPosition(m_nodegame->m_window);
		const sf::Vector2f globalPos(m_nodegame->m_view.getCenter().x - m_nodegame->m_view.getSize().x / 2 + locPos.x,
			m_nodegame->m_view.getCenter().y - m_nodegame->m_view.getSize().y / 2 + locPos.y);

		switch (_event.type)
		{
		case sf::Event::Closed:
			m_nodegame->m_mod = GameMod::Exit;
			break;

		case sf::Event::MouseButtonReleased:
			for (auto& butt : m_buttons) {

				if (butt.second.m_text.getGlobalBounds().contains(globalPos)) {

					_pressing_(butt.first);

				}

			}
			break;

		case sf::Event::KeyPressed:

			if (_event.key.code == sf::Keyboard::Escape) {
			
				if (m_nodegame->m_mod == GameMod::Pause) {

					m_nodegame->m_mod = GameMod::Game;
				}
			}

		} // switch (_event.type)
	}


	void Menu::checkKeyBoard() {

		const sf::Vector2i locPos = sf::Mouse::getPosition(m_nodegame->m_window);
		const sf::Vector2f globalPos(m_nodegame->m_view.getCenter().x - m_nodegame->m_view.getSize().x / 2 + locPos.x,
			m_nodegame->m_view.getCenter().y - m_nodegame->m_view.getSize().y / 2 + locPos.y);

		for (auto& butt : m_buttons) {

			//std::cout << globalPos.x << ' ' << globalPos.y << " | " <<
			//	butt.second.m_text.getGlobalBounds().left << ' ' << butt.second.m_text.getGlobalBounds().top
			//	<< ' ' << butt.second.m_text.getGlobalBounds().width << ' ' << butt.second.m_text.getGlobalBounds().height
			//	<<std::endl;

			if (butt.second.m_text.getGlobalBounds().contains(globalPos)) {

				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

					butt.second.m_text.setFillColor(sf::Color::Black);
				}
				else {
					butt.second.m_text.setFillColor(sf::Color::Blue);
				}
			}
			else {
				butt.second.m_text.setFillColor(m_def_color);
			}
		}

	}


	void Menu::_pressing_(int _id) {

		auto it = m_buttons.find(_id);
		assert(it != m_buttons.end());

		MenuButton butt = it->second.m_tipe;;

		if (butt == MenuButton::Continue) {

			m_nodegame->m_mod = GameMod::Game;

		}
		else if (butt == MenuButton::toExit) {

			m_nodegame->m_mod = GameMod::Exit;

		}
		else if (butt == MenuButton::toMainMenu) {

			m_nodegame->m_mod = GameMod::MainMenu;

			m_nodegame->m_view.setCenter(0.f, 0.f);
			setMenu(TipeMenu::Main);
			
		}
		else if (butt == MenuButton::toGame) {

			m_nodegame->restart();

			m_nodegame->m_mod = GameMod::Game;

		}
		else if (butt == MenuButton::Restart) {

			m_nodegame->restart();

			m_nodegame->m_mod = GameMod::Game;

		}

	}


	void Menu::setMenu(TipeMenu _tipe) {

		if (_tipe == TipeMenu::Main) {


			setTextColor(sf::Color(125, 113, 74));

			setBackGround(MENU_BACKGROUND1_FNAME);

			// -_-
			addButton(1, MenuButton::toGame, "Game",
				sf::Vector2f((m_nodegame->m_view.getCenter().x + m_nodegame->m_view.getSize().x / 5)
					, (m_nodegame->m_view.getCenter().y - m_nodegame->m_view.getSize().y / 3)));

			addButton(2, MenuButton::toExit, "Exit",
				sf::Vector2f((m_nodegame->m_view.getCenter().x + m_nodegame->m_view.getSize().x / 5)
					, (m_nodegame->m_view.getCenter().y - m_nodegame->m_view.getSize().y / 5)));
		}
		else if (_tipe == TipeMenu::Pause) {

			clear();

			setBackGround(MENU_BACKGROUND_PAUSE_FNAME);

			// -_-
			addButton(1, MenuButton::Continue, "Continue",
				sf::Vector2f((m_nodegame->m_view.getCenter().x + m_nodegame->m_view.getSize().x / 5)
					, (m_nodegame->m_view.getCenter().y - m_nodegame->m_view.getSize().y / 3)));

			addButton(2, MenuButton::Restart, "Restart",
				sf::Vector2f((m_nodegame->m_view.getCenter().x + m_nodegame->m_view.getSize().x / 5)
					, (m_nodegame->m_view.getCenter().y - m_nodegame->m_view.getSize().y / 6)));

			addButton(3, MenuButton::toMainMenu, "Menu",
				sf::Vector2f((m_nodegame->m_view.getCenter().x + m_nodegame->m_view.getSize().x / 5)
					, (m_nodegame->m_view.getCenter().y - m_nodegame->m_view.getSize().y / 36)));
		}

	}

}