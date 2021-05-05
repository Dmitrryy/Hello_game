#include <iomanip>

#include "NodeGame.h"

#include "../Entities/Solid/Solid.h"

#include "../Entities/Bots/Mushroom/MushroomRed.h"
#include "../Entities/Bots/Mushroom/MushroomBlue.h"

#include "../Entities/Bots/Bee/Bee.h"
#include "../Entities/Bots/Snake/Snake.h"
#include "../Entities/Traps/Needle/Needle.h"


namespace ezg {

    Game::Game()
        : m_map()
        , m_hero()
        , m_menus()
        , m_mood(Mood::NotInitialized)
        , m_view(sf::FloatRect(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT))
        , m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Hello Game (ezg)")
        , m_cnslIsActive(false)
        //, m_refresh_rate_cnsl(1)
    {

        _initialize_();
        m_window.setVerticalSyncEnabled(true);
        //m_window.setFramerateLimit(100);

    }
    Game::~Game()
    {

        for (auto& ent : m_entities) {
            delete ent;
            ent = nullptr;
        }

    }


    void Game::_initialize_() {

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

        _setMainMenu_();
        _setPauseMenu_();
        _setDeathMenu_();

        _setDbConsole_();

        changeMood(Mood::MainMenu);
    }


    void Game::run() {

        sf::Clock clock;
        float periodUpdate = 0.f;
        while (getMood() != Game::Mood::Exit) {
            m_period = clock.restart().asSeconds();
            periodUpdate += m_period * GAME_SPEED_CONTROLLER;

            while (periodUpdate > FrameTime) {
                periodUpdate -= FrameTime;
                checkEvents();
                checkKeyBoard();

                update(FrameTime);
                m_update_VS_render++;
            }
            updateDbConsole(m_period);

            m_update_VS_render = 0;
            draw();
        }
    }


