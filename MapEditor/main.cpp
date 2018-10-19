#include "stdafx.h"
#include "MapMaker.h"

int main()
{
	if (!MapMaker::mainFont.loadFromFile("Carlito-Regular.ttf"))
		throw;
	MapMaker makeMe("pokeTiles2.png", sf::Vector2u(16, 16), 20, 69);
	makeMe.setBlankTile(551);
	makeMe.setMapFileName("burtHouse.dat");
	makeMe.load("burtHouse.dat");

	//In order to use this, just add the row/col then reOpen mapmaker
	//makeMe.addColumnToLeft(1);
	//makeMe.addColumnToRight(1);

	makeMe.display();
	makeMe.save("burtHouse.dat");
	return 0;
}