#include "stdafx.h"
#include "MapMaker.h"

int main()
{
	if (!MapMaker::mainFont.loadFromFile("Carlito-Regular.ttf"))
		throw;
	MapMaker makeMe("pokeTiles2.png", sf::Vector2u(16, 16), 22, 16);
	makeMe.setBlankTile(551);
	makeMe.setMapFileName("burtHouseInside.dat");
	makeMe.load("burtHouseInside.dat");
	makeMe.display();
	makeMe.save("burtHouseInside.dat");
	return 0;
}