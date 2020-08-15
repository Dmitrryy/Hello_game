#include "NodeGame.h"

namespace ezg {

	void Game::_setMainMenu_ () {
        m_menus.create(TipeMenu::Main);

        if (!m_menus.atMenu(TipeMenu::Main).setTexture(MENU_BACKGROUND1_FNAME)) {
            assert(0);
        }

        menu::Button _but;
        _but.setSize({ 0.15f, 0.095f });
        sf::Text _txt;
        _txt.setCharacterSize(MAIN_MENU_FONT_SIZE);
        _txt.setFillColor(MAIN_MENU_FONT_COLOR);

        _txt.setScale(0.25f, 0.25f);

        sf::Text _hl_txt = _txt;
        _hl_txt.setFillColor(sf::Color::Blue);
        sf::Text _pr_txt = _txt;
        _pr_txt.setFillColor(sf::Color::Black);


        _but.setText(_txt);
        _but.setText(_hl_txt, menu::Button::Stat::HighLight);
        _but.setText(_pr_txt, menu::Button::Stat::Pressed);
        _but.setPositionTxt(sf::Vector2f(0.5f, 0.5f));


        _but.setString("Game");
        _but.setScript(menu::Button::Script::Released, [this]() {
            this->changeMood(Mood::Game);
            });
        _but.setPosition({ 0.65f, 0.46f });
        m_menus.atMenu(TipeMenu::Main).addButton(_but);


        _but.setString("Exit");
        _but.setScript(menu::Button::Script::Released, [this]() {
            this->changeMood(Mood::Exit);
            });
        _but.setPosition({ 0.65f, 0.56f });
        m_menus.atMenu(TipeMenu::Main).addButton(_but);


        auto _bg(std::make_unique<menu::Image>());
        _bg->setSize(sf::Vector2f(1.f, 1.f));
        _bg->setTextureRect(sf::IntRect(0, 0, 1003, 563));
        m_menus.atMenu(TipeMenu::Main).addImage(std::move(_bg));
	}
	void Game::_setPauseMenu_() {

        m_menus.create(TipeMenu::Pause);

        if (!m_menus.atMenu(TipeMenu::Pause).setTexture(MENU_BACKGROUND_PAUSE_FNAME)) {
            assert(0);
        }

        menu::Button _but;
        _but.setSize({ 0.15f, 0.095f });
        sf::Text _txt;
        _txt.setCharacterSize(MAIN_MENU_FONT_SIZE);
        _txt.setFillColor(MAIN_MENU_FONT_COLOR);
        _txt.setScale(0.25f, 0.25f);

        sf::Text _hl_txt = _txt;
        _hl_txt.setFillColor(sf::Color::Blue);
        sf::Text _pr_txt = _txt;
        _pr_txt.setFillColor(sf::Color::Black);

        _but.setText(_txt);
        _but.setText(_hl_txt, menu::Button::Stat::HighLight);
        _but.setText(_pr_txt, menu::Button::Stat::Pressed);
        _but.setPositionTxt(sf::Vector2f(0.5f, 0.5f));


        _but.setString("Continue");
        _but.setScript(menu::Button::Script::Released, [this]() {
            this->changeMood(Mood::Game);
            });
        _but.setPosition({ 0.8f, 0.2f });
        m_menus.atMenu(TipeMenu::Pause).addButton(_but);

        _but.setString("Restart");
        _but.setScript(menu::Button::Script::Released, [this]() {
            this->changeMood(Mood::Restart);
            });
        _but.setPosition({ 0.8f, 0.3f });
        m_menus.atMenu(TipeMenu::Pause).addButton(_but);

        _but.setString("Menu");
        _but.setScript(menu::Button::Script::Released, [this]() {
            this->changeMood(Mood::MainMenu);
            });
        _but.setPosition({ 0.8f, 0.4f });
        m_menus.atMenu(TipeMenu::Pause).addButton(_but);


        auto _bg(std::make_unique<menu::Image>());
        _bg->setSize(sf::Vector2f(1.f, 1.f));
        _bg->setTextureRect(sf::IntRect(0, 0, 256, 144));
        m_menus.atMenu(TipeMenu::Pause).addImage(std::move(_bg));
	}
	void Game::_setDeathMenu_() {

        m_menus.create(TipeMenu::Death);

        if (!m_menus.atMenu(TipeMenu::Death).setTexture(MENU_BACKGROUND_DEATH_FNAME)) {
            assert(0);
        }

        menu::Button _but;
        _but.setSize({ 0.15f, 0.095f });
        sf::Text _txt;
        _txt.setCharacterSize(MAIN_MENU_FONT_SIZE);
        _txt.setFillColor(MAIN_MENU_FONT_COLOR);
        _txt.setScale(0.25f, 0.25f);

        sf::Text _hl_txt = _txt;
        _hl_txt.setFillColor(sf::Color::Blue);
        sf::Text _pr_txt = _txt;
        _pr_txt.setFillColor(sf::Color::Black);

        _but.setText(_txt);
        _but.setText(_hl_txt, menu::Button::Stat::HighLight);
        _but.setText(_pr_txt, menu::Button::Stat::Pressed);
        _but.setPositionTxt(sf::Vector2f(0.5f, 0.5f));

        _but.setString("Restart");
        _but.setScript(menu::Button::Script::Released, [this]() {
            this->changeMood(Mood::Restart);
            });
        _but.setPosition({ 0.5f, 0.55f });
        m_menus.atMenu(TipeMenu::Death).addButton(_but);

        _but.setString("Menu");
        _but.setScript(menu::Button::Script::Released, [this]() {
            this->changeMood(Mood::MainMenu);
            });
        _but.setPosition({ 0.5f, 0.65f });
        m_menus.atMenu(TipeMenu::Death).addButton(_but);


        auto _bg = std::make_unique<menu::Image>();
        _bg->setSize(sf::Vector2f(1.f, 1.f));
        _bg->setTextureRect(sf::IntRect(0, 0, 1920, 1080));
        m_menus.atMenu(TipeMenu::Death).addImage(std::move(_bg));

	}

