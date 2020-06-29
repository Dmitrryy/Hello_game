
#include "../Header/NodeGame.h"



int main()
{
    ezg::NodeGame game;

    game.loadLevelXML("Resource/Levels/lvl2.tmx");

    //sf::Font font;
    //if (!font.loadFromFile("Resource/19440.ttf")) {
    //    return 3;
    //}
    //for fps
    //sf::Text fps;
    //fps.setFont(font);
    //fps.setFillColor(sf::Color::Green);
    //fps.setCharacterSize(25);


    sf::Clock clock;
  
    //double gameTime = 0;

    // run the main loop
    while (game.getMod() != ezg::GameMod::Exit)
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();

        //fps
        //fps.setString(std::to_string(static_cast<int>(1.0 / time)));
        //gameTime += time;

        // game speed controller
        time = time * GAME_SPEED_CONSTROLLER;


        game.checkEvents ();
        game.checkKeyBoard ();


        game.update(time);
       

        sf::Transform tr;
        tr.scale(SCALE_ALL_X, SCALE_ALL_Y);

        game.draw(tr);

    } // while (window.isOpen())

    return 0;
} // main()