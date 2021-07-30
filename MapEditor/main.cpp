#include "stdafx.h"
#include "MapMaker.h"

int main()
{
	if (!MapMaker::mainFont.loadFromFile("Carlito-Regular.ttf"))
		throw;
	MapMaker makeMe("gen3indoors.png", sf::Vector2u(16, 16), 25, 25);
	makeMe.setBlankTile(0); //551 for gen 1.....0 for gen 3

	std::string fileName{ "stadiumRoom1.dat" };

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