
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
    {

        _initialize_();
        m_window.setFramerateLimit(150);

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


        m_hero.setWidth(HERO_WIDTH);
        m_hero.setHeight(HERO_HEIGHT);
        

        if (!m_hero.atAnimation().loadTexture(HERO_TEXTURE_FNAME)) {
            assert(0);
        }
        {
            m_hero.atAnimation().addAnimation(static_cast<int>(EntityAnimation::Walk));
            m_hero.atAnimation().addFrame(static_cast<int>(EntityAnimation::Walk), sf::IntRect(0, 8, 8, 8), 100.f);
            m_hero.atAnimation().addFrame(static_cast<int>(EntityAnimation::Walk), sf::IntRect(8, 8, 8, 8), 100.f);
            m_hero.atAnimation().addFrame(static_cast<int>(EntityAnimation::Walk), sf::IntRect(16, 8, 8, 8), 100.f);
            m_hero.atAnimation().addFrame(static_cast<int>(EntityAnimation::Walk), sf::IntRect(24, 8, 8, 8), 100.f);
            m_hero.atAnimation().addFrame(static_cast<int>(EntityAnimation::Walk), sf::IntRect(32, 8, 8, 8), 100.f);
            m_hero.atAnimation().addFrame(static_cast<int>(EntityAnimation::Walk), sf::IntRect(40, 8, 8, 8), 100.f);
            m_hero.atAnimation().addFrame(static_cast<int>(EntityAnimation::Walk), sf::IntRect(48, 8, 8, 8), 100.f);
            m_hero.atAnimation().addFrame(static_cast<int>(EntityAnimation::Walk), sf::IntRect(56, 8, 8, 8), 100.f);

            m_hero.atAnimation().addAnimation(static_cast<int>(EntityAnimation::Jump));
            m_hero.atAnimation().addFrame(static_cast<int>(EntityAnimation::Jump), sf::IntRect(0, 80, 8, 8), 100.f);
            m_hero.atAnimation().addFrame(static_cast<int>(EntityAnimation::Jump), sf::IntRect(8, 80, 8, 8), 120.f);
            m_hero.atAnimation().addFrame(static_cast<int>(EntityAnimation::Jump), sf::IntRect(16, 80, 8, 8), 500.f);
            m_hero.atAnimation().addFrame(static_cast<int>(EntityAnimation::Jump), sf::IntRect(24, 80, 8, 8), 4000.f);

            m_hero.atAnimation().addAnimation(static_cast<int>(EntityAnimation::Idle));
            m_hero.atAnimation().addFrame(static_cast<int>(EntityAnimation::Idle), sf::IntRect(0, 56, 8, 8), 100.f);
            m_hero.atAnimation().addFrame(static_cast<int>(EntityAnimation::Idle), sf::IntRect(8, 56, 8, 8), 400.f);
            m_hero.atAnimation().addFrame(static_cast<int>(EntityAnimation::Idle), sf::IntRect(16, 56, 8, 8), 100.f);
            m_hero.atAnimation().addFrame(static_cast<int>(EntityAnimation::Idle), sf::IntRect(24, 56, 8, 8), 400.f);
        }


        if (!m_enemy_texture.loadFromFile(ENEMY_TEXTURE_FNAME)) {
            assert(0);
        }


        m_menus.setFont(FONT_FNAME);

        _addMainMenu_();
        _addPauseMenu_();
        _addDeathMenu_();


        changeMood(GameMood::MainMenu);
    }


#define ButtonMain(a) m_menus.atMenu(TipeMenu::Main).atButton(a)
    void NodeGame::_addMainMenu_() {


        m_menus.create(TipeMenu::Main);


        m_menus.atMenu(TipeMenu::Main).addButton(MenuButton::toGame, "Game", 0.70f, 0.36f);
        ButtonMain(MenuButton::toGame).setTextColor(MAIN_MENU_FONT_COLOR);
        ButtonMain(MenuButton::toGame).setTextSize(MAIN_MENU_FONT_SIZE);

        m_menus.atMenu(TipeMenu::Main).addButton(MenuButton::toExit, "Exit", 0.70f, 0.46f);
        ButtonMain(MenuButton::toExit).setTextColor(MAIN_MENU_FONT_COLOR);
        ButtonMain(MenuButton::toExit).setTextSize(MAIN_MENU_FONT_SIZE);


        if (!m_menus.atMenu(TipeMenu::Main).setBackGround(MENU_BACKGROUND1_FNAME)) {
            assert(0);
        }

    }


