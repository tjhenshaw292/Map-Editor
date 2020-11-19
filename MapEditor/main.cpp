#include "stdafx.h"
#include "MapMaker.h"

int main()
{
	if (!MapMaker::mainFont.loadFromFile("Carlito-Regular.ttf"))
		throw;
	MapMaker makeMe("gen3Indoors.png", sf::Vector2u(16, 16), 12, 12);
	makeMe.setBlankTile(0);

	std::string fileName{ "rightHouse02.dat" };

	makeMe.setMapFileName(fileName);
	//makeMe.load(fileName);

	//In order to use this, just add the row/col then reOpen mapmaker
	//makeMe.addColumnToLeft(1);
	//makeMe.addColumnToRight(1);

	makeMe.display();
	makeMe.save(fileName);
	return 0;
}