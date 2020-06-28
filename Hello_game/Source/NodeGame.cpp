
#include "../Header/NodeGame.h"


namespace ezg {


    NodeGame::~NodeGame()
    {

        m_map = nullptr;
        m_hero = nullptr;

        m_entities.clear();
    }


    void NodeGame::draw(sf::RenderTarget& target, sf::RenderStates states) const /* override */ {

        m_map->drawBackGround(target, states);
        m_map->drawMap(target, states);


        m_hero->draw(target, states);


        for (auto elem : m_entities) {

            elem->draw(target, states);

        }

        m_map->drawFrontGround(target, states);
    }


    bool NodeGame::loadLevelXML(const std::string& _fileXML) {

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
                    //std::cout << "back\n";
                    m_map->addBackGround(layer);
                }
                else if (0 == std::strcmp(layer->Attribute("name"), "frontground")) {
                    m_map->addFrontGround(layer);
                    //std::cout << "front\n";
                }
                else {
                    m_map->addLayer(layer);
                    //std::cout << "layer\n";

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

        //todo: проверка на состояние уровня (все ли загружено) 
        return true;
    } // loadLevelXML


    void NodeGame::checkEvent(const sf::Event& _event) noexcept {

        switch (_event.type)
        {

        case sf::Event::Closed:

            m_mod = GameMod::Exit;
            break;

        case sf::Event::KeyReleased:

            switch (_event.key.code) {

            case sf::Keyboard::A:
                m_hero->addSpeedX(0.1);
                break;

            case sf::Keyboard::D:
                m_hero->addSpeedX(-0.1);
                break;
            }

            break;


        case sf::Event::KeyPressed:

            switch (_event.key.code) {

            case sf::Keyboard::A:
                m_hero->addSpeedX(-0.1);
                break;

            case sf::Keyboard::D:
                m_hero->addSpeedX(0.1);
                break;

            //case sf::Keyboard::W:
            //    m_hero->jump();
            //    break;
            }

            break;
        }

    }


    void NodeGame::checkKeyBoard() {

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {

            m_hero->jump();

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

        upAllPosition(time, Direction::Horixontal);
        allColision(Direction::Horixontal);

        upAllPosition(time, Direction::Vertical);
        allColision(Direction::Vertical);

        //std::cout << m_hero->onGround() << std::endl;

        upSprite();

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