#define ButtonPause(a) m_menus.atMenu(TipeMenu::Pause).atButton(a)
    void NodeGame::_addPauseMenu_() {

        m_menus.create(TipeMenu::Pause);


        m_menus.atMenu(TipeMenu::Pause).addButton(MenuButton::Continue, "Continue", 0.8f, 0.2f);
        ButtonPause(MenuButton::Continue).setTextColor(MAIN_MENU_FONT_COLOR);
        ButtonPause(MenuButton::Continue).setTextSize(MAIN_MENU_FONT_SIZE);

        m_menus.atMenu(TipeMenu::Pause).addButton(MenuButton::Restart, "Restart", 0.8f, 0.3f);
        ButtonPause(MenuButton::Restart).setTextColor(MAIN_MENU_FONT_COLOR);
        ButtonPause(MenuButton::Restart).setTextSize(MAIN_MENU_FONT_SIZE);

        m_menus.atMenu(TipeMenu::Pause).addButton(MenuButton::toMainMenu, "Menu", 0.8f, 0.4f);
        ButtonPause(MenuButton::toMainMenu).setTextColor(MAIN_MENU_FONT_COLOR);
        ButtonPause(MenuButton::toMainMenu).setTextSize(MAIN_MENU_FONT_SIZE);


        if (!m_menus.atMenu(TipeMenu::Pause).setBackGround(MENU_BACKGROUND_PAUSE_FNAME)) {
            assert(0);
        }

    }


#define ButtonDeath(a) m_menus.atMenu(TipeMenu::Death).atButton(a)
    void NodeGame::_addDeathMenu_() {

        m_menus.create(TipeMenu::Death); 

        m_menus.atMenu(TipeMenu::Death).addButton(MenuButton::Restart, "Restart", 0.5f, 0.55f);
        ButtonDeath(MenuButton::Restart).setTextColor(MENU_DEATH_FONT_COLOR);
        ButtonDeath(MenuButton::Restart).setTextSize(MENU_DEATH_FONT_SIZE);

        m_menus.atMenu(TipeMenu::Death).addButton(MenuButton::toMainMenu, "Menu", 0.5f, 0.65f);
        ButtonDeath(MenuButton::toMainMenu).setTextColor(MENU_DEATH_FONT_COLOR);
        ButtonDeath(MenuButton::toMainMenu).setTextSize(MENU_DEATH_FONT_SIZE);

        if (!m_menus.atMenu(TipeMenu::Death).setBackGround(MENU_BACKGROUND_DEATH_FNAME)) {
            assert(0);
        }

    }


    void NodeGame::draw(sf::RenderStates states) {

        m_window.clear();

        m_window.setView(m_view);

        //printf("%d\n", m_mood);

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

        m_window.display();

    }


    bool NodeGame::loadLevelXML(const std::string& _fileXML) {

        //changeMood(GameMood::Loading);

        system("dir");
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

                    const int64_t x = obj->Int64Attribute("x");
                    const int64_t y = obj->Int64Attribute("y");

                    const int64_t width = obj->Int64Attribute("width");
                    const int64_t height = obj->Int64Attribute("height");

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

        float _period = m_clock.getElapsedTime().asSeconds();
        m_clock.restart();

        //printf("%g\n",  _period );

        _period *= GAME_SPEED_CONTROLLER;
        m_time += _period;
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

            //std::cout << m_entities.size() << std::endl;

            upAllPosition(_period, Direction::Horixontal);
            allColision(Direction::Horixontal);


            upAllPosition(_period, Direction::Vertical);
            allColision(Direction::Vertical);

            allOtherUpdate(_period);

            //std::cout << static_cast<float>(_period) << std::endl;
            m_view.move(sf::Vector2f((getPosHeroX() * SCALE_ALL_X - m_view.getCenter().x) / _period / 7.f,
                (getPosHeroY() * SCALE_ALL_Y - m_view.getCenter().y) / _period / 7.f ));

            if (m_hero.getStat() == EntityStat::Death) {
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
        }

        for (const gsl::not_null < Entity* > elem1 : m_entities) {
            for (const gsl::not_null < Entity* > elem2 : m_entities) {

                if (elem1 != elem2) {
                    elem1->colision(elem2, _dir);
                }
                //Entity::colision(elem1, elem2, _dir);
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

        m_entities.remove_if([](auto elem) {
            return elem == nullptr;
            });

        //std::cout << m_entities.size() << std::endl;
    }


} //namecpace ezg