    void Game::draw() {

        m_window.clear();

        m_window.setView(m_view);

        sf::Transform states;
        switch (m_mood) 
        {
        case Mood::Game: 
            m_map.drawBackGround(m_window, states);
            m_map.drawMap(m_window, states);
            m_hero.draw(m_window, states);
            for (auto elem : m_entities) {
                elem->draw(m_window, states);
            }
            m_map.drawFrontGround(m_window, states);
            break; // case GameMod::Game

        case Mood::MainMenu:
            m_menus.drawActive(m_window);
            break;

            //these menus are drawn against the background of a stopped game
        case Mood::Pause:
        case Mood::Death:
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


    bool Game::loadLevelXML(const std::string& _fileXML) {

        //changeMood(Mood::Loading);

        //system("dir");
        tinyxml2::XMLDocument lvl;
        lvl.LoadFile(_fileXML.c_str());


        tinyxml2::XMLElement* mapXML = lvl.FirstChildElement("map");
        if (mapXML == nullptr) {
            throw std::invalid_argument("not found a 'map' in XML file");
        }

        tinyxml2::XMLElement* pr = mapXML->FirstChildElement("properties");
        if (pr != nullptr) {
            pr = pr->FirstChildElement("property");
            while (pr != nullptr) {

                if (0 == std::strcmp(pr->Attribute("name"), "view spawn(x)")) {
                    m_view.setCenter(pr->Int64Attribute("value"), m_view.getCenter().y);
                }
                else if (0 == std::strcmp(pr->Attribute("name"), "view spawn(y)")) {
                    m_view.setCenter(m_view.getCenter().x, pr->Int64Attribute("value"));
                }
                else {
                    std::cout << std::endl << "unknown property's name for map: " << pr->Attribute("name") << std::endl;
                }

                pr = pr->NextSiblingElement();
            }
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

                    //addObject(Entity::loadFromXML(obj));

                    const int64_t id = obj->Int64Attribute("id");
                    const int64_t x = obj->Int64Attribute("x");
                    const int64_t y = obj->Int64Attribute("y");

                    const int64_t width = obj->Int64Attribute("width");
                    const int64_t height = obj->Int64Attribute("height");
                    //todo ������� � Entity
                    if (obj->Attribute("type") == nullptr) {
                        std::cout << "unknown tipe object" << std::endl;
                    }
                    else if (0 == std::strcmp(obj->Attribute("type"), "solid")) {
                        std::unique_ptr<Entity> res = std::move(std::make_unique <Solid>(x, y, width, height));
                        res->setID(id);
                        addObject(std::move(res));
                    }
                    else if (0 == std::strcmp(obj->Attribute("type"), "solidabove")) {
                        std::unique_ptr<Entity> res = std::move(std::make_unique <SolidAbove>(x, y, width, height));
                        res->setID(id);
                        addObject(std::move(res));
                    }
                    else if (0 == std::strcmp(obj->Attribute("type"), "stairs")) {
                        std::unique_ptr<Entity> res = std::move(std::make_unique <Stairs>(x, y, width, height));
                        res->setID(id);
                        addObject(std::move(res));
                    }
                    else if (0 == std::strcmp(obj->Attribute("type"), "MushroomRed")) {
                        std::unique_ptr<MushroomRed> res = std::move(std::make_unique<MushroomRed>(0, 0, sf::IntRect(x, y, width, height), m_enemy_texture));

                        tinyxml2::XMLElement* pr = obj->FirstChildElement("properties");
                        if (pr != nullptr) {
                            pr = pr->FirstChildElement("property");
                            while (pr != nullptr) {

                                if (0 == std::strcmp(pr->Attribute("name"), "spawn_x")) {
                                    res->setPosition(pr->Int64Attribute("value"), res->getPosY());
                                }
                                else if (0 == std::strcmp(pr->Attribute("name"), "spawn_y")) {
                                    res->setPosition(res->getPosX(), pr->Int64Attribute("value"));
                                }
                                else if (0 == std::strcmp(pr->Attribute("name"), "hp")) {
                                    res->setHP(pr->Int64Attribute("value"));
                                }
                                else if (0 == std::strcmp(pr->Attribute("name"), "speed")) {
                                    res->setSpeedFactor(pr->Int64Attribute("value"));
                                }
                                else {
                                    std::cout << std::endl << "unknown property's name for MushroomRed: " << pr->Attribute("name") << std::endl;
                                }
                                pr = pr->NextSiblingElement();
                            }
                        }

                        res->setID(id);
                        addObject(std::move(res));
                    }
                    else if (0 == std::strcmp(obj->Attribute("type"), "MushroomBlue")) {

                        std::unique_ptr<MushroomBlue> res = std::move(std::make_unique<MushroomBlue>(0, 0, sf::IntRect(x, y, width, height), m_enemy_texture));

                        tinyxml2::XMLElement* pr = obj->FirstChildElement("properties");
                        if (pr != nullptr) {
                            pr = pr->FirstChildElement("property");
                            while (pr != nullptr) {

                                if (0 == std::strcmp(pr->Attribute("name"), "spawn_x")) {
                                    res->setPosition(pr->Int64Attribute("value"), res->getPosY());
                                }
                                else if (0 == std::strcmp(pr->Attribute("name"), "spawn_y")) {
                                    res->setPosition(res->getPosX(), pr->Int64Attribute("value"));
                                }
                                else if (0 == std::strcmp(pr->Attribute("name"), "hp")) {
                                    res->setHP(pr->Int64Attribute("value"));
                                }
                                else if (0 == std::strcmp(pr->Attribute("name"), "speed")) {
                                    res->setSpeedFactor(pr->Int64Attribute("value"));
                                }
                                else {
                                    std::cout << std::endl << "unknown property's name for MushroomBlue: " << pr->Attribute("name") << std::endl;
                                }
                                pr = pr->NextSiblingElement();
                            }
                        }

                        res->setID(id);
                        addObject(std::move(res));
                    }
                    else if (0 == std::strcmp(obj->Attribute("type"), "bee")) {
                        std::unique_ptr<Bee> res = std::move(std::make_unique <Bee>(x, y, sf::IntRect(x, y, width, height),
                            0, m_enemy_texture));

                        tinyxml2::XMLElement* pr = obj->FirstChildElement("properties");
                        if (pr != nullptr) {
                            pr = pr->FirstChildElement("property");
                            while (pr != nullptr) {

                                if (0 == std::strcmp(pr->Attribute("name"), "radius")) {
                                    res->setRadius(pr->Int64Attribute("value"));
                                }
                                else if (0 == std::strcmp(pr->Attribute("name"), "hp")) {
                                    res->setHP(pr->Int64Attribute("value"));
                                }
                                else if (0 == std::strcmp(pr->Attribute("name"), "damage")) {
                                    res->setDamage(pr->Int64Attribute("value"));
                                }
                                else {
                                    std::cout << std::endl << "unknown property's name for Bee: " << pr->Attribute("name") << std::endl;
                                }

                                pr = pr->NextSiblingElement();
                            }
                        }

                        res->setID(id);
                        addObject(std::move(res));
                    }
                    else if (0 == std::strcmp(obj->Attribute("type"), "snake")) {
                        std::unique_ptr<Snake> res = std::move(std::make_unique <Snake>(x, y, 0, m_enemy_texture));

                        tinyxml2::XMLElement* pr = obj->FirstChildElement("properties");
                        if (pr != nullptr) {
                            pr = pr->FirstChildElement("property");
                            while (pr != nullptr) {

                                if (0 == std::strcmp(pr->Attribute("name"), "radius")) {
                                    res->setRadius(pr->Int64Attribute("value"));
                                }
                                else if (0 == std::strcmp(pr->Attribute("name"), "hp")) {
                                    res->setHP(pr->Int64Attribute("value"));
                                }
                                else if (0 == std::strcmp(pr->Attribute("name"), "damage")) {
                                    res->setDamage(pr->Int64Attribute("value"));
                                }
                                else {
                                    std::cout << std::endl << "unknown property's name for Snake: " << pr->Attribute("name") << std::endl;
                                }

                                pr = pr->NextSiblingElement();
                            }
                        }


                        res->setID(id);
                        addObject(std::move(res));
                    }
                    else if (0 == std::strcmp(obj->Attribute("type"), "needle")) {
                    std::unique_ptr<Needle> res = std::move(std::make_unique <Needle>(x, y, width, height));

                    tinyxml2::XMLElement* pr = obj->FirstChildElement("properties");
                    if (pr != nullptr) {
                        pr = pr->FirstChildElement("property");
                        while (pr != nullptr) {

                            if (0 == std::strcmp(pr->Attribute("name"), "damage")) {
                                res->setDamage(pr->Int64Attribute("value"));
                            }
                            else {
                                std::cout << std::endl << "unknown property's name for Needle: " << pr->Attribute("name") << std::endl;
                            }

                            pr = pr->NextSiblingElement();
                        }
                    }
                        res->setID(id);
                        addObject(std::move(res));
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
                                else {
                                    std::cout << std::endl << "unknown property's name for playerspawn: " << pr->Attribute("name") << std::endl;
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

        //todo: �������� �� ��������� ������ (��� �� ���������) 
        return true;

    } // loadLevelXML


    void Game::checkEvents() {

        sf::Event event;
        while (m_window.pollEvent(event))
        {
            checkEventDbConsole(event);
            switch (m_mood)
            {
///////////////////////////Game///////////////////////////////////
            case Mood::Game:
                addObject(m_hero.checkEvent(event));
                switch (event.type)
                {
                case sf::Event::Closed:
                    m_mood = Mood::Exit;
                    break;

                case sf::Event::KeyPressed:
                    switch (event.key.code)
                    {
                    case sf::Keyboard::Escape:
                        changeMood(Mood::Pause);
                        break;
                    }
                    break; //case sf::Event::KeyPressed:
                }
                break; 

///////////////////////////Pause///////////////////////////////////
            case Mood::Pause:
                m_menus.checkEvent(event, m_window);
                switch (event.type)
                {
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) {
                        changeMood(Mood::Game);
                    }
                    break;

                case sf::Event::Closed:
                    m_mood = Mood::Exit;
                    break;
                }
                break;

///////////////////////////Death///////////////////////////////////
            case Mood::Death:
                m_menus.checkEvent(event, m_window);
                switch (event.type)
                {
                case sf::Event::Closed:
                    m_mood = Mood::Exit;
                    break;
                }
                break;

///////////////////////////MainMenu///////////////////////////////////
            case Mood::MainMenu:
                m_menus.checkEvent(event, m_window);
                switch (event.type)
                {
                case sf::Event::Closed:
                    m_mood = Mood::Exit;
                    break;
                }
                break;

            default:
                break;
            }
        }
    }


    void Game::checkKeyBoard() {

        switch (m_mood) {

        case Mood::Game:
            m_hero.checkKeyBoard();
            break;

        }
    }


    void Game::changeMood(Mood _new) {

        switch (_new)
        {
        case Mood::Exit:
            clear();
            m_mood = _new;
            break;

        case Mood::Game:
            if (m_mood != Mood::Pause) {
                clear();
                loadLevelXML(LVL_FNAME);
            }
            m_clock.restart();
            m_mood = _new;
            break;

        case Mood::Restart:
            clear();
            loadLevelXML(LVL_FNAME);
            m_clock.restart();
            m_mood = Mood::Game;
            break;

        case Mood::MainMenu:
            clear();
            m_menus.activate(TipeMenu::Main);
            m_mood = _new;
            break;

        case Mood::Pause:
            m_menus.activate(TipeMenu::Pause);
            m_mood = _new;
            break;

        case Mood::Death:
            m_menus.activate(TipeMenu::Death);
            m_mood = _new;
            break;

        default:
            
            break;
        }
    }


    void Game::clear() {

        m_window.clear();
        
        for (Entity* en : m_entities) {
            delete en;
            en = nullptr;
        }
        m_entities.clear();
        m_hero.restart();

        m_map.clear();
    }


    void Game::update(float _time) {

        switch (m_mood) 
        {
        case Mood::Game:

            m_hero.upEffect(_time);

            upAllPosition(_time, Direction::Horixontal);
            allColision(Direction::Horixontal);

            upAllPosition(_time, Direction::Vertical);
            allColision(Direction::Vertical);

            allOtherUpdate(_time);

            m_view.move(sf::Vector2f((getPosHeroX() - m_view.getCenter().x) * _time,
                (getPosHeroY() - m_view.getCenter().y) * _time));

            if (m_hero.getStat() == Entity::Stat::Death) {
                changeMood(Mood::Death);
            }
            break;

        }

    } // update


    void Game::upAllPosition(float time, Direction _dir) {

        m_hero.upPosition(time, _dir);

        for (Entity* elem : m_entities) {
            elem->upPosition(time, _dir);
        }

    }


    void Game::allColision(Direction _dir) {

        for (Entity* elem : m_entities) {

            addObject(m_hero.colision(elem, _dir));
            addObject(elem->colision(&m_hero, _dir));
        }

        for (Entity*  elem1 : m_entities) {
            for (Entity* elem2 : m_entities) {

                if (elem1 != elem2) {
                    elem1->colision(elem2, _dir);
                }

            }
        }

    }


    void Game::allOtherUpdate(float _time) {

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


    void Game::checkEventDbConsole(const sf::Event& _event) {

        if (_event.type == sf::Event::KeyPressed) {

            if (_event.key.code == sf::Keyboard::F1) {
                m_cnslIsActive = !m_cnslIsActive;
                m_debug_cnsl.activate(ConsoleType::Main);
            }

        }
        if (m_cnslIsActive) {
            m_debug_cnsl.checkEvent(_event, m_window);
        }

    }


    void Game::updateDbConsole(float ) {

        if (m_cnslIsActive) {
            
            int id = -1;
            m_debug_cnsl.isActive(id);
            if (id == ConsoleType::Main) {
                menu::Item* txt = m_debug_cnsl.atMenu(ConsoleType::Main).atItem(1);
                assert(txt != nullptr);

                if (txt->getType() == menu::Item::Type::Text) {
                    menu::Text* ptr_txt = dynamic_cast<menu::Text*>(txt);

                    ptr_txt->setText(localDebugString() + '\n' + m_map.debugString());
                }
                else { assert(0); }
            }
            else if (id == ConsoleType::Entities) {
                menu::Text* txt = dynamic_cast<menu::Text*>(
                    m_debug_cnsl.atMenu(ConsoleType::Entities).atItem(2));
                menu::DataInt* id_entity = dynamic_cast<menu::DataInt*>(
                    m_debug_cnsl.atMenu(ConsoleType::Entities).atItem(1));

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
                menu::Text* txt = dynamic_cast<menu::Text*>(
                    m_debug_cnsl.atMenu(ConsoleType::Hero).atItem(1));

                txt->setText(m_hero.debugString());
            }

        }

    }


    std::string Game::localDebugString() {

        using std::setw;
        using std::endl;
        using std::setfill;

        std::ostringstream out;
        out.setf(std::ios::left | std::ios::boolalpha);

        out << setw(13) << setfill('\t') << "FPS" << 1.f / m_period << endl
            << setw(16) << "game speed" << "  " << GAME_SPEED_CONTROLLER << endl
            << setw(20) << "update in a frame" << "   " << m_update_VS_render << endl
            << setw(15) << "game mood" << "  " << m_mood << endl
            << setw(20) << "number of objects" << "   " << m_entities.size() << endl
            << setw(14) << "time" << " " << m_clock.getElapsedTime().asSeconds() << endl
            << setw(20) << "window resolution" << "  " << m_window.getSize().x << 'x' << m_window.getSize().y << endl
            << setw(17) << "mouse coord" << "(" << sf::Mouse::getPosition().x << ", " << sf::Mouse::getPosition().y << ") || ("
              << static_cast<int>(m_window.mapPixelToCoords(sf::Mouse::getPosition()).x) << ", " 
              << static_cast<int>(m_window.mapPixelToCoords(sf::Mouse::getPosition()).y) << ')' << endl << endl
            << "Viev:" << endl
            << setw(22) << "coordinates(center)" << " (" << static_cast<int>(m_view.getCenter().x) << ", " << static_cast<int>(m_view.getCenter().y) << ')' << endl
            << setw(14) << "size" << static_cast<int>(m_view.getSize().x) << 'x' << static_cast<int>(m_view.getSize().y) << endl;

        return out.str();
    }


#define GMcase(a) case Game::Mood::a: \
res = #a; \
break;
    std::string enumName(Game::Mood _en) {

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
    std::ostream& operator<<(std::ostream& _stream, Game::Mood _en) {
        _stream << enumName(_en);
        return _stream;
    }

} //namecpace ezg