	void Game::_setDbConsole_() {
        m_debug_cnsl.setFont(FONT_FOR_CNSL_FNAME);
        //Main page
        {
            m_debug_cnsl.create(ConsoleType::Main);
            m_debug_cnsl.atMenu(ConsoleType::Main).setTexture("Resource/Images/console_db.png");

            //set background
            {
                auto _bg(std::make_unique<menu::Image>());
                _bg->setSize(sf::Vector2f(0.4f, 0.7f));
                _bg->setTextureRect(sf::IntRect(0, 0, 100, 100));
                m_debug_cnsl.atMenu(ConsoleType::Main).addImage(std::move(_bg));
            }

            //set text
            sf::Text tmp_txt;
            tmp_txt.setFillColor(sf::Color(80, 80, 80, 255));
            tmp_txt.setFont(m_debug_cnsl.getFont());
            tmp_txt.setCharacterSize(30);
            tmp_txt.setScale(0.15f, 0.15f);
            {
                //_txt2 - item(id = 1) for data (text)
                auto _txt = std::make_unique<menu::Text>();
                _txt->setText(tmp_txt);
                _txt->setPosition(sf::Vector2f(0.005f, 0.05f));

                m_debug_cnsl.atMenu(ConsoleType::Main).addOtherItem(std::move(_txt));
            }


            menu::Button _but;
            _but.setSize({ 0.05f, 0.03f });

            _but.setTextureRect(sf::IntRect(115, 0, 85, 31));
            _but.setTextureRect(sf::IntRect(115, 0, 85, 31), menu::Button::Stat::HighLight);
            _but.setTextureRect(sf::IntRect(115, 31, 85, 31), menu::Button::Stat::Pressed);
            _but.setTexture(m_debug_cnsl.atMenu(ConsoleType::Main).getTexture());

            tmp_txt.setScale(0.15f, 0.15f);
            tmp_txt.setFillColor(sf::Color::White);
            _but.setText(tmp_txt);

            {//button to Main(pressed)
                auto _but1 = std::make_unique<menu::Button>(_but);
                _but1->setTextureRect(sf::IntRect(115, 31, 85, 31));
                _but1->setPosition({ 0.025f, 0.015f });
                _but1->setPositionTxt({ 0.5f, 0.5f });
                _but1->setString("main");
                m_debug_cnsl.atMenu(ConsoleType::Main).addOtherItem(std::move(_but1));
            }
            {//button to Hero
                auto _but1 = std::make_unique<menu::Button>(_but);
                _but1->setPosition({ 0.075f, 0.015f });
                _but1->setPositionTxt({ 0.5f, 0.5f });
                _but1->setString("Hero");
                _but1->setScript(menu::Button::Script::Released, [cnsl = &(this->m_debug_cnsl)]() {
                    cnsl->activate(ConsoleType::Hero);
                });
                m_debug_cnsl.atMenu(ConsoleType::Main).addButton(std::move(_but1));
            }
            {//button to Entities
                auto _but1 = std::make_unique<menu::Button>(_but);
                _but1->setPosition({ 0.125f, 0.015f });
                _but1->setPositionTxt({ 0.5f, 0.5f });
                _but1->setString("entities");
                _but1->setScript(menu::Button::Script::Released, [cnsl = &(this->m_debug_cnsl)]() {
                    cnsl->activate(ConsoleType::Entities);
                });
                m_debug_cnsl.atMenu(ConsoleType::Main).addButton(std::move(_but1));
            }
        }
        //Hero page
        {
            m_debug_cnsl.create(ConsoleType::Hero);
            m_debug_cnsl.atMenu(ConsoleType::Hero).setTexture(m_debug_cnsl.atMenu(ConsoleType::Main).getTexture());

            //set background
            {
                auto _bg(std::make_unique<menu::Image>());
                _bg->setSize(sf::Vector2f(0.4f, 0.7f));
                _bg->setTextureRect(sf::IntRect(0, 0, 100, 100));
                m_debug_cnsl.atMenu(ConsoleType::Hero).addImage(std::move(_bg));
            }
            //set text
            sf::Text tmp_txt;
            tmp_txt.setFillColor(sf::Color(80, 80, 80, 255));
            tmp_txt.setFont(m_debug_cnsl.getFont());
            tmp_txt.setCharacterSize(30);
            tmp_txt.setScale(0.15f, 0.15f);

            {//_txt - item(id = 1) for data (text)
                auto _txt = std::make_unique<menu::Text>();
                _txt->setText(tmp_txt);
                _txt->setPosition(sf::Vector2f(0.005f, 0.05f));

                m_debug_cnsl.atMenu(ConsoleType::Hero).addOtherItem(std::move(_txt));
            }

            //set button
            menu::Button _but;
            _but.setSize({ 0.05f, 0.03f });

            _but.setTextureRect(sf::IntRect(115, 0, 85, 31));
            _but.setTextureRect(sf::IntRect(115, 0, 85, 31), menu::Button::Stat::HighLight);
            _but.setTextureRect(sf::IntRect(115, 31, 85, 31), menu::Button::Stat::Pressed);
            _but.setTexture(m_debug_cnsl.atMenu(ConsoleType::Main).getTexture());

            tmp_txt.setScale(0.15f, 0.15f);
            tmp_txt.setFillColor(sf::Color::White);
            _but.setText(tmp_txt);

            {//button to Main
                auto _but1 = std::make_unique<menu::Button>(_but);
                _but1->setPosition({ 0.025f, 0.015f });
                _but1->setPositionTxt({ 0.5f, 0.5f });
                _but1->setString("main");
                _but1->setScript(menu::Button::Script::Released, [cnsl = &(this->m_debug_cnsl)]() {
                    cnsl->activate(ConsoleType::Main);
                });
                m_debug_cnsl.atMenu(ConsoleType::Hero).addButton(std::move(_but1));
            }
            {//button to Hero(pressed)
                auto _but1 = std::make_unique<menu::Button>(_but);
                _but1->setTextureRect(sf::IntRect(115, 31, 85, 31));
                _but1->setPosition({ 0.075f, 0.015f });
                _but1->setPositionTxt({ 0.5f, 0.5f });
                _but1->setString("Hero");
                m_debug_cnsl.atMenu(ConsoleType::Hero).addOtherItem(std::move(_but1));
            }
            {//button to Entities
                auto _but1 = std::make_unique<menu::Button>(_but);
                _but1->setPosition({ 0.125f, 0.015f });
                _but1->setPositionTxt({ 0.5f, 0.5f });
                _but1->setString("entities");
                _but1->setScript(menu::Button::Script::Released, [cnsl = &(this->m_debug_cnsl)]() {
                    cnsl->activate(ConsoleType::Entities);
                });
                m_debug_cnsl.atMenu(ConsoleType::Hero).addButton(std::move(_but1));
            }

        }
        //Entities page
        {
            m_debug_cnsl.create(ConsoleType::Entities);
            m_debug_cnsl.atMenu(ConsoleType::Entities).setTexture(m_debug_cnsl.atMenu(ConsoleType::Main).getTexture());

            //set background
            {
                auto _bg(std::make_unique<menu::Image>());
                _bg->setSize(sf::Vector2f(0.4f, 0.7f));
                _bg->setTextureRect(sf::IntRect(0, 0, 100, 100));
                m_debug_cnsl.atMenu(ConsoleType::Entities).addImage(std::move(_bg));
            }

            //set text
            sf::Text tmp_txt;
            tmp_txt.setFillColor(sf::Color(80, 80, 80, 255));
            tmp_txt.setFont(m_debug_cnsl.getFont());
            tmp_txt.setCharacterSize(30);
            tmp_txt.setScale(0.15f, 0.15f);
            {//id_entiti - item(id = 1) with id of entity
                auto id_entiti = std::make_unique<menu::DataInt>();
                id_entiti->set(19);
                m_debug_cnsl.atMenu(ConsoleType::Entities).addOtherItem(std::move(id_entiti));
            }
            {//_txt - item(id = 2) for data (text)
                auto _txt = std::make_unique<menu::Text>();
                _txt->setText(tmp_txt);
                _txt->setPosition(sf::Vector2f(0.005f, 0.05f));

                m_debug_cnsl.atMenu(ConsoleType::Entities).addOtherItem(std::move(_txt));
            }
            //set button
            menu::Button _but;
            _but.setSize({ 0.05f, 0.03f });

            _but.setTextureRect(sf::IntRect(115, 0, 85, 31));
            _but.setTextureRect(sf::IntRect(115, 0, 85, 31), menu::Button::Stat::HighLight);
            _but.setTextureRect(sf::IntRect(115, 31, 85, 31), menu::Button::Stat::Pressed);
            _but.setTexture(m_debug_cnsl.atMenu(ConsoleType::Main).getTexture());

            tmp_txt.setScale(0.15f, 0.15f);
            tmp_txt.setFillColor(sf::Color::White);
            _but.setText(tmp_txt);

            {//button to Main
                auto _but1 = std::make_unique<menu::Button>(_but);
                _but1->setPosition({ 0.025f, 0.015f });
                _but1->setPositionTxt({ 0.5f, 0.5f });
                _but1->setString("main");
                _but1->setScript(menu::Button::Script::Released, [cnsl = &(this->m_debug_cnsl)]() {
                    cnsl->activate(ConsoleType::Main);
                });
                m_debug_cnsl.atMenu(ConsoleType::Entities).addButton(std::move(_but1));
            }
            {//button to Hero
                auto _but1 = std::make_unique<menu::Button>(_but);
                _but1->setPosition({ 0.075f, 0.015f });
                _but1->setPositionTxt({ 0.5f, 0.5f });
                _but1->setString("Hero");
                _but1->setScript(menu::Button::Script::Released, [cnsl = &(this->m_debug_cnsl)]() {
                    cnsl->activate(ConsoleType::Hero);
                });
                m_debug_cnsl.atMenu(ConsoleType::Entities).addButton(std::move(_but1));
            }
            {//button to Entities(pressed)
                auto _but1 = std::make_unique<menu::Button>(_but);
                _but1->setTextureRect(sf::IntRect(115, 31, 85, 31));
                _but1->setPosition({ 0.125f, 0.015f });
                _but1->setPositionTxt({ 0.5f, 0.5f });
                _but1->setString("entities");
                m_debug_cnsl.atMenu(ConsoleType::Entities).addOtherItem(std::move(_but1));
            }
            {
                auto _but1 = std::make_unique<menu::Button>(_but);
                _but1->setPosition({ 0.225f, 0.685f });
                _but1->setPositionTxt({ 0.5f, 0.5f });
                _but1->setString("next");
                _but1->setScript(menu::Button::Script::Released, [cnsl = &(this->m_debug_cnsl)]() {
                    const gsl::not_null < menu::DataInt* > id_entity = dynamic_cast<menu::DataInt*>(cnsl->atMenu(ConsoleType::Entities).atItem(1));
                    id_entity->set(id_entity->get() + 1);
                });
                m_debug_cnsl.atMenu(ConsoleType::Entities).addButton(std::move(_but1));
            }
            {
                auto _but1 = std::make_unique<menu::Button>(_but);
                _but1->setPosition({ 0.175f, 0.685f });
                _but1->setPositionTxt({ 0.5f, 0.5f });
                _but1->setString("pref");
                _but1->setScript(menu::Button::Script::Released, [cnsl = &(this->m_debug_cnsl)]() {
                    const gsl::not_null < menu::DataInt* > id_entity = dynamic_cast<menu::DataInt*>(cnsl->atMenu(ConsoleType::Entities).atItem(1));
                    id_entity->set(id_entity->get() - 1);
                });
                m_debug_cnsl.atMenu(ConsoleType::Entities).addButton(std::move(_but1));
            }
        }
	}
}//namespace ezg