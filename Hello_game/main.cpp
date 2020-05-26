#include <SFML/Graphics.hpp>

#include "NodeGame.h"
#include <iostream>


int main()
{
    ezg::NodeGame game;


    if (!game.loadLevel("Resource/Images/TileMap.png", sf::Vector2u(16, 16)/*, level*/, MAP_WIDTH, 8)) {
        return 1;
    }
    

    sf::Texture txlist;
    if (!txlist.loadFromFile("Resource/Images/Mario_tileset.png")) {
        return 2;
    }

    game.loadHero(txlist, 50, 50, HERO_WIDTH, HERO_HEIGHT);


    //вид дл€ скролинга камеры (прикреплена к герою)
    sf::View mainView(sf::FloatRect(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT));


    // create the window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tilemap");


    //запускаем таймер дл€ регулировки скорости игры
    sf::Clock clock;
  

    // run the main loop
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();

        // регул€тор скорости игры
        time /= 800;


        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {

                window.close();

            }
        }


        // взаимодействие пользовател€ с игрой 
        // todo: вынести в отдельную функцию
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {

            game.m_hero.addSpeedX(-0.1);

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {

            game.m_hero.addSpeedX(0.1);
          
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            
            game.m_hero.jump();

        }


        game.update(time);
        

        // draw the map
        window.clear(); 

        sf::Transform tr;
        tr.scale(4, 4);

        mainView.setCenter(sf::Vector2f(game.m_hero.m_hit_box.left * 4, game.m_hero.m_hit_box.top * 4));
        window.setView(mainView);

        window.draw(game, tr);

        window.display();
    
    } // while (window.isOpen())


    return 0;
}