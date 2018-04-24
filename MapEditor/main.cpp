#include "stdafx.h"
#include "MapMaker.h"

int main()
{
	if (!MapMaker::mainFont.loadFromFile("Carlito-Regular.ttf"))
		throw;
	MapMaker makeMe("pokeTiles2.png", sf::Vector2u(16, 16), 32, 69);
	makeMe.setBlankTile(551);
	makeMe.setMapFileName("superMaze.dat");
	makeMe.load("superMaze.dat");
	makeMe.display();
	makeMe.save("superMaze.dat");
	return 0;
}