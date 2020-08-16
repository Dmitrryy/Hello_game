#include "tools/Game.h"

#include <SFML/Graphics.hpp>

#include <iostream>


int main()
{
	try
	{
		ezg::Game game;
		game.run();
	}
	catch (std::exception& e)
	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	}
}