
#include "Header/NodeGame.h"



int main()
{

    ezg::NodeGame game;


    // run the main loop
    while (game.getMood() != ezg::GameMood::Exit)
    {

        game.checkEvents ();
        game.checkKeyBoard ();


        game.update();
       

        sf::Transform tr;
        tr.scale(SCALE_ALL_X, SCALE_ALL_Y);


        game.draw(tr);

    }



    return 0;

} // main()