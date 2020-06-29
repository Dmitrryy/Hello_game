
#include "../Header/NodeGame.h"


namespace ezg {


    NodeGame::~NodeGame()
    {

        m_map = nullptr;
        m_hero = nullptr;

        m_entities.clear();

        m_window.close();
    }


    void NodeGame::draw(sf::RenderStates states) {

        m_window.clear();

        switch (m_mod) 
        {
        case GameMod::Game: 
            m_map->drawBackGround(m_window, states);
            m_map->drawMap(m_window, states);


            m_hero->draw(m_window, states);


            for (auto elem : m_entities) {

                elem->draw(m_window, states);

            }

            m_map->drawFrontGround(m_window, states);

            break; // case GameMod::Game
        

        case GameMod::MainMenu:

            m_menu.draw(m_window, states);
            
            break;


        case GameMod::Pause:

            m_map->drawBackGround(m_window, states);
            m_map->drawMap(m_window, states);


            m_hero->draw(m_window, states);

            for (auto elem : m_entities) {

                elem->draw(m_window, states);

            }
            m_map->drawFrontGround(m_window, states);


            m_menu.draw(m_window, states);

            break;

        }

        m_window.display();

        m_window.setView(m_view);
    }


    bool NodeGame::loadLevelXML(const std::string& _fileXML) {

        m_mod = GameMod::Loading;


        tinyxml2::XMLDocument lvl;
        lvl.LoadFile(_fileXML.c_str());


        tinyxml2::XMLElement* mapXML = lvl.FirstChildElement("map");

        if (mapXML == nullptr) {
            throw std::invalid_argument("not found a 'map' in XML file");
        }

       

        m_map =  std::move(std::make_unique <TileMap>());
        m_hero = std::move(std::make_unique <Hero>());

        m_hero->setWidth (HERO_WIDTH);
        m_hero->setHeight(HERO_HEIGHT);

        m_hero->SetSprite(HERO_TEXTURE_FNAME, HERO_TEXTURE_LOC_X, HERO_TEXTURE_LOC_Y, HERO_WIDTH, HERO_HEIGHT);


        {   //фиксируем размер карты (в тайлах)
            const int64_t width = mapXML->Int64Attribute("width");
            const int64_t height = mapXML->Int64Attribute("height");
            assert(width >= 0 || height >= 0);

            m_map->setWidth(width);
            m_map->setHeight(height);
        }

        {   //фиксируем размер одного тайла
            const int64_t tilewidth = mapXML->Int64Attribute("tilewidth");
            const int64_t tileheight = mapXML->Int64Attribute("tileheight");
            assert(tilewidth >= 0 || tileheight >= 0);

            m_map->setTileSize(sf::Vector2u(tilewidth, tileheight));
        }

        tinyxml2::XMLElement* layer = mapXML->FirstChildElement();
        

        while (layer != nullptr) {

            if (0 == std::strcmp(layer->Name(), "editorsettings")) {

                if (0 != std::strcmp(layer->FirstChildElement("export")->Attribute("format"), "csv")) {
                    throw std::invalid_argument("format is not csv (editorsettings)");
                }

            }
            else if (0 == std::strcmp(layer->Name(), "tileset")) {

                if (!m_map->setTexture(layer->FirstChildElement("image")->Attribute("source"))) {
                    throw std::invalid_argument("image upload error");
                }

            }
            else if (0 == std::strcmp(layer->Name(), "layer")) {

                if (0 == std::strcmp(layer->Attribute("name"), "background")) {
                    
                    m_map->addBackGround(layer);
                }
                else if (0 == std::strcmp(layer->Attribute("name"), "frontground")) {

                    m_map->addFrontGround(layer); 
                }
                else {

                    m_map->addLayer(layer);
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

                        addObject (std::move (std::make_unique <Solid> (x, y, width, height)));

                    }
                    else if(0 == std::strcmp(obj->Attribute("type"), "solidabove")) {

                        addObject(std::move(std::make_unique <SolidAbove>(x, y, width, height)));

                    }
                    else if (0 == std::strcmp(obj->Attribute("type"), "playerspawn")) {

                        m_hero->setPositionHB(x, y);

                    }
                    else {

                        std::cout << "unknown tipe object: " << obj->Attribute("type") << std::endl;

                    }

                    obj = obj->NextSiblingElement();

                } // while (obj != nullptr)
            }

            layer = layer->NextSiblingElement();

        } // while (layer != nullptr)


