#include "stdafx.h"
#include "MapMaker.h"

int main()
{
	if (!MapMaker::mainFont.loadFromFile("Carlito-Regular.ttf"))
		throw;
	MapMaker makeMe("gen3Outdoors.png", sf::Vector2u(16, 16), 14, 28);
	makeMe.setBlankTile(0);

	std::string fileName{ "route03.dat" };

	makeMe.setMapFileName(fileName);
	makeMe.load(fileName);

	//In order to use this, just add the row/col then reOpen mapmaker
	//makeMe.addRowToTop(1);
	//makeMe.addColumnToLeft(1);
	//makeMe.addColumnToRight(1);

	makeMe.display();
	makeMe.save(fileName);
	return 0;
}