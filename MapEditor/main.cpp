#include "stdafx.h"
#include "MapMaker.h"

int main()
{
	if (!MapMaker::mainFont.loadFromFile("Carlito-Regular.ttf"))
		throw;
	MapMaker makeMe("pokeTiles2.png", sf::Vector2u(16, 16), 14, 22);
	makeMe.setBlankTile(551);
	makeMe.setMapFileName("intro.dat");
	makeMe.load("intro.dat");
	makeMe.display();
	makeMe.save("intro.dat");
	return 0;
}