        m_menu.setFont(FONT_FNAME);
        m_menu.setTextSize(FONT_DEFAULT_SIZE);
        m_menu.setTextColor(FONT_DEFAULT_COLOR);


        m_mod = GameMod::MainMenu;
        m_menu.setMenu(TipeMenu::Main);

        //m_menu.clear();
        //m_menu.setTextColor(sf::Color(192, 181, 149));

        //m_menu.setBackGround(MENU_BACKGROUND1_FNAME);

        //// -_-
        //m_menu.addButton(1, MenuButton::toGame, "Game",
        //    sf::Vector2f((m_view.getCenter().x + m_view.getSize().x / 5)
        //        , (m_view.getCenter().y - m_view.getSize().y / 2) + 128));

        //m_menu.addButton(2, MenuButton::toExit, "Exit",
        //    sf::Vector2f((m_view.getCenter().x + m_view.getSize().x / 5)
        //        , (m_view.getCenter().y - m_view.getSize().y / 2) + 200));



        //todo: проверка на состояние уровня (все ли загружено) 
        return true;
    } // loadLevelXML


    void NodeGame::checkEvents() noexcept {

        sf::Event event;
        while (m_window.pollEvent(event))
        {

            switch (m_mod)
            {
            case GameMod::Game:

                switch (event.type)
                {
                    ////////////////////////////Closed//////////////////////////////////////////////////////////
                case sf::Event::Closed:
                    m_mod = GameMod::Exit;
                    break;

                    ////////////////////////////KeyPressed//////////////////////////////////////////////////////
                case sf::Event::KeyPressed:
                    switch (event.key.code)
                    {

                    case sf::Keyboard::A:
                        m_hero->addSpeedX(-0.1);
                        break;


                    case sf::Keyboard::D:
                        m_hero->addSpeedX(0.1);
                        break;


                    case sf::Keyboard::Escape:
                        m_mod = GameMod::Pause;
                        m_hero->setSpeed(0, 0);

                        m_menu.setMenu(TipeMenu::Pause);

                    }
                    break; //case sf::Event::KeyPressed:


                case sf::Event::KeyReleased:

                    switch (event.key.code) {

                    case sf::Keyboard::A:
                        m_hero->addSpeedX(0.1);
                        break;

                    case sf::Keyboard::D:
                        m_hero->addSpeedX(-0.1);
                        break;
                    }
                    break; //case sf::Event::KeyReleased:
                }
                break; //case GameMod::Game:

            case GameMod::Pause:
            case GameMod::MainMenu:
                m_menu.checkEvent(event);
                break;

            default:
                
                break;
            }
        }


    }


    void NodeGame::checkKeyBoard() {

        switch (m_mod) {

        case GameMod::Game:

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {

                m_hero->jump();

            }

            break; // GameMode::Game

        case GameMod::MainMenu:
        case GameMod::Pause   :

            m_menu.checkKeyBoard();

            break; //Menu
        }

    }


    void NodeGame::update(float time) {

//************************************************************
// алгоритм(для каждого элемента):
//      1. смещаем все элементы по х (устанавливаем on_ground = false т.к. персонаж может упасть)
//      2. проверяем на взаиможействие с картой
//      3. смещаем все элементы по у
//      4. проверяем на взаимодействие с картой
//      6. устанавливаем спрайт в новую позицию
// все в цикл с проверкой на dзаимодействие элементов между собой
//************************************************************
        switch (m_mod) 
        {
        case GameMod::Game:

            upAllPosition(time, Direction::Horixontal);
            allColision(Direction::Horixontal);

            upAllPosition(time, Direction::Vertical);
            allColision(Direction::Vertical);

            //std::cout << m_hero->onGround() << std::endl;

            upSprite();
            m_view.setCenter(sf::Vector2f(getPosHeroX() * SCALE_ALL_X, getPosHeroY() * SCALE_ALL_Y));

            break;

        }

    } // update


    void NodeGame::upAllPosition(float time, Direction _dir) {

        m_hero->upPosition(time, _dir);

        for (const gsl::not_null < Entity* > elem : m_entities) {
            elem->upPosition(time, _dir);
        }

    }


    void NodeGame::allColision(Direction _dir) {

        for (const gsl::not_null < Entity* > elem : m_entities) {

            m_hero->colision(elem, _dir);
          
        }

    }


    void NodeGame::upSprite() {

        m_hero->upPosSprite();

        for (const gsl::not_null < Entity* > elem : m_entities) {
            elem->upPosSprite();
        }
    }


} //namecpace ezg