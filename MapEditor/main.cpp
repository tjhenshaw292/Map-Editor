#include "stdafx.h"
#include "MapMaker.h"

int main()
{
	if (!MapMaker::mainFont.loadFromFile("Carlito-Regular.ttf"))
		throw;
	MapMaker makeMe("pokeTiles2.png", sf::Vector2u(16, 16), 19, 69);
	makeMe.setBlankTile(551);
	makeMe.setMapFileName("bertHouse.dat");
	makeMe.load("bertHouse.dat");
	makeMe.display();
	makeMe.save("bertHouse.dat");
	return 0;
}