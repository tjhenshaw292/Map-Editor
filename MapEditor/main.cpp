#include "stdafx.h"
#include "MapMaker.h"

int main()
{
	if (!MapMaker::mainFont.loadFromFile("Carlito-Regular.ttf"))
		throw;
	MapMaker makeMe("pokeTiles2.png", sf::Vector2u(16, 16), 19, 69);
	makeMe.setBlankTile(551);
	makeMe.setMapFileName("route01.dat");
	makeMe.load("route01.dat");
	makeMe.display();
	makeMe.save("route01.dat");
	return 0;
}