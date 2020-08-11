#include <iomanip>

#include "NodeGame.h"

#include "../Entities/Solid/Solid.h"

#include "../Entities/Bots/Mushroom/MushroomRed.h"
#include "../Entities/Bots/Mushroom/MushroomBlue.h"

#include "../Entities/Bots/Bee/Bee.h"
#include "../Entities/Bots/Snake/Snake.h"
#include "../Entities/Traps/Needle/Needle.h"

namespace ezg {

    NodeGame::NodeGame() noexcept
        : m_map()
        , m_hero()
        , m_menus()
        , m_mood(GameMood::NotInitialized)
        , m_view(sf::FloatRect(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT))
        , m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Hello Game (ezg)")
        , m_cnslIsActive(false)
    {

        _initialize_();
        //m_window.setFramerateLimit(100);

    }
    NodeGame::~NodeGame()
    {

        for (auto& ent : m_entities) {
            delete ent;
            ent = nullptr;
        }

    }


    void NodeGame::_initialize_() {

        m_window.setView(m_view);
        m_window.setKeyRepeatEnabled(false);

        m_view.zoom(0.2f);

        m_hero.setWidth(HERO_WIDTH);
        m_hero.setHeight(HERO_HEIGHT);
        

        if (!m_hero.atAnimation().loadTexture(HERO_TEXTURE_FNAME)) {
            assert(0);
        }
        {
            m_hero.atAnimation().addAnimation(static_cast<int>(Entity::Animation::Walk));
            m_hero.atAnimation().addFrame(static_cast<int>(Entity::Animation::Walk), sf::IntRect(0, 8, 8, 8), 0.1f);
            m_hero.atAnimation().addFrame(static_cast<int>(Entity::Animation::Walk), sf::IntRect(8, 8, 8, 8), 0.1f);
            m_hero.atAnimation().addFrame(static_cast<int>(Entity::Animation::Walk), sf::IntRect(16, 8, 8, 8), 0.1f);
            m_hero.atAnimation().addFrame(static_cast<int>(Entity::Animation::Walk), sf::IntRect(24, 8, 8, 8), 0.1f);
            m_hero.atAnimation().addFrame(static_cast<int>(Entity::Animation::Walk), sf::IntRect(32, 8, 8, 8), 0.1f);
            m_hero.atAnimation().addFrame(static_cast<int>(Entity::Animation::Walk), sf::IntRect(40, 8, 8, 8), 0.1f);
            m_hero.atAnimation().addFrame(static_cast<int>(Entity::Animation::Walk), sf::IntRect(48, 8, 8, 8), 0.1f);
            m_hero.atAnimation().addFrame(static_cast<int>(Entity::Animation::Walk), sf::IntRect(56, 8, 8, 8), 0.1f);

            m_hero.atAnimation().addAnimation(static_cast<int>(Entity::Animation::Jump));
            m_hero.atAnimation().addFrame(static_cast<int>(Entity::Animation::Jump), sf::IntRect(0, 80, 8, 8), 0.1f);
            m_hero.atAnimation().addFrame(static_cast<int>(Entity::Animation::Jump), sf::IntRect(8, 80, 8, 8), 0.12f);
            m_hero.atAnimation().addFrame(static_cast<int>(Entity::Animation::Jump), sf::IntRect(16, 80, 8, 8), 0.5f);
            m_hero.atAnimation().addFrame(static_cast<int>(Entity::Animation::Jump), sf::IntRect(24, 80, 8, 8), 5.f);

            m_hero.atAnimation().addAnimation(static_cast<int>(Entity::Animation::Idle));
            m_hero.atAnimation().addFrame(static_cast<int>(Entity::Animation::Idle), sf::IntRect(0, 56, 8, 8), 0.1f);
            m_hero.atAnimation().addFrame(static_cast<int>(Entity::Animation::Idle), sf::IntRect(8, 56, 8, 8), 0.4f);
            m_hero.atAnimation().addFrame(static_cast<int>(Entity::Animation::Idle), sf::IntRect(16, 56, 8, 8), 0.1f);
            m_hero.atAnimation().addFrame(static_cast<int>(Entity::Animation::Idle), sf::IntRect(24, 56, 8, 8), 0.4f);
        }


        if (!m_enemy_texture.loadFromFile(ENEMY_TEXTURE_FNAME)) {
            assert(0);
        }


        m_menus.setFont(FONT_FNAME);

        _addMainMenu_();
        _addPauseMenu_();
        _addDeathMenu_();

        //set Console (MenuManager)
        {
            m_debug_cnsl.setFont(FONT_FNAME);
            
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

                _but.texture(true);
                _but.setDefTextureRect(sf::IntRect(115, 0, 85, 31));
                _but.setHighlTextureRect(sf::IntRect(115, 0, 85, 31));
                _but.setPressedTextureRect(sf::IntRect(115, 31, 85, 31));
                _but.setTexture(m_debug_cnsl.atMenu(ConsoleType::Main).getTexture());

                tmp_txt.setScale(0.15f, 0.15f);
                tmp_txt.setFillColor(sf::Color::White);
                _but.setText(tmp_txt);

                {//button to Main(pressed)
                    auto _but1 = std::make_unique<menu::Button>(_but);
                    _but1->setDefTextureRect(sf::IntRect(115, 31, 85, 31));
                    _but1->setPosition({ 0.025f, 0.015f });
                    _but1->setPositionTxt({ 0.3f, 0.3f });
                    _but1->setString("main");
                    m_debug_cnsl.atMenu(ConsoleType::Main).addOtherItem(std::move(_but1));
                }
                {//button to Hero
                    auto _but1 = std::make_unique<menu::Button>(_but);
                    _but1->setPosition({ 0.075f, 0.015f });
                    _but1->setPositionTxt({ 0.3f, 0.3f });
                    _but1->setString("Hero");
                    m_debug_cnsl.atMenu(ConsoleType::Main).addButton(ConsoleButton::toHero, std::move(_but1));
                }
                {//button to Entities
                    auto _but1 = std::make_unique<menu::Button>(_but);
                    _but1->setPosition({ 0.125f, 0.015f });
                    _but1->setPositionTxt({ 0.17f, 0.2f });
                    _but1->setString("entities");
                    m_debug_cnsl.atMenu(ConsoleType::Main).addButton(ConsoleButton::toEntities, std::move(_but1));
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

                _but.texture(true);
                _but.setDefTextureRect(sf::IntRect(115, 0, 85, 31));
                _but.setHighlTextureRect(sf::IntRect(115, 0, 85, 31));
                _but.setPressedTextureRect(sf::IntRect(115, 31, 85, 31));
                _but.setTexture(m_debug_cnsl.atMenu(ConsoleType::Main).getTexture());

                tmp_txt.setScale(0.15f, 0.15f);
                tmp_txt.setFillColor(sf::Color::White);
                _but.setText(tmp_txt);

                {//button to Main
                    auto _but1 = std::make_unique<menu::Button>(_but);
                    _but1->setPosition({ 0.025f, 0.015f });
                    _but1->setPositionTxt({ 0.3f, 0.3f });
                    _but1->setString("main");
                    m_debug_cnsl.atMenu(ConsoleType::Hero).addButton(ConsoleButton::toMain, std::move(_but1));
                }
                {//button to Hero(pressed)
                    auto _but1 = std::make_unique<menu::Button>(_but);
                    _but1->setDefTextureRect(sf::IntRect(115, 31, 85, 31));
                    _but1->setPosition({ 0.075f, 0.015f });
                    _but1->setPositionTxt({ 0.3f, 0.3f });
                    _but1->setString("Hero");
                    m_debug_cnsl.atMenu(ConsoleType::Hero).addOtherItem(std::move(_but1));
                }
                {//button to Entities
                    auto _but1 = std::make_unique<menu::Button>(_but);
                    _but1->setPosition({ 0.125f, 0.015f });
                    _but1->setPositionTxt({ 0.17f, 0.2f });
                    _but1->setString("entities");
                    m_debug_cnsl.atMenu(ConsoleType::Hero).addButton(ConsoleButton::toEntities, std::move(_but1));
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

                _but.texture(true);
                _but.setDefTextureRect(sf::IntRect(115, 0, 85, 31));
                _but.setHighlTextureRect(sf::IntRect(115, 0, 85, 31));
                _but.setPressedTextureRect(sf::IntRect(115, 31, 85, 31));
                _but.setTexture(m_debug_cnsl.atMenu(ConsoleType::Main).getTexture());

                tmp_txt.setScale(0.15f, 0.15f);
                tmp_txt.setFillColor(sf::Color::White);
                _but.setText(tmp_txt);

                {//button to Main
                    auto _but1 = std::make_unique<menu::Button>(_but);
                    _but1->setPosition({ 0.025f, 0.015f });
                    _but1->setPositionTxt({ 0.3f, 0.3f });
                    _but1->setString("main");
                    m_debug_cnsl.atMenu(ConsoleType::Entities).addButton(ConsoleButton::toMain, std::move(_but1));
                }
                {//button to Hero
                    auto _but1 = std::make_unique<menu::Button>(_but);
                    _but1->setPosition({ 0.075f, 0.015f });
                    _but1->setPositionTxt({ 0.3f, 0.3f });
                    _but1->setString("Hero");
                    m_debug_cnsl.atMenu(ConsoleType::Entities).addButton(ConsoleButton::toHero, std::move(_but1));
                }
                {//button to Entities(pressed)
                    auto _but1 = std::make_unique<menu::Button>(_but);
                    _but1->setDefTextureRect(sf::IntRect(115, 31, 85, 31));
                    _but1->setPosition({ 0.125f, 0.015f });
                    _but1->setPositionTxt({ 0.17f, 0.2f });
                    _but1->setString("entities");
                    m_debug_cnsl.atMenu(ConsoleType::Entities).addOtherItem(std::move(_but1));
                }
                {
                    auto _but1 = std::make_unique<menu::Button>(_but);
                    _but1->setPosition({ 0.225f, 0.685f });
                    _but1->setPositionTxt({ 0.3f, 0.3f });
                    _but1->setString("next");
                    m_debug_cnsl.atMenu(ConsoleType::Entities).addButton(ConsoleButton::Next, std::move(_but1));
                }
                {
                    auto _but1 = std::make_unique<menu::Button>(_but);
                    _but1->setPosition({ 0.175f, 0.685f });
                    _but1->setPositionTxt({ 0.3f, 0.3f });
                    _but1->setString("pref");
                    m_debug_cnsl.atMenu(ConsoleType::Entities).addButton(ConsoleButton::Pref, std::move(_but1));
                }
            }
        }//end set Concole

        changeMood(GameMood::MainMenu);
    }

    void NodeGame::_addMainMenu_() {


        m_menus.create(TipeMenu::Main);

        if (!m_menus.atMenu(TipeMenu::Main).setTexture(MENU_BACKGROUND1_FNAME)) {
            assert(0);
        }

        menu::Button _but;
        _but.setSize({ 0.15f, 0.095f});
        sf::Text _txt;
        _txt.setCharacterSize(MAIN_MENU_FONT_SIZE);
        _txt.setFillColor(MAIN_MENU_FONT_COLOR);
        

        sf::Text _hl_txt = _txt;
        _hl_txt.setFillColor(sf::Color::Blue);
        sf::Text _pr_txt = _txt;
        _pr_txt.setFillColor(sf::Color::Black);

        _but.setDefText(_txt);
        _but.setHighlightingText(_hl_txt);
        _but.setPressedText(_pr_txt);
        _but.setPositionTxt(sf::Vector2f(0.5f, 0.5f));


        _but.setString("Game");
        _but.setPosition({ 0.65f, 0.46f });
        m_menus.atMenu(TipeMenu::Main).addButton(MenuButton::toGame, _but);


        _but.setString("Exit");
        _but.setPosition({ 0.65f, 0.56f });
        m_menus.atMenu(TipeMenu::Main).addButton(MenuButton::toExit, _but);


        auto _bg(std::make_unique<menu::Image>());
        _bg->setSize(sf::Vector2f(1.f, 1.f));
        _bg->setTextureRect(sf::IntRect(0, 0, 1003, 563));
        m_menus.atMenu(TipeMenu::Main).addImage(std::move(_bg));

    }

    void NodeGame::_addPauseMenu_() {

        m_menus.create(TipeMenu::Pause);

        if (!m_menus.atMenu(TipeMenu::Pause).setTexture(MENU_BACKGROUND_PAUSE_FNAME)) {
            assert(0);
        }

        menu::Button _but;
        _but.setSize({ 0.15f, 0.095f });
        sf::Text _txt;
        _txt.setCharacterSize(MAIN_MENU_FONT_SIZE);
        _txt.setFillColor(MAIN_MENU_FONT_COLOR);

        sf::Text _hl_txt = _txt;
        _hl_txt.setFillColor(sf::Color::Blue);
        sf::Text _pr_txt = _txt;
        _pr_txt.setFillColor(sf::Color::Black);

        _but.setDefText(_txt);
        _but.setHighlightingText(_hl_txt);
        _but.setPressedText(_pr_txt);
        _but.setPositionTxt(sf::Vector2f(0.5f, 0.5f));


        _but.setString("Continue");
        _but.setPosition({ 0.8f, 0.2f });
        m_menus.atMenu(TipeMenu::Pause).addButton(MenuButton::Continue, _but);

        _but.setString("Restart");
        _but.setPosition({ 0.8f, 0.3f });
        m_menus.atMenu(TipeMenu::Pause).addButton(MenuButton::Restart, _but);

        _but.setString("Menu");
        _but.setPosition({ 0.8f, 0.4f });
        m_menus.atMenu(TipeMenu::Pause).addButton(MenuButton::toMainMenu, _but);


        auto _bg(std::make_unique<menu::Image>());
        _bg->setSize(sf::Vector2f(1.f, 1.f));
        _bg->setTextureRect(sf::IntRect(0, 0, 256, 144));
        m_menus.atMenu(TipeMenu::Pause).addImage(std::move(_bg));
    }

    void NodeGame::_addDeathMenu_() {
       
        m_menus.create(TipeMenu::Death); 

        if (!m_menus.atMenu(TipeMenu::Death).setTexture(MENU_BACKGROUND_DEATH_FNAME)) {
            assert(0);
        }

        menu::Button _but;
        _but.setSize({ 0.15f, 0.095f });
        sf::Text _txt;
        _txt.setCharacterSize(MAIN_MENU_FONT_SIZE);
        _txt.setFillColor(MAIN_MENU_FONT_COLOR);

        sf::Text _hl_txt = _txt;
        _hl_txt.setFillColor(sf::Color::Blue);
        sf::Text _pr_txt = _txt;
        _pr_txt.setFillColor(sf::Color::Black);

        _but.setDefText(_txt);
        _but.setHighlightingText(_hl_txt);
        _but.setPressedText(_pr_txt);
        _but.setPositionTxt(sf::Vector2f(0.5f, 0.5f));

        _but.setString("Restart");
        _but.setPosition({ 0.5f, 0.55f });
        m_menus.atMenu(TipeMenu::Death).addButton(MenuButton::Restart, _but);

        _but.setString("Menu");
        _but.setPosition({ 0.5f, 0.65f });
        m_menus.atMenu(TipeMenu::Death).addButton(MenuButton::toMainMenu, _but);


        auto _bg(std::make_unique<menu::Image>());
        _bg->setSize(sf::Vector2f(1.f, 1.f));
        _bg->setTextureRect(sf::IntRect(0, 0, 1920, 1080));
        m_menus.atMenu(TipeMenu::Death).addImage(std::move(_bg));

    }


    void NodeGame::draw(sf::RenderStates states) {

        m_window.clear();

        m_window.setView(m_view);

        switch (m_mood) 
        {
        case GameMood::Game: 
            m_map.drawBackGround(m_window, states);
            m_map.drawMap(m_window, states);


            m_hero.draw(m_window, states);


            for (auto elem : m_entities) {

                elem->draw(m_window, states);

            }

            m_map.drawFrontGround(m_window, states);

            break; // case GameMod::Game
        

        case GameMood::MainMenu:

            m_menus.drawActive(m_window);
            
            break;


            //these menus are drawn against the background of a stopped game
        case GameMood::Pause:
        case GameMood::Death:

            m_map.drawBackGround(m_window, states);
            m_map.drawMap(m_window, states);


            m_hero.draw(m_window, states);

            for (auto elem : m_entities) {

                elem->draw(m_window, states);

            }
            m_map.drawFrontGround(m_window, states);


            m_menus.drawActive(m_window);

            break;

        }

        if (m_cnslIsActive) {
            m_debug_cnsl.drawActive(m_window);
        }

        m_window.display();

    }


    bool NodeGame::loadLevelXML(const std::string& _fileXML) {

        //changeMood(GameMood::Loading);

        //system("dir");
        tinyxml2::XMLDocument lvl;
        lvl.LoadFile(_fileXML.c_str());


        tinyxml2::XMLElement* mapXML = lvl.FirstChildElement("map");

        if (mapXML == nullptr) {
            throw std::invalid_argument("not found a 'map' in XML file");
        }

        {   //fix the card size (in tiles)
            const int64_t width = mapXML->Int64Attribute("width");
            const int64_t height = mapXML->Int64Attribute("height");
            assert(width >= 0 || height >= 0);

            m_map.setWidth(width);
            m_map.setHeight(height);
        }

        {   //fix the size of one tile
            const int64_t tilewidth = mapXML->Int64Attribute("tilewidth");
            const int64_t tileheight = mapXML->Int64Attribute("tileheight");
            assert(tilewidth >= 0 || tileheight >= 0);

            m_map.setTileSize(sf::Vector2u(tilewidth, tileheight));
        }


        tinyxml2::XMLElement* layer = mapXML->FirstChildElement();

        while (layer != nullptr) {

            if (0 == std::strcmp(layer->Name(), "editorsettings")) {

                if (0 != std::strcmp(layer->FirstChildElement("export")->Attribute("format"), "csv")) {
                    throw std::invalid_argument("format is not csv (editorsettings)");
                }

            }
            else if (0 == std::strcmp(layer->Name(), "tileset")) {

                if (!m_map.setTexture(layer->FirstChildElement("image")->Attribute("source"))) {
                    throw std::invalid_argument("image upload error");
                }

            }
            else if (0 == std::strcmp(layer->Name(), "layer")) {

                if (0 == std::strcmp(layer->Attribute("name"), "background")) {
                    
                    m_map.addBackGround(layer);
                }
                else if (0 == std::strcmp(layer->Attribute("name"), "frontground")) {

                    m_map.addFrontGround(layer); 
                }
                else {

                    m_map.addLayer(layer);
                }

            }
            else if (0 == std::strcmp(layer->Name(), "objectgroup")) {

                tinyxml2::XMLElement* obj = layer->FirstChildElement("object");

                while (obj != nullptr) {

                    //const int64_t id = obj->Int64Attribute("id");
                    const int64_t x = obj->Int64Attribute("x");
                    const int64_t y = obj->Int64Attribute("y");

                    const int64_t width = obj->Int64Attribute("width");
                    const int64_t height = obj->Int64Attribute("height");
                    //todo вынести в Entity
                    if (obj->Attribute("type") == nullptr) {

                        std::cout << "unknown tipe object" << std::endl;
                    }
                    else if (0 == std::strcmp(obj->Attribute("type"), "solid")) {

                        addObject(std::move(std::make_unique <Solid>(x, y, width, height)));
                    }
                    else if (0 == std::strcmp(obj->Attribute("type"), "solidabove")) {

                        addObject(std::move(std::make_unique <SolidAbove>(x, y, width, height)));
                    }
                    else if (0 == std::strcmp(obj->Attribute("type"), "stairs")) {

                        addObject(std::move(std::make_unique <Stairs>(x, y, width, height)));
                    }
                    else if (0 == std::strcmp(obj->Attribute("type"), "MushroomRed")) {

                        int64_t pos_x = 0;
                        int64_t pos_y = 0;

                        tinyxml2::XMLElement* pr = obj->FirstChildElement("properties");
                        if (pr != nullptr) {
                            pr = pr->FirstChildElement("property");
                            while (pr != nullptr) {

                                if (0 == std::strcmp(pr->Attribute("name"), "spawn_x")) {
                                    pos_x = pr->Int64Attribute("value");
                                }
                                else if (0 == std::strcmp(pr->Attribute("name"), "spawn_y")) {
                                    pos_y = pr->Int64Attribute("value");
                                }

                                pr = pr->NextSiblingElement();
                            }
                        }

                        
                        addObject(std::move(std::make_unique<MushroomRed>(pos_x, pos_y, sf::IntRect(x, y, width, height), m_enemy_texture)));
                    }
                    else if (0 == std::strcmp(obj->Attribute("type"), "MushroomBlue")) {

                        int64_t pos_x = 0;
                        int64_t pos_y = 0;

                        tinyxml2::XMLElement* pr = obj->FirstChildElement("properties");
                        if (pr != nullptr) {
                            pr = pr->FirstChildElement("property");
                            while (pr != nullptr) {

                                if (0 == std::strcmp(pr->Attribute("name"), "spawn_x")) {
                                    pos_x = pr->Int64Attribute("value");
                                }
                                else if (0 == std::strcmp(pr->Attribute("name"), "spawn_y")) {
                                    pos_y = pr->Int64Attribute("value");
                                }

                                pr = pr->NextSiblingElement();
                            }
                        }


                        addObject(std::move(std::make_unique<MushroomBlue>(pos_x, pos_y, sf::IntRect(x, y, width, height), m_enemy_texture)));
                    }
                    else if (0 == std::strcmp(obj->Attribute("type"), "bee")) {

                        int64_t radius = 0;

                        tinyxml2::XMLElement* pr = obj->FirstChildElement("properties");
                        if (pr != nullptr) {
                            pr = pr->FirstChildElement("property");
                            while (pr != nullptr) {

                                if (0 == std::strcmp(pr->Attribute("name"), "radius")) {
                                    radius = pr->Int64Attribute("value");
                                }

                                pr = pr->NextSiblingElement();
                            }
                        }

                        addObject(std::move(std::make_unique <Bee>(x, y, sf::IntRect(x, y, width, height), 
                            radius, m_enemy_texture)));
                    }
                    else if (0 == std::strcmp(obj->Attribute("type"), "snake")) {

                        int64_t radius = 0;

                        tinyxml2::XMLElement* pr = obj->FirstChildElement("properties");
                        if (pr != nullptr) {
                            pr = pr->FirstChildElement("property");
                            while (pr != nullptr) {

                                if (0 == std::strcmp(pr->Attribute("name"), "radius")) {
                                    radius = pr->Int64Attribute("value");
                                }

                                pr = pr->NextSiblingElement();
                            }
                        }

                        addObject(std::move(std::make_unique <Snake>(x, y, radius, m_enemy_texture)));


                    }
                    else if (0 == std::strcmp(obj->Attribute("type"), "needle")) {

                        addObject(std::move(std::make_unique <Needle>(x, y, width, height)));
                    }
                    else if (0 == std::strcmp(obj->Attribute("type"), "playerspawn")) {

                        m_hero.setPosition(x, y);
                        
                        //advanced setting
                        tinyxml2::XMLElement* pr = obj->FirstChildElement("properties");
                        if (pr != nullptr) {
                            pr = pr->FirstChildElement("property");
                            while (pr != nullptr) {

                                if (0 == std::strcmp(pr->Attribute("name"), "hp")) {
                                    m_hero.setHp(pr->IntAttribute("value"));
                                    //assert(m_hero.m_hp > 0);
                                }
                              
                                pr = pr->NextSiblingElement();
                            }
                        }

                    }
                    else {

                        std::cout << "unknown tipe object: " << obj->Attribute("type") << std::endl;
                    }

                    obj = obj->NextSiblingElement();

                } // while (obj != nullptr)
            }

            layer = layer->NextSiblingElement();

        } // while (layer != nullptr)

        //todo: проверка на состояние уровня (все ли загружено) 
        return true;

    } // loadLevelXML


    void NodeGame::checkEvents() noexcept {

        int id = 0;
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            checkEventDbConsole(event);
            switch (m_mood)
            {
///////////////////////////Game///////////////////////////////////
            case GameMood::Game:
                m_hero.checkEvent(event);
                switch (event.type)
                {
                    
                case sf::Event::Closed:
                    m_mood = GameMood::Exit;
                    break;

                   
                case sf::Event::KeyPressed:
                    switch (event.key.code)
                    {

                    case sf::Keyboard::Escape:
                        changeMood(GameMood::Pause);
                        break;


                    case sf::Keyboard::LControl:
                    case sf::Keyboard::RControl:
                        addObject(std::move(m_hero.fire()));
                        break;

                    }
                    break; //case sf::Event::KeyPressed:


                case sf::Event::KeyReleased:

                    switch (event.key.code) 
                    {

                    }
                    break; //case sf::Event::KeyReleased:
                }
                break; 


///////////////////////////Pause///////////////////////////////////
            case GameMood::Pause:
                if (m_menus.checkPressing(event, m_window, id)) {
                    if (id == MenuButton::Continue) {
                        changeMood(GameMood::Game);
                    }
                    else if (id == MenuButton::Restart) {
                        m_mood = GameMood::Restart;
                        changeMood(GameMood::Game);
                    }
                    else if (id == MenuButton::toMainMenu) {
                        changeMood(GameMood::MainMenu);
                    }
                }
                switch (event.type)
                {
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) {
                        changeMood(GameMood::Game);
                    }
                    break;


                case sf::Event::Closed:
                    m_mood = GameMood::Exit;
                    break;
                }
                break;


///////////////////////////Death///////////////////////////////////
            case GameMood::Death:
                if (m_menus.checkPressing(event, m_window, id)) {
                    if (id == MenuButton::Restart) {
                        m_mood = GameMood::Restart;
                        changeMood(GameMood::Game);
                    }
                    else if (id == MenuButton::toMainMenu) {
                        changeMood(GameMood::MainMenu);
                    }
                }
                switch (event.type)
                {

                case sf::Event::Closed:
                    m_mood = GameMood::Exit;
                    break;
                }
                break;


///////////////////////////MainMenu///////////////////////////////////
            case GameMood::MainMenu:
                if (m_menus.checkPressing(event, m_window, id)) {
                    if (id == MenuButton::toGame) {
                        changeMood(GameMood::Game);
                    }
                    else if (id == MenuButton::toExit) {
                        changeMood(GameMood::Exit);
                    }
                }
                switch (event.type)
                {

                case sf::Event::Closed:
                    m_mood = GameMood::Exit;
                    break;
                }
            
                break;


////////////////////////////////////////////////////////////////////////
            default:
                
                break;
            }
        }


    }


    void NodeGame::checkKeyBoard() {

        switch (m_mood) {

        case GameMood::Game:
            m_hero.checkKeyBoard();
            break;

        case GameMood::MainMenu:
        case GameMood::Death   :
        case GameMood::Pause   :

            //m_menus.checkKeyBoard(m_window);

            break;
        }

    }


    void NodeGame::changeMood(GameMood _new) {

        switch (_new)
        {
        case GameMood::Exit:
            clear();
            break;


        case GameMood::Game:
            if (m_mood != GameMood::Pause) {
                clear();
                loadLevelXML(LVL_FNAME);
            }
            m_clock.restart();
            break;


        case GameMood::MainMenu:
            clear();
            m_menus.activate(TipeMenu::Main);
            break;


        case GameMood::Pause:
            m_menus.activate(TipeMenu::Pause);
            break;


        case GameMood::Death:
            m_menus.activate(TipeMenu::Death);
            break;

        default:
            
            break;
        }
        m_mood = _new;
    }


    void NodeGame::clear() {

        m_window.clear();
        
        for (Entity* en : m_entities) {
            delete en;
            en = nullptr;
        }
        m_entities.clear();
        m_hero.restart();

        m_map.clear();
    }


    void NodeGame::update() {

        m_period = m_clock.restart().asSeconds();
        //m_period = _time.asSeconds();

        updateDbConsole(m_period);

        m_period *= GAME_SPEED_CONTROLLER;
        m_time += m_period;



//************************************************************
// алгоритм(для каждого элемента):
//      1. смещаем все элементы по х (устанавливаем on_ground = false т.к. персонаж может упасть)
//      2. проверяем на взаиможействие с картой
//      3. смещаем все элементы по у
//      4. проверяем на взаимодействие с картой
//      6. устанавливаем спрайт в новую позицию
// все в цикл с проверкой на dзаимодействие элементов между собой
//************************************************************
        switch (m_mood) 
        {
        case GameMood::Game:

            m_hero.upEffect(m_period);

            upAllPosition(m_period, Direction::Horixontal);
            allColision(Direction::Horixontal);


            upAllPosition(m_period, Direction::Vertical);
            allColision(Direction::Vertical);

            allOtherUpdate(m_period);

            m_view.move(sf::Vector2f((getPosHeroX() - m_view.getCenter().x) * m_period,
                (getPosHeroY() - m_view.getCenter().y) * m_period));

            if (m_hero.getStat() == Entity::Stat::Death) {
                changeMood(GameMood::Death);
            }
            break;

        }

    } // update


    void NodeGame::upAllPosition(float time, Direction _dir) {

        m_hero.upPosition(time, _dir);

        for (const gsl::not_null < Entity* > elem : m_entities) {
            elem->upPosition(time, _dir);
        }

    }


    void NodeGame::allColision(Direction _dir) {

        for (const gsl::not_null < Entity* > elem : m_entities) {

            addObject(m_hero.colision(elem, _dir));
            addObject(elem->colision(&m_hero, _dir));
        }

        for (const gsl::not_null < Entity* > elem1 : m_entities) {
            for (const gsl::not_null < Entity* > elem2 : m_entities) {

                if (elem1 != elem2) {
                    elem1->colision(elem2, _dir);
                }

            }
        }

    }


    void NodeGame::allOtherUpdate(float _time) {

        m_hero.otherUpdate(_time);

        for (auto& e : m_entities) {
            e->otherUpdate(_time);
        }

        for (auto& e : m_entities) {
            if (!e->alive()) {
                delete e;
                e = nullptr;
            }
        }

        m_entities.remove_if([](auto elem) noexcept {
            return elem == nullptr;
            });

    }


    void NodeGame::checkEventDbConsole(const sf::Event& _event) {


        if (_event.type == sf::Event::KeyPressed) {

            if (_event.key.code == sf::Keyboard::F1) {
                m_cnslIsActive = !m_cnslIsActive;
                m_debug_cnsl.activate(ConsoleType::Main);
            }

        }

        if (m_cnslIsActive) {

            int id = 0;
            m_debug_cnsl.isActive(id);
            if (id == ConsoleType::Main) {
                if (m_debug_cnsl.checkPressing(_event, m_window, id)) {
                    if (id == ConsoleButton::toEntities) {
                        m_debug_cnsl.activate(ConsoleType::Entities);
                    }
                    else if (id == ConsoleButton::toHero) {
                        m_debug_cnsl.activate(ConsoleType::Hero);
                    }
                }
            }
            else if (id == ConsoleType::Entities) {
                if (m_debug_cnsl.checkPressing(_event, m_window, id)) {
                    if (id == ConsoleButton::toMain) {
                        m_debug_cnsl.activate(ConsoleType::Main);
                    }
                    else if (id == ConsoleButton::toHero) {
                        m_debug_cnsl.activate(ConsoleType::Hero);
                    }
                    else if (id == ConsoleButton::Next) {
                        gsl::not_null < menu::DataInt* > id_entity = dynamic_cast<menu::DataInt*>(m_debug_cnsl.atMenu(ConsoleType::Entities).atItem(1));
                        id_entity->set(id_entity->get() + 1);
                    }
                    else if (id == ConsoleButton::Pref) {
                        gsl::not_null < menu::DataInt* > id_entity = dynamic_cast<menu::DataInt*>(m_debug_cnsl.atMenu(ConsoleType::Entities).atItem(1));
                        id_entity->set(id_entity->get() - 1);
                    }
                }
            }
            else if (id == ConsoleType::Hero) {
                if (m_debug_cnsl.checkPressing(_event, m_window, id)) {
                    if (id == ConsoleButton::toMain) {
                        m_debug_cnsl.activate(ConsoleType::Main);
                    }
                    else if (id == ConsoleButton::toEntities) {
                        m_debug_cnsl.activate(ConsoleType::Entities);
                    }
                }
            }

        }

    }


    void NodeGame::updateDbConsole(float _time) {

        if (m_cnslIsActive) {
            
            int id = -1;
            m_debug_cnsl.isActive(id);
            if (id == ConsoleType::Main) {
                menu::Item* txt = m_debug_cnsl.atMenu(ConsoleType::Main).atItem(1);

                if (txt->getType() == menu::Item::Type::Text) {
                    auto ptr_txt = dynamic_cast<menu::Text*>(txt);

                    ptr_txt->setText(localDebugString() + '\n' + m_map.debugString());
                }
                else { assert(0); }
            }
            else if (id == ConsoleType::Entities) {
                gsl::not_null < menu::Text* > txt = dynamic_cast<menu::Text*>(m_debug_cnsl.atMenu(ConsoleType::Entities).atItem(2));
                gsl::not_null < menu::DataInt* > id_entity = dynamic_cast<menu::DataInt*>(m_debug_cnsl.atMenu(ConsoleType::Entities).atItem(1));

                auto ent = std::find_if(m_entities.cbegin(), m_entities.cend(), [i = id_entity->get()](auto _enttity) {
                    return i == _enttity->getID();
                    });
                
                if (ent != m_entities.cend()) {

                    txt->setText((*ent)->DebugStr());
                }
                else {
                    txt->setText("not founded: " + std::to_string(id_entity->get()));
                }
            }
            else if (id == ConsoleType::Hero) {
                gsl::not_null < menu::Text* > txt = dynamic_cast<menu::Text*>(m_debug_cnsl.atMenu(ConsoleType::Hero).atItem(1));

                txt->setText(m_hero.debugString());
            }

        }

    }


    std::string NodeGame::localDebugString() {

        using std::setw;
        using std::endl;
        using std::setfill;

        std::ostringstream out;
        out.setf(std::ios::left | std::ios::boolalpha);

        out << setw(13) << setfill('\t') << "FPS" << 1.f / m_period << endl
            << setw(16) << "game speed" << "  " << GAME_SPEED_CONTROLLER << endl
            << setw(15) << "game mood" << "  " << m_mood << endl
            << setw(20) << "number of objects" << "   " << m_entities.size() << endl
            << setw(14) << "m_time" << " " << m_time << endl
            << setw(20) << "window resolution" << "  " << m_window.getSize().x << 'x' << m_window.getSize().y << endl
            << setw(17) << "mouse coord" << "(" << sf::Mouse::getPosition().x << ", " << sf::Mouse::getPosition().y << ") || ("
              << static_cast<int>(m_window.mapPixelToCoords(sf::Mouse::getPosition()).x) << ", " 
              << static_cast<int>(m_window.mapPixelToCoords(sf::Mouse::getPosition()).y) << ')' << endl << endl
            << "Viev:" << endl
            << setw(22) << "coordinates(center)" << " (" << static_cast<int>(m_view.getCenter().x) << ", " << static_cast<int>(m_view.getCenter().y) << ')' << endl
            << setw(14) << "size" << static_cast<int>(m_view.getSize().x) << 'x' << static_cast<int>(m_view.getSize().y) << endl;

        return out.str();
    }


#define GMcase(a) case GameMood::a: \
res = #a; \
break;
    std::string enumName(GameMood _en) {

        std::string res;

        switch (_en)
        {
            GMcase(NotInitialized);
            GMcase(Loading);
            GMcase(MainMenu);
            GMcase(Pause);
            GMcase(Restart);
            GMcase(Game);
            GMcase(Death);
            GMcase(Exit);
        }

        return res;
    }
    std::ostream& operator<<(std::ostream& _stream, GameMood _en) {
        _stream << enumName(_en);
        return _stream;
    }

} //namecpace ezg