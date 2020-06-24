#include <SFML/Graphics.hpp>

#include <iostream>
#include "../Header/NodeGame.h"



int main()
{
    ezg::NodeGame game;


    if (!game.loadLevel("Resource/Images/Mario_tileset.png", sf::Vector2u(16, 16)/*, level*/, MAP_WIDTH, MAP_HEIGHT)) {
        return 1;
    }
    

    sf::Texture txlist;
    if (!txlist.loadFromFile("Resource/Images/Mario_tileset.png")) {
        return 2;
    }

    sf::Font font;
    if (!font.loadFromFile("Resource/calibri.ttf")) {
        return 3;
    }

    sf::Text fps;
    fps.setFont(font);
    fps.setFillColor(sf::Color::Green);
    fps.setCharacterSize(25);

    game.loadHero(txlist, HERO_TEXTURE_LOC_X, HERO_TEXTURE_LOC_Y, 50, 50, HERO_WIDTH, HERO_HEIGHT);


    //вид дл€ скролинга камеры (прикреплена к герою)
    sf::View mainView(sf::FloatRect(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT));


    // create the window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Hello Game (ezg)");



    ///////////////////////////////////////test////////////////////////////////////////

    ezg::Solid* sol1 = new ezg::Solid (0, 208, 432, 16);
    game.m_entities.push_back(sol1);

    ezg::Solid* sol2 = new ezg::Solid (32, 160, 16, 48);
    game.m_entities.push_back(sol2);
     
    ezg::Solid* sol3 = new ezg::Solid (48, 176, 16, 16);
    game.m_entities.push_back(sol3);

    ezg::Solid* sol4 = new ezg::Solid (96, 144, 80, 16);
    game.m_entities.push_back(sol4);

    ezg::Solid* sol5 = new ezg::Solid (208, 160, 32, 16);
    game.m_entities.push_back(sol5);

    ///////////////////////////////////////////////////////////////////////////////////






    //запускаем таймер дл€ регулировки скорости игры
    sf::Clock clock;
  
    double gameTime = 0;

    // run the main loop
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();

        
        //fps.setString(std::to_string(static_cast<int>(1.0 / time)));
        //gameTime += time;

        // регул€тор скорости игры
        time = time * 1200;



        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {

            case sf::Event::Closed:

                window.close();
                break;

            case sf::Event::KeyReleased:

                switch (event.key.code) {

                case sf::Keyboard::Left:
                    game.m_hero.setSpeedX(0);
                    break;

                case sf::Keyboard::Right:
                    game.m_hero.setSpeedX(0);
                    break;
                }

                break;
            }

        }


        // взаимодействие пользовател€ с игрой 
        // todo: вынести в отдельную функцию
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {

            game.m_hero.setSpeedX(-0.1);

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {

            game.m_hero.setSpeedX(0.1);
          
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            
            game.m_hero.jump();

        }


        game.update(time);
        

        // draw the map
        window.clear(); 

        sf::Transform tr;
        tr.scale(4, 4);

        //printf("%f    %f\n", game.m_hero.m_hit_box.left * 4, game.m_hero.m_hit_box.top * 4);

        mainView.setCenter(sf::Vector2f(game.m_hero.m_hit_box.left * 4, game.m_hero.m_hit_box.top * 4));
        window.setView(mainView);

        //fps.setPosition(mainView.getCenter().x - WINDOW_WIDTH / 2, mainView.getCenter().y - WINDOW_HEIGHT / 2);

        window.draw(game, tr);
        //window.draw(fps);

        window.display();

    } // while (window.isOpen())


    return 0;
} // main()