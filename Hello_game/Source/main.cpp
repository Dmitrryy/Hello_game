
#include "../Header/NodeGame.h"



int main()
{
    ezg::NodeGame game;

    game.loadLevelXML("Resource/Levels/lvl2.tmx");

    sf::Font font;
    if (!font.loadFromFile("Resource/calibri.ttf")) {
        return 3;
    }

    //for fps
    sf::Text fps;
    fps.setFont(font);
    fps.setFillColor(sf::Color::Green);
    fps.setCharacterSize(25);

    //view for scrolling the camera (attached to the hero)
    sf::View mainView(sf::FloatRect(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT));


    // create the window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Hello Game (ezg)");
    window.setKeyRepeatEnabled(false);


    sf::Clock clock;
  
    //double gameTime = 0;

    // run the main loop
    while (game.getMod() != ezg::GameMod::Exit)
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();

        //fps
        fps.setString(std::to_string(static_cast<int>(1.0 / time)));
        //gameTime += time;

        // game speed controller
        time = time * GAME_SPEED_CONSTROLLER;


        // handle events
        sf::Event event;
        while (window.pollEvent (event))
        {

            game.checkEvent (event);

        }

        game.checkKeyBoard ();


        game.update(time);
        

        // draw the game
        window.clear(); 

        sf::Transform tr;
        tr.scale(SCALE_ALL_X, SCALE_ALL_Y);

        
        mainView.setCenter(sf::Vector2f(game.getPosHeroX() * SCALE_ALL_X, game.getPosHeroY() * SCALE_ALL_Y));
        window.setView(mainView);

        fps.setPosition(mainView.getCenter().x - WINDOW_WIDTH / 2, mainView.getCenter().y - WINDOW_HEIGHT / 2);

        game.draw(window, tr);
        window.draw(fps);

        window.display();

    } // while (window.isOpen())


    window.close();

    return 0;
} // main()