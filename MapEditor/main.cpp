#include "stdafx.h"
#include "MapMaker.h"

int main()
{
	if (!MapMaker::mainFont.loadFromFile("Carlito-Regular.ttf"))
		throw;
	MapMaker makeMe("pokeTiles2.png", sf::Vector2u(16, 16), 14, 6);
	makeMe.setBlankTile(551);
	makeMe.setMapFileName("wideHouse.dat");
	//makeMe.load("wideHouse.dat");

	//In order to use this, just add the row/col then reOpen mapmaker
	//makeMe.addColumnToLeft(1);
	//makeMe.addColumnToRight(1);

	makeMe.display();
	makeMe.save("wideHouse.dat");
	return 